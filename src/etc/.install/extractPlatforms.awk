
# $Id$

# Author: Svein Boe, Ifi, UiO

BEGIN {
  n = -1
}

/^<platform> is one of/, /^$/ {
    if ($0 !~ /^<platform>/ && length > 0) {
	n++;
	plForm[n] = $2;
	desc[n] = $0;
	lines[n] = "echo '" $0 "'";
    }
  }
END {
    if (testScript + descrip + plat == 0) {
	print "#!/bin/sh\n";
	print "# $" "Id$";
	print "# First parameter non-empty if output should go to stderr.\n";
	print "if test \"$#\" -lt 1";
	print "then";
	print "    dest=\"\"";
	print "else";
	print "    dest=\"1>&2\"";
	print "fi\n";

	for (i=0; i<=n; i++) {
	    print "eval" " \"" lines[i] "\" $dest";
	}
	print "\nexit 0";
    } else if (descrip + plat == 0) {
	print "#!/bin/sh\n";
	print "# $" "Id$";
	print "# Takes one argument, the suggested platform name.\n"
	print "XITE_HOSTTYPE=`expr \"$1\" : '[ ]*\\\(.*\\\)'`\n";
	ORS=""
	print "if test \"$XITE_HOSTTYPE\" != \"" plForm[0] "\"";

	for (i = 1; i <= n; i++) {
	    print " \\\n";
	    print "     -a \"$XITE_HOSTTYPE\" != \"" plForm[i] "\"";
	}
	print "\n";
	ORS="\n";
	print "then";
	print "    exit 1";
	print "else";
	print "    exit 0";
	print "fi"
    } else if (plat == 0) {
        for (i = 0; i <= n; i++) {
	    print desc[i];
	}
    } else {
        ORS="";
        print plForm[0];
	for (i = 1; i <= n; i++) {
            print " " plForm[i];
	}
    }
  }
