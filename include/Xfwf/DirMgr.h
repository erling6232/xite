/****************************************************************************

	DirMgr.h

	This file contains the C declarations and definitions for the
	DirectoryMgr system.

	This system is intended to managed filtered and sorted directory
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

#ifndef _FWF_DIRECTORY_MGR_H_
#define	_FWF_DIRECTORY_MGR_H_

#include <xite/includes.h>
#include <X11/Xfuncproto.h>
#include <Xfwf/Directory.h>
#include <Xfwf/RegExp.h>

/*---------------------------------------------------------------------------*

        Simple DirectoryMgr Interface

		DirectoryMgrSimpleOpen();
		DirectoryMgrSimpleRefilter();
		DirectoryMgrSimpleResort();

	Standard DirectoryMgr Interface

		DirectoryMgrCanOpen();
		DirectoryMgrOpen();
		DirectoryMgrClose();
		DirectoryMgrRefilter();
		DirectoryMgrRefresh();
		DirectoryMgrResort();

	Moving Around Items

		DirectoryMgrGotoItem();
		DirectoryMgrGotoNamedItem();
		DirectoryMgrRestart();
		DirectoryMgrGetIndex();
		DirectoryMgrCurrentEntry();
		DirectoryMgrNextEntry();
		DirectoryMgrPrevEntry();

	Utility Functions

		DirectoryMgrSimpleFilterFunc();
		DirectoryMgrSimpleSortingFunc();

	Comparison Functions

		DirectoryMgrCompareName();
		DirectoryMgrCompareSizeAscending();
		DirectoryMgrCompareSizeDescending();

	Macros

		DirectoryMgrDir();
		DirectoryMgrData();
		DirectoryMgrSortedPtrs();
		DirectoryMgrFilterFunc();
		DirectoryMgrCompFunc();
		DirectoryMgrFilterData();
		DirectoryMgrFreeFilterData();
		DirectoryMgrTotalCount();
		DirectoryMgrFilteredCount();
		DirectoryMgrCurrentIndex();


 *---------------------------------------------------------------------------*/

#ifndef PFI
    typedef int (*PSI)(DirEntry **e1p, DirEntry **e2p);
#  ifdef REGEX
    typedef int (*PFI)(DirEntry *de, regex_t *fsm);
#  else /* REGEX */
    typedef int (*PFI)(DirEntry *de, char *fsm);
#  endif /* !REGEX */
#endif

typedef struct entry_cons
{
	DIR_ENTRY dir_entry;
	struct entry_cons *next;
} DIR_ENTRY_CONS;

typedef	DIR_ENTRY_CONS DirEntryCons;

typedef struct
{
	DIRECTORY dir;
	DIR_ENTRY *data;
	DIR_ENTRY **sorted_ptrs;
	int total_count;
	int filtered_count;
	PFI filter_func;
# ifdef REGEX
	regex_t *filter_data;
# else /* REGEX */
	char *filter_data;
# endif /* !REGEX */
	int free_filter_data;
	PSI comp_func;
	int current_index;
} DIRECTORY_MGR;

typedef	DIRECTORY_MGR DirectoryMgr;

#define	DIR_MGR_SORT_NONE			0
#define	DIR_MGR_SORT_NAME			1
#define	DIR_MGR_SORT_NAME_DIRS_FIRST		2
#define DIR_MGR_SORT_SIZE_ASCENDING		3
#define DIR_MGR_SORT_SIZE_DESCENDING		4
#define	DIR_MGR_SORT_ACCESS_ASCENDING		5
#define	DIR_MGR_SORT_ACCESS_DESCENDING		6

#define	DirectoryMgrDir(dm)		(&((dm)->dir))
#define	DirectoryMgrData(dm)		((dm)->data)
#define	DirectoryMgrSortedPtrs(dm)	((dm)->sorted_ptrs)
#define	DirectoryMgrFilterFunc(dm)	((dm)->filter_func)
#define	DirectoryMgrCompFunc(dm)	((dm)->comp_func)
#define	DirectoryMgrFilterData(dm)	((dm)->filter_data)
#define	DirectoryMgrFreeFilterData(dm)	((dm)->free_filter_data)
#define	DirectoryMgrTotalCount(dm)	((dm)->total_count)
#define	DirectoryMgrFilteredCount(dm)	((dm)->filtered_count)
#define	DirectoryMgrCurrentIndex(dm)	((dm)->current_index)

DirectoryMgr *	DirectoryMgrSimpleOpen(char *path, int sort_type,
			char *pattern);
int		DirectoryMgrSimpleRefilter(DirectoryMgr *dm, char *pattern);
int		DirectoryMgrSimpleResort(DirectoryMgr *dm, int sort_type);

int		DirectoryMgrCanOpen(char *path);
# ifdef REGEX
DirectoryMgr *	DirectoryMgrOpen(char *path, PSI c_func, PFI f_func,
			regex_t *f_data, int free_data);
# else /* REGEX */
DirectoryMgr *	DirectoryMgrOpen(char *path, PSI c_func, PFI f_func,
			char *f_data, int free_data);
# endif /* !REGEX */
void		DirectoryMgrClose(DirectoryMgr *dm);
# ifdef REGEX
int		DirectoryMgrRefilter(DirectoryMgr *dm, PFI f_func,
			regex_t *f_data, int f_free);
# else /* REGEX */
int		DirectoryMgrRefilter(DirectoryMgr *dm, PFI f_func,
			char *f_data, int f_free);
# endif /* !REGEX */
int		DirectoryMgrRefresh(DirectoryMgr *dm);
void		DirectoryMgrResort(DirectoryMgr *dm, PSI c_func);

int		DirectoryMgrGotoItem(DirectoryMgr *dm, int i);
int		DirectoryMgrGotoNamedItem(DirectoryMgr *dm, char *name);
void		DirectoryMgrRestart(DirectoryMgr *dm);
DirEntry *	DirectoryMgrCurrentEntry(DirectoryMgr *dm);
DirEntry *	DirectoryMgrNextEntry(DirectoryMgr *dm);
DirEntry *	DirectoryMgrPrevEntry(DirectoryMgr *dm);

# ifdef REGEX
int		DirectoryMgrSimpleFilterFunc(char *pattern, PFI *ff_ptr,
			regex_t **fd_ptr);
# else /* REGEX */
int		DirectoryMgrSimpleFilterFunc(char *pattern, PFI *ff_ptr,
			char **fd_ptr);
# endif /* !REGEX */
int		DirectoryMgrSimpleSortingFunc(int sort_type, PSI *sf_ptr);

int		DirectoryMgrCompareName(DirEntry **e1p, DirEntry **e2p);
int		DirectoryMgrCompareNameDirsFirst(DirEntry **e1p,
			DirEntry **e2p);
int		DirectoryMgrCompareSizeAscending(DirEntry **e1p,
			DirEntry **e2p);
int		DirectoryMgrCompareSizeDescending(DirEntry **e1p,
			DirEntry **e2p);
int		DirectoryMgrCompareLastAccessAscending(DirEntry **e1p,
			DirEntry **e2p);
int		DirectoryMgrCompareLastAccessDescending(DirEntry **e1p,
			DirEntry **e2p);

# ifdef REGEX
int		DirectoryMgrFilterName(DirEntry *de, regex_t *fsm);
# else /* REGEX */
int		DirectoryMgrFilterName(DirEntry *de, char *fsm);
# endif /* !REGEX */

#endif
