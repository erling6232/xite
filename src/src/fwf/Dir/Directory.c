/****************************************************************************

        Directory.c

	This file contains the C code that implements the directory
	iteration and file information subsystem.

	This code is intended to be used as a convenient, machine
	independent interface to iterate through the contents of a
	directory.

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

#include <Xfwf/Directory.h>
#include <xite/includes.h>
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#ifdef HAVE_UNISTD_H
#  include <unistd.h>
#endif
#ifdef HAVE_SYS_STAT_H
#  include <sys/stat.h>
#endif

/*--------------------------------------------------------------------------*

        L O W    L E V E L    D I R E C T O R Y    I N T E R F A C E

 *--------------------------------------------------------------------------*/

int DirectoryOpen(char *dir_name, Directory *dp)
{
	if (DirectoryPathExpand(dir_name,DirectoryPath(dp)) == NULL)
	{
		return(FALSE);
	}
	DirectoryDir(dp) = opendir(DirectoryPath(dp));
	if (DirectoryDir(dp) == NULL) return(FALSE);
	return(TRUE);
} /* End DirectoryOpen */


void DirectoryRestart(Directory *dp)
{
	rewinddir(DirectoryDir(dp));
} /* End DirectoryRestart */


void DirectoryClose(Directory *dp)
{
	closedir(DirectoryDir(dp));
} /* End DirectoryClose */


long DirectoryTellPosition(Directory *dp)
{
	return(telldir(DirectoryDir(dp)));
} /* End DirectoryTellPosition */


void DirectorySetPosition(Directory *dp, long int pos)
{
	seekdir(dp->filep,pos);                 /* BB, 8 Mar 95 */
} /* End DirectorySetPosition */


int DirectoryReadNextEntry(Directory *dp, DirEntry *de)
{
	u_short orig_file_type;
	static struct dirent *_ep;
	static struct stat _lstats,_stats;
	char full_path[MAXPATHLEN + 2];

	_ep = readdir(DirectoryDir(dp));
	if (_ep == NULL) return(FALSE);
	strcpy(DirEntryFileName(de),_ep->d_name);
	strcpy(full_path,DirectoryPath(dp));
	strcat(full_path,DirEntryFileName(de));

	if (lstat(full_path,&_lstats) != 0) return(FALSE);

	orig_file_type = _lstats.st_mode & S_IFMT;
	switch (orig_file_type)
	{
	    case S_IFDIR:
		DirEntryType(de) = F_TYPE_DIR;
		break;
	    case S_IFREG:
		DirEntryType(de) = F_TYPE_FILE;
		break;
	    case S_IFCHR:
		DirEntryType(de) = F_TYPE_CHAR_SPECIAL;
		break;
	    case S_IFBLK:
		DirEntryType(de) = F_TYPE_BLOCK_SPECIAL;
		break;
	    case S_IFLNK:
		DirEntryType(de) = F_TYPE_SYM_LINK;
		break;
	    case S_IFSOCK:
		DirEntryType(de) = F_TYPE_SOCKET;
		break;
#ifdef S_IFIFO
	    case S_IFIFO:
		DirEntryType(de) = F_TYPE_FIFO;
		break;
#endif
	    default:
		DirEntryType(de) = orig_file_type;
		break;
	}

	DirEntryIsBrokenLink(de) = FALSE;
	DirEntryIsDirectoryLink(de) = FALSE;
	if (DirEntryIsSymLink(de))			/* Symbolic Link */
	{
		if (stat(full_path,&_stats) != 0)	/* Can't Stat File */
		{
			DirEntryIsBrokenLink(de) = TRUE;
			_stats = _lstats;
		}
		    else				/* Link Not Broken */
		{
#ifdef SLOW_DIRLINK_TEST
			char temp_path[MAXPATHLEN + 2];

			if (DirectoryPathExpand(full_path,temp_path) != NULL)
			{
#else
			if ((_stats.st_mode & S_IFMT) == S_IFDIR)
			{
#endif
				DirEntryIsDirectoryLink(de) = TRUE;
			}

		}
	}
	    else					/* Not Symbolic Link */
	{
		_stats = _lstats;
	}

	FileInfoOrigMode(DirEntrySelfInfo(de)) = _lstats.st_mode;
	FileInfoProt(DirEntrySelfInfo(de)) = _lstats.st_mode & 0777;
	FileInfoUserID(DirEntrySelfInfo(de)) = _lstats.st_uid;
	FileInfoGroupID(DirEntrySelfInfo(de)) = _lstats.st_gid;
	FileInfoFileSize(DirEntrySelfInfo(de)) = _lstats.st_size;
	FileInfoLastAccess(DirEntrySelfInfo(de)) = _lstats.st_atime;
	FileInfoLastModify(DirEntrySelfInfo(de)) = _lstats.st_mtime;
	FileInfoLastStatusChange(DirEntrySelfInfo(de)) = _lstats.st_ctime;

	FileInfoOrigMode(DirEntryActualInfo(de)) = _stats.st_mode;
	FileInfoProt(DirEntryActualInfo(de)) = _stats.st_mode & 0777;
	FileInfoUserID(DirEntryActualInfo(de)) = _stats.st_uid;
	FileInfoGroupID(DirEntryActualInfo(de)) = _stats.st_gid;
	FileInfoFileSize(DirEntryActualInfo(de)) = _stats.st_size;
	FileInfoLastAccess(DirEntryActualInfo(de)) = _stats.st_atime;
	FileInfoLastModify(DirEntryActualInfo(de)) = _stats.st_mtime;
	FileInfoLastStatusChange(DirEntryActualInfo(de)) = _stats.st_ctime;

	return(TRUE);
} /* End DirectoryReadNextEntry */


char *DirectoryPathExpand(char *old_path, char *new_path)
{
	register char *p;
	char path[MAXPATHLEN + 2];

	if (getwd(path) == NULL) return(NULL);
	if (chdir(old_path) != 0) return(NULL);
	if (getwd(new_path) == NULL) strcpy(new_path,old_path);
	if (chdir(path) != 0) return(NULL);
	for (p = new_path; *p != '\0'; p++);
	if ((p != new_path) && *(p - 1) != '/')
	{
		*p++ = '/';
		*p = '\0';
	}
	return(new_path);
} /* End DirectoryPathExpand */


/*---------------------------------------------------------------------------*

             D I R E C T O R Y    E N T R Y    R O U T I N E S

 *---------------------------------------------------------------------------*/

void DirEntryDump(FILE *fp, DirEntry *de)
{
	fprintf(fp,"%20s, Size %7ld, Prot %3o\n",
		DirEntryFileName(de),DirEntryFileSize(de),DirEntryProt(de));
} /* End DirEntryDump */
