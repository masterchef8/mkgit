/*
 * This software is based on mkdir.c
 * 
 */

#ifndef lint
static char const copyright[] =
"@(#) Copyright (c) 1983, 1992, 1993\n\
The Regents of the University of California.  All rights reserved.\n";
#endif /* not lint */

#ifndef lint
#if 0
static char sccsid[] = "@(#)mkdir.c	8.2 (Berkeley) 1/25/94";
#endif
#endif /* not lint */
#include <sys/cdefs.h>
__RCSID("$FreeBSD: src/bin/mkdir/mkdir.c,v 1.26 2002/06/30 05:13:54 obrien Exp $");

#include <sys/types.h>
#include <sys/stat.h>

#include <err.h>
#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>

static int	build(char *, mode_t);
static void	usage(void);

int vflag;

int
main(int argc, char *argv[])
{
    int ch, exitval, success, pflag;
    mode_t omode, *set = (mode_t *)NULL;
    char *mode;
    
    omode = pflag = 0;
    mode = NULL;
    while ((ch = getopt(argc, argv, "m:pv")) != -1)
        switch(ch) {
            case 'm':
                mode = optarg;
                break;
            case 'p':
                pflag = 1;
                break;
            case 'v':
                vflag = 1;
                break;
            case '?':
            default:
                usage();
        }
    
    argc -= optind;
    argv += optind;
    if (argv[0] == NULL)
        usage();
    
    if (mode == NULL) {
        omode = S_IRWXU | S_IRWXG | S_IRWXO;
    } else {
        if ((set = setmode(mode)) == NULL)
            errx(1, "invalid file mode: %s", mode);
        omode = getmode(set, S_IRWXU | S_IRWXG | S_IRWXO);
        free(set);
    }
    
    for (exitval = 0; *argv != NULL; ++argv) {
        success = 1;
        if (pflag) {
            if (build(*argv, omode))
                success = 0;
        } else if (mkdir(*argv, omode) < 0) {
            if (errno == ENOTDIR || errno == ENOENT)
                warn("%s", dirname(*argv));
            else
                warn("%s", *argv);
            success = 0;
        } else if (vflag)
            (void)printf("mkdir: created directory '%s'\n", *argv);
        
        if (!success)
            exitval = 1;
        /*
         * The mkdir() and umask() calls both honor only the low
         * nine bits, so if you try to set a mode including the
         * sticky, setuid, setgid bits you lose them.  Don't do
         * this unless the user has specifically requested a mode,
         * as chmod will (obviously) ignore the umask.
         */
        if (success && mode != NULL && chmod(*argv, omode) == -1) {
            warn("%s", *argv);
            exitval = 1;
        }
    }
    exit(exitval);
}

int
build(char *path, mode_t omode)
{
    struct stat sb;
    mode_t numask, oumask;
    int first, last, retval;
    char *p;
    
    p = path;
    oumask = 0;
    retval = 0;
    if (p[0] == '/')		/* Skip leading '/'. */
        ++p;
    for (first = 1, last = 0; !last ; ++p) {
        if (p[0] == '\0')
            last = 1;
        else if (p[0] != '/')
            continue;
        *p = '\0';
        if (p[1] == '\0')
            last = 1;
        if (first) {
            /*
             * POSIX 1003.2:
             * For each dir operand that does not name an existing
             * directory, effects equivalent to those cased by the
             * following command shall occcur:
             *
             * mkdir -p -m $(umask -S),u+wx $(dirname dir) &&
             *    mkdir [-m mode] dir
             *
             * We change the user's umask and then restore it,
             * instead of doing chmod's.
             */
            oumask = umask(0);
            numask = oumask & ~(S_IWUSR | S_IXUSR);
            (void)umask(numask);
            first = 0;
        }
        if (last)
            (void)umask(oumask);
        if (mkdir(path, last ? omode : S_IRWXU | S_IRWXG | S_IRWXO) < 0) {
            if (errno == EEXIST || errno == EISDIR) {
                if (stat(path, &sb) < 0) {
                    warn("%s", path);
                    retval = 1;
                    break;
                } else if (!S_ISDIR(sb.st_mode)) {
                    if (last)
                        errno = EEXIST;
                    else
                        errno = ENOTDIR;
                    warn("%s", path);
                    retval = 1;
                    break;
                }
            } else {
                warn("%s", path);
                retval = 1;
                break;
            }
        } else if (vflag)
            printf("%s\n", path);
        if (!last)
            *p = '/';
    }
    if (!first && !last)
        (void)umask(oumask);
    return (retval);
}

void
usage(void)
{
    
    (void)fprintf(stderr, "usage: mkdir [-pv] [-m mode] directory ...\n");
    exit (EX_USAGE);
}