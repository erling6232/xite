/****************************************************************************

	DirMgr.c

	This file contains the C code to implement the DirectoryMgr system.

	This system is intended to manage filtered and sorted directory
	lists.

 ****************************************************************************/

/*
 * Author:
 * 	Brian Totty
 * 	Department of Computer Science
 * 	University Of Illinois at Urbana-Champaign
 *	1304 West Springfield Avenue
 * 	Urbana, IL 61801
 * 
 * 	totty@cs.uiuc.edu
 * 	
 */ 

#if defined(AIX) && !defined(__GNUC__) && !defined(_ALL_SOURCE)
# define _ALL_SOURCE
#endif

#include <Xfwf/DirMgr.h>

#ifndef NO_REGEXP
#include <Xfwf/RegExp.h>
#endif
#include <stdlib.h>
#include <xite/includes.h>
#include XITE_STDIO_H
#include XITE_MALLOC_H

#define	DIR_MGR_FSM_SIZE 1024

/*---------------------------------------------------------------------------*

                   S I M P L E    I N T E R F A C E

 *---------------------------------------------------------------------------*/

DirectoryMgr *DirectoryMgrSimpleOpen(char *path, int sort_type, char *pattern)
{
	DirectoryMgr *dm;
	PFI f_func;
	PSI s_func;
# ifdef REGEX
	regex_t *f_data;
# else /* REGEX */
	char *f_data;
# endif /* !REGEX */

	if (pattern == NULL) pattern = "*";
	if (!DirectoryMgrSimpleFilterFunc(pattern,&f_func,&f_data))
	{
		return(NULL);
	}
	if (!DirectoryMgrSimpleSortingFunc(sort_type,&s_func))
	{
# ifdef REGEX
	        regfree(f_data);
# else /* REGEX */
		free(f_data);
# endif /* !REGEX */
		return(NULL);
	}
	dm = DirectoryMgrOpen(path,s_func,f_func,f_data,TRUE);
	return(dm);
} /* End DirectoryMgrSimpleOpen */


int DirectoryMgrSimpleRefilter(DirectoryMgr *dm, char *pattern)
{
	PFI f_func;
# ifdef REGEX
        regex_t *f_data;
# else /* REGEX */
	char *f_data;
# endif /* !REGEX */

	if (!DirectoryMgrSimpleFilterFunc(pattern,&f_func,&f_data))
	{
		return(FALSE);
	}
	DirectoryMgrRefilter(dm,f_func,f_data,TRUE);
	return(TRUE);
} /* End DirectoryMgrSimpleRefilter */


int DirectoryMgrSimpleResort(DirectoryMgr *dm, int sort_type)
{
	PSI c_func;

	if (!DirectoryMgrSimpleSortingFunc(sort_type,&c_func))
	{
		return(FALSE);
	}
	DirectoryMgrResort(dm,c_func);
	return(TRUE);
} /* End DirectoryMgrSimpleResort */


/*---------------------------------------------------------------------------*

                    N O R M A L    I N T E R F A C E

 *---------------------------------------------------------------------------*/

int DirectoryMgrCanOpen(char *path)
{
	int status;
	Directory dir;

	status = DirectoryOpen(path,&dir);
	if (status == TRUE) DirectoryClose(&dir);
	return(status);
} /* End DirectoryMgrCanOpen */


# ifdef REGEX
DirectoryMgr *DirectoryMgrOpen(char *path, PSI c_func, PFI f_func, regex_t *f_data, int free_data)
# else /* REGEX */
DirectoryMgr *DirectoryMgrOpen(char *path, PSI c_func, PFI f_func, char *f_data, int free_data)
# endif /* REGEX */
{
	DirectoryMgr *dm;

	dm = (DirectoryMgr *)calloc(1,sizeof(DirectoryMgr));
	if (dm == NULL)
	{
		fprintf(stderr,"DirectoryMgrOpen: out of memory\n");
		if (free_data && f_data)
# ifdef REGEX
		        regfree(f_data);
# else /* REGEX */
		        free(f_data);
# endif /* !REGEX */

		return(NULL);
	}
	if (DirectoryOpen(path,DirectoryMgrDir(dm)) == FALSE)
	{
		fprintf(stderr,"DirectoryMgrOpen: can't open dir '%s'\n",
			DirectoryPath(DirectoryMgrDir(dm)));
		free(dm);
		if (free_data && f_data)
# ifdef REGEX
                        regfree(f_data);
# else /* REGEX */
                        free(f_data);
# endif /* !REGEX */

		return(NULL);
	}
	DirectoryMgrCompFunc(dm) = c_func;
	DirectoryMgrRefilter(dm,f_func,f_data,free_data);
	return(dm);
} /* End DirectoryMgrOpen */


void DirectoryMgrClose(DirectoryMgr *dm)
{
	free(DirectoryMgrData(dm));
	free(DirectoryMgrSortedPtrs(dm));
	if (DirectoryMgrFilterData(dm) && DirectoryMgrFreeFilterData(dm))
	{
		free(DirectoryMgrFilterData(dm));
	}
	DirectoryClose(DirectoryMgrDir(dm));
	free(dm);
} /* End DirectoryMgrClose */


# ifdef REGEX
int DirectoryMgrRefilter(DirectoryMgr *dm, PFI f_func, regex_t *f_data, int f_free)
# else /* REGEX */
int DirectoryMgrRefilter(DirectoryMgr *dm, PFI f_func, char *f_data, int f_free)
# endif /* REGEX */
{
	if (DirectoryMgrFilterData(dm) && DirectoryMgrFreeFilterData(dm))
	{
		free(DirectoryMgrFilterData(dm));
	}
	DirectoryMgrFilterFunc(dm) = f_func;
	DirectoryMgrFilterData(dm) = f_data;
	DirectoryMgrFreeFilterData(dm) = f_free;
	DirectoryMgrRefresh(dm);

	return(TRUE);

} /* End DirectoryMgrRefilter */


int DirectoryMgrRefresh(DirectoryMgr *dm)
{
	int err,data_size,ptrs_size,i;
	DirEntryCons *head,*tail,*cons;
	DirEntry *dm_data,**dm_ptrs;
	PFI f_func;
# ifdef REGEX
	regex_t *f_data;
# else /* REGEX */
	char *f_data;
# endif /* !REGEX */

	DirectoryMgrTotalCount(dm) = 0;
	DirectoryMgrFilteredCount(dm) = 0;
	DirectoryRestart(DirectoryMgrDir(dm));
	if (DirectoryMgrData(dm)) free(DirectoryMgrData(dm));
	if (DirectoryMgrSortedPtrs(dm)) free(DirectoryMgrSortedPtrs(dm));
	head = NULL;
	f_func = DirectoryMgrFilterFunc(dm);
	f_data = DirectoryMgrFilterData(dm);
	while (1)
	{
		cons = (DirEntryCons *)malloc(sizeof(DirEntryCons));
		if (cons == NULL)
		{
			fprintf(stderr,
				"DirectoryMgrRefresh: Can't Alloc Cons\n");
			exit(-1);
		}
		err = DirectoryReadNextEntry(DirectoryMgrDir(dm),
					     &(cons->dir_entry));
		if (err == FALSE)
		{
			free(cons);
			break;
		}
		++ DirectoryMgrTotalCount(dm);
		if ((f_func == NULL) ||
		    (DirEntryType(&(cons->dir_entry)) == F_TYPE_DIR) ||
		    (f_func && f_func(&(cons->dir_entry),f_data)))
		{
			cons->next = NULL;
			if (head == NULL)
				head = cons;
			    else
				tail->next = cons;
			tail = cons;
			++ DirectoryMgrFilteredCount(dm);
		}
		    else if (DirEntryType(&(cons->dir_entry)) == F_TYPE_DIR)
  	        {
		  /* Do not filter out directory entries. [svein@ifi.uio.no] */
			cons->next = NULL;
			if (head == NULL)
				head = cons;
			    else
				tail->next = cons;
			tail = cons;
			++ DirectoryMgrFilteredCount(dm);
		}
		    else			/* Filter Failed */
		{
			free(cons);
		}
	}

	data_size = sizeof(DirEntry) * DirectoryMgrFilteredCount(dm);
	ptrs_size = sizeof(DirEntry *) * DirectoryMgrFilteredCount(dm);
	dm_data = (DirEntry *)malloc(data_size);
	dm_ptrs = (DirEntry **)malloc(ptrs_size);
	if ((dm_data == NULL) || (dm_ptrs == NULL))
	{
		fprintf(stderr,"DirectoryMgrRefresh: Out of memory\n");
		exit(1);
	}
	DirectoryMgrData(dm) = dm_data;
	DirectoryMgrSortedPtrs(dm) = dm_ptrs;

	for (i = 0; i < DirectoryMgrFilteredCount(dm); i++)
	{
		DirectoryMgrData(dm)[i] = head->dir_entry;
		DirectoryMgrSortedPtrs(dm)[i] = &(DirectoryMgrData(dm)[i]);
		cons = head->next;
		free(head);
		head = cons;
	}

	DirectoryMgrResort(dm,DirectoryMgrCompFunc(dm));
	DirectoryMgrRestart(dm);
	return(TRUE);
} /* End DirectoryMgrRefresh */


void DirectoryMgrResort(DirectoryMgr *dm, PSI c_func)
{
	DirectoryMgrCompFunc(dm) = c_func;
	if (c_func != NULL)
	{
	        qsort(DirectoryMgrSortedPtrs(dm),DirectoryMgrFilteredCount(dm),
		      sizeof(DirEntry *),
		      (int (*)(const void*, const void*))
		      DirectoryMgrCompFunc(dm));
	}
	DirectoryMgrRestart(dm);
} /* End DirectoryMgrResort */

/*---------------------------------------------------------------------------*

                  I T E R A T I O N    C O M M A N D S

 *---------------------------------------------------------------------------*/

int DirectoryMgrGotoItem(DirectoryMgr *dm, int i)
{
	if (i < 0 || i >= DirectoryMgrFilteredCount(dm)) return(FALSE);
	DirectoryMgrCurrentIndex(dm) = i;
	return(TRUE);
} /* End DirectoryMgrGotoItem */


int DirectoryMgrGotoNamedItem(DirectoryMgr *dm, char *name)
{
	int i;
	DirEntry *entry;

	for (i = 0; i < DirectoryMgrFilteredCount(dm); i++)
	{
		entry = DirectoryMgrSortedPtrs(dm)[i];
		if (strcmp(DirEntryFileName(entry),name) == 0)
		{
			DirectoryMgrCurrentIndex(dm) = i;
			return(TRUE);
		}
	}
	return(FALSE);
} /* End DirectoryMgrGotoNamedItem */


void DirectoryMgrRestart(DirectoryMgr *dm)
{
	DirectoryMgrCurrentIndex(dm) = 0;
} /* End DirectoryMgrRestart */


DirEntry *DirectoryMgrCurrentEntry(DirectoryMgr *dm)
{
	int index;

	index = DirectoryMgrCurrentIndex(dm);
	if (index < 0 || index >= DirectoryMgrFilteredCount(dm)) return(NULL);
	return(DirectoryMgrSortedPtrs(dm)[index]);
} /* End DirectoryMgrCurrentEntry */


DirEntry *DirectoryMgrNextEntry(DirectoryMgr *dm)
{
	int index;

	index = DirectoryMgrCurrentIndex(dm);
	if (index >= DirectoryMgrFilteredCount(dm)) return(NULL);
	++ DirectoryMgrCurrentIndex(dm);
	return(DirectoryMgrSortedPtrs(dm)[index]);
} /* End DirectoryMgrNextEntry */


DirEntry *DirectoryMgrPrevEntry(DirectoryMgr *dm)
{
	int index;

	index = DirectoryMgrCurrentIndex(dm) - 1;
	if (index < 0) return(NULL);
	-- DirectoryMgrCurrentIndex(dm);
	return(DirectoryMgrSortedPtrs(dm)[index]);
} /* End DirectoryMgrPrevEntry */

/*---------------------------------------------------------------------------*

                   U T I L I T Y    F U N C T I O N S

 *---------------------------------------------------------------------------*/

# ifdef REGEX
int DirectoryMgrSimpleFilterFunc(char *pattern, PFI *ff_ptr, regex_t **fd_ptr)
# else /* REGEX */
int DirectoryMgrSimpleFilterFunc(char *pattern, PFI *ff_ptr, char **fd_ptr)
# endif /* !REGEX */
{
#ifndef	NO_REGEXP
	char regexp[2048];

	*ff_ptr = DirectoryMgrFilterName;
# ifdef REGEX
	*fd_ptr = (regex_t *) malloc(sizeof(regex_t));
# else /* REGEX */
	*fd_ptr = (char *)malloc(sizeof(char) * DIR_MGR_FSM_SIZE);
# endif /* !REGEX */
	if (*fd_ptr == NULL) return(FALSE);
	RegExpPatternToRegExp(pattern, regexp);
	RegExpCompile(regexp, *fd_ptr, DIR_MGR_FSM_SIZE);
#endif
	return(TRUE);
} /* End DirectoryMgrSimpleFilterFunc */


int DirectoryMgrSimpleSortingFunc(int sort_type, PSI *sf_ptr)
{
	*sf_ptr = NULL;
	switch (sort_type)
	{
	    case DIR_MGR_SORT_NONE:
		break;
	    case DIR_MGR_SORT_NAME:
		*sf_ptr = DirectoryMgrCompareName;
		break;
	    case DIR_MGR_SORT_SIZE_ASCENDING:
		*sf_ptr = DirectoryMgrCompareSizeAscending;
		break;
	    case DIR_MGR_SORT_SIZE_DESCENDING:
		*sf_ptr = DirectoryMgrCompareSizeDescending;
		break;
	    case DIR_MGR_SORT_NAME_DIRS_FIRST:
		*sf_ptr = DirectoryMgrCompareNameDirsFirst;
		break;
	    case DIR_MGR_SORT_ACCESS_ASCENDING:
		*sf_ptr = DirectoryMgrCompareLastAccessAscending;
		break;
	    case DIR_MGR_SORT_ACCESS_DESCENDING:
		*sf_ptr = DirectoryMgrCompareLastAccessDescending;
		break;
	    default:
		fprintf(stderr,"Bad sort type %d\n",sort_type);
		return(FALSE);
	}
	return(TRUE);
} /* End DirectoryMgrSimpleSortingFunc */

/*---------------------------------------------------------------------------*

                    S O R T I N G    R O U T I N E S

 *---------------------------------------------------------------------------*/

int DirectoryMgrCompareName(DirEntry **e1p, DirEntry **e2p)
{
	return(strcmp(DirEntryFileName(*e1p),DirEntryFileName(*e2p)));
} /* End DirectoryMgrCompareName */


int DirectoryMgrCompareNameDirsFirst(DirEntry **e1p, DirEntry **e2p)
{
	if (DirEntryLeadsToDir(*e1p))
	{
		if (!DirEntryLeadsToDir(*e2p)) return(-1);
	}
	    else if (DirEntryLeadsToDir(*e2p))
	{
		return(1);
	}
	return(strcmp(DirEntryFileName(*e1p),DirEntryFileName(*e2p)));
} /* End DirectoryMgrCompareNameDirsFirst */


int DirectoryMgrCompareSizeAscending(DirEntry **e1p, DirEntry **e2p)
{
	if (DirEntryFileSize(*e1p) < DirEntryFileSize(*e2p))
		return (-1);
	    else if (DirEntryFileSize(*e1p) == DirEntryFileSize(*e2p))
		return (0);
	    else
		return (1);
} /* End DirectoryMgrCompareSizeAscending */


int DirectoryMgrCompareSizeDescending(DirEntry **e1p, DirEntry **e2p)
{
	if (DirEntryFileSize(*e1p) > DirEntryFileSize(*e2p))
		return (-1);
	    else if (DirEntryFileSize(*e1p) == DirEntryFileSize(*e2p))
		return (0);
	    else
		return (1);
} /* End DirectoryMgrCompareSizeDescending */


int DirectoryMgrCompareLastAccessAscending(DirEntry **e1p, DirEntry **e2p)
{
	return((long)DirEntryLastAccess(*e1p) >
	       (long)DirEntryLastAccess(*e2p));
} /* End DirectoryMgrCompareLastAccessAscending */


int DirectoryMgrCompareLastAccessDescending(DirEntry **e1p, DirEntry **e2p)
{
	return((long)DirEntryLastAccess(*e1p) <
	       (long)DirEntryLastAccess(*e2p));
} /* End DirectoryMgrCompareLastAccessDescending */

/*---------------------------------------------------------------------------*

                     F I L T E R    R O U T I N E S

 *---------------------------------------------------------------------------*/

# ifdef REGEX
int DirectoryMgrFilterName(DirEntry *de, regex_t *fsm)
# else /* REGEX */
int DirectoryMgrFilterName(DirEntry *de, char *fsm)
# endif /* REGEX */
{
#ifndef	NO_REGEXP
# ifdef REGEX
	return(RegExpMatch(DirEntryFileName(de), fsm));
# else /* REGEX */
	return(RegExpMatch(DirEntryFileName(de), fsm));
# endif /* REGEX */
#else
	return(TRUE);
#endif
} /* End DirectoryMgrFilterName */
