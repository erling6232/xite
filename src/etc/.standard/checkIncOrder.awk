
# $Id$
# Author: Svein Bøe, 1994.
# Purpose: To be called from script checkIncOrder.

BEGIN {
  fi=0;      # Not found '#include <xite/includes.h>'
  fxite=0;   # Not found other '#include <xite/*.h>'
  fXITE=0;   # Not found '#include XITE_'
  fX11=0;    # Not found '#include <X11/*>'
  fEndian=0;
  fFcntl=0;
  fFile=0;
  fFork=0;
  fIo=0;
  fLimits=0;
  fMalloc=0;
  fMemory=0;
  fMktemp=0;
  fRandom=0;
  fStat=0;
  fStdarg=0;
  fStdio=0;
  fString=0;
  fStrtol=0;
  fTime=0;
  fToupper=0;
  fTypes=0;
  fUnistd=0;

}

/^#include *<xite\/includes\.h> */ {
   fi = 1;  # Found '#include <xite/includes.h>'
   # Only print if not already printed
   # fXITE == 1 || fxite == 1 || fX11: Printed already.
   if (fXITE != 1 && fxite != 1 && fX11 != 1) {
       if (dont == 0) {print;}
   }
   next;
}

/^#include *<xite\/.+\.h> */ {
   fxite = 1; # Found '#include <xite/*.h>'

   # Check to see whether <xite/includes.h> must be included first.

   if (fi != 1) {
       # Either <xite/includes.h> exists or must be added.
       fi = 1;  # <xite/includes.h> not found, but not to be printed again.
       if (dont == 0) {print "#include <xite/includes.h>";}
   }

   if (dont == 0) {print;}
   next;
}

/^#include *<X11\/.+> */ {
   fX11 = 1; # Found '#include <X11/*>'

   # Check to see whether <xite/includes.h> must be included first.

   if (fi != 1) {
       # Either <xite/includes.h> exists or must be added.
       fi = 1;  # <xite/includes.h> not found, but not to be printed again.
       if (dont == 0) {print "#include <xite/includes.h>";}
   }

   if (dont == 0) {print;}
   next;
}

/^#include *XITE_/ {
   fXITE = 1;  # Found '#include XITE_'

   # Check to see whether <xite/includes.h> must be included first.

   if (fi != 1) {
       # <xite/includes.h> must be added.
       fi = 1;  # <xite/includes.h> not found, but not to be printed again.
       if (dont == 0) {print "#include <xite/includes.h>";}
   }


   # Only output include directive if not already encountered.

   if ($0 ~ /^#include *XITE_ENDIAN_H */) {
       if (fEndian != 1) {
	   fEndian=1;
	   if (dont == 0) print;
       }
   }
   if ($0 ~ /^#include *XITE_FCNTL_H */) {
       if (fFcntl != 1) {
	   fFcntl=1;
	   if (dont == 0) print;
       }
   }
   if ($0 ~ /^#include *XITE_FILE_H */) {
       if (fFile != 1) {
	   fFile=1;
	   if (dont == 0) print;
       }
   }
   if ($0 ~ /^#include *XITE_FORK_H */) {
       if (fFork != 1) {
	   fFork=1;
	   if (dont == 0) print;
       }
   }
   if ($0 ~ /^#include *XITE_IO_H */) {
       if (fIo != 1) {
	   fIo=1;
	   if (dont == 0) print;
       }
   }
   if ($0 ~ /^#include *XITE_LIMITS_H */) {
       if (fLimits != 1) {
	   fLimits=1;
	   if (dont == 0) print;
       }
   }
   if ($0 ~ /^#include *XITE_MALLOC_H */) {
       if (fMalloc != 1) {
	   fMalloc=1;  # Not written already
	   if (dont == 0) print;
       }
   }
   if ($0 ~ /^#include *XITE_MEMORY_H */) {
       if (fMemory != 1) {
	   fMemory=1;  # Not written already
	   if (dont == 0) print;
       }
   }
   if ($0 ~ /^#include *XITE_MKTEMP_H */) {
       if (fMktemp != 1) {
	   fMktemp=1;  # Not written already
	   if (dont == 0) print;
       }
   }
   if ($0 ~ /^#include *XITE_RANDOM_H */) {
       if (fRandom != 1) {
	   fRandom=1;  # Not written already
	   if (dont == 0) print;
       }
   }
   if ($0 ~ /^#include *XITE_STAT_H */) {
       if (fStat != 1) {
	   fStat=1;
	   if (dont == 0) print;
       }
   }
   if ($0 ~ /^#include *XITE_STDARG_H */) {
       if (fStdarg != 1) {
	   fStdarg=1;  # Not written already
	   if (dont == 0) print;
       }
   }
   if ($0 ~ /^#include *XITE_STDIO_H */) {
       if (fStdio != 1) {
	   fStdio=1;
	   if (dont == 0) print;
       }
   }
   if ($0 ~ /^#include *XITE_STRING_H */) {
       if (fString != 1) {
	   fString=1;
	   if (dont == 0) print;
       }
   }
   if ($0 ~ /^#include *XITE_STRTOL_H */) {
       if (fStrtol != 1) {
	   fStrtol=1;
	   if (dont == 0) print;
       }
   }
   if ($0 ~ /^#include *XITE_TIME_H */) {
       if (fTime != 1) {
	   fTime=1;
	   if (dont == 0) print;
       }
   }
   if ($0 ~ /^#include *XITE_TOUPPER_H */) {
       if (fToupper != 1) {
	   fToupper=1;
	   if (dont == 0) print;
       }
   }
   if ($0 ~ /^#include *XITE_TYPES_H */) {
       if (fTypes != 1) {
	   fTypes=1;
	   if (dont == 0) print;
       }
   }
   if ($0 ~ /^#include *XITE_UNISTD_H */) {
       if (fUnistd != 1) {
	   fUnistd=1;  # Not written already
	   if (dont == 0) print;
       }
   }
   next;
}
{if (dont == 0) print;}

END {
    if (dont == 1) {
        print "Include directives to be printed (1 for yes, 0 for no):" | \
		"cat - 1>&2"
	print "<xite/includes.h>=" fi ", <xite/*.h>=" fxite \
		", XITE_*=" fXITE | "cat - 1>&2"
	print "XITE_FCNTL_H=" fFcntl ", XITE_FILE_H=" fFile \
	        ", XITE_FORK_H=" fFork | "cat - 1>&2"
	print "XITE_IO_H=" fIo ", XITE_MALLOC_H=" fMalloc \
	        ", XITE_MKTEMP_H=" fMktemp | "cat - 1>&2"
        print "XITE_RANDOM_H=" fRandom ", XITE_STAT_H=" fStat \
	        ", XITE_STDARG_H=" fStdarg | "cat - 1>&2"
	print "XITE_STDIO_H=" fStdio ", XITE_STRING_H=" fString \
	        ", XITE_STRTOL_H=" fStrtol | "cat - 1>&2"
	print "XITE_TIME_H=" fTime ", XITE_TOUPPER_H=" fToupper \
	        ", XITE_TYPES_H=" fTypes | "cat - 1>&2"
        print "XITE_UNISTD_H=" fUnistd ", XITE_MEMORY_H=" fMemory \
	        ", XITE_LIMITS_H=" fLimits | "cat - 1>&2"
        print "XITE_ENDIAN_H=" fEndian | "cat - 1>&2"
    }
}

