/* $Id: plplotP.h,v 1.1.1.1 2003-12-28 21:06:17 wyo Exp $
 * $Log: not supported by cvs2svn $
 * Revision 1.1.1.1  1999/09/01 14:01:41  fhuebner
 * Start of wxPlot
 *
 * Revision 1.1.1.1  1999/09/01 12:55:12  fhuebner
 * Start of wxPlot
 *
 * Revision 1.41  1995/10/13  21:19:09  mjl
 * Now reverts back to fseek/ftell if the USE_FSEEK macro is define, for
 * use on systems where libc is busted.
 *
 * Revision 1.40  1995/06/11  19:22:35  mjl
 * Changed caddr_t treatment to make bogus cases easier to diagnose.
 *
 * Revision 1.39  1995/06/01  21:44:36  mjl
 * Repaired some bogus comments.
 *
 * Revision 1.38  1995/05/26  20:20:12  mjl
 * Changed plstream.h -> plstrm.h everywhere.
 *
 * Revision 1.37  1995/05/07  02:28:58  mjl
 * Added include of pldebug.h so that all PLplot source files now get it.
 * Eliminated prototypes for obsolete functions plCmap0_init() and
 * plCmap1_init() and added prototype for new function plstrm_init().
 *
 * Revision 1.36  1995/04/12  08:22:30  mjl
 * Eliminated SCHAR in favor of simply "signed char".  Removed prototypes
 * for obsolete functions.
 *
 * Revision 1.35  1995/03/21  19:47:44  mjl
 * Moved inclusion of <math.h>, <string.h>, and <ctype.h> to plplotP.h, since
 * they are strictly needed by plplot.h.
 *
 * Revision 1.34  1995/03/16  23:40:54  mjl
 * Prototypes for many obsolete accessor functions removed.  Prototype for
 * new function plP_swin added.
 *
 * Revision 1.33  1995/01/13  23:24:19  mjl
 * Now drops down to pre-ANSI-C behavior as regards fpos_t under VMS since
 * the code in plmeta.c doesn't work under the VMS implementation.
 *
 * Revision 1.32  1995/01/06  07:49:29  mjl
 * Moved definition of window coordinate structure into plplot.h since it's
 * now used in the plstream definition.  Also updated prototype for pldtik
 * and added prototype for pldprec.
 *
 * Revision 1.31  1994/11/02  19:54:58  mjl
 * Changed stream variables vpx.. to vdx.. since they now hold the relative
 * device coordinates (0 to 1) rather than the physical coordinates.
 *
 * Revision 1.30  1994/10/11  18:59:03  mjl
 * Virtual coordinate space for X-based drivers and metafile increased to
 * 32K by 32K, in order to improve appearance of plots at high levels of
 * magnification.
*/

/*
    plplotP.h

    Copyright (C) 1993, 1994, 1995  by 
    Maurice J. LeBrun, Geoff Furnish, Tony Richardson.

    Internal (private) macros and prototypes for the PLplot package.  This
    header file must be included before all others, including system header
    files.  This file is typically needed when including driver specific
    header files (e.g. pltkd.h).

    This software may be freely copied, modified and redistributed without
    fee provided that this copyright notice is preserved intact on all
    copies and modified copies. 
 
    There is no warranty or other guarantee of fitness of this software.
    It is provided solely "as is". The author(s) disclaim(s) all
    responsibility and liability with respect to this software's usage or
    its effect upon hardware or computer systems. 
*/

#ifndef __PLPLOTP_H__
#define __PLPLOTP_H__

/*--------------------------------------------------------------------------*\
 *        SYSTEM-SPECIFIC SETTINGS
 * 
 * Here we enable or disable based on system specific capabilities of
 * PLPLOT.  At present there are only two different "optional"
 * capabilities.  They are:
 *
 * POSIX_TTY	Defined if POSIX.1 tty control functions are available. 
 * STDC_HEADERS	Defined if libc is ANSI-compliant.
 *
 * POSIX.1 tty control functions are used by some of the drivers, e.g. to
 * switch to character-oriented (CBREAK) i/o (notably the tek driver and
 * all its variants).  It is usable without this but not as powerful.  The
 * reason for using this is that some supported systems are still not
 * POSIX.1 compliant (and some may never be).
 *
 * ANSI libc calls are used for: (a) setting up handlers to be called
 * before program exit (via the "atexit" call), and (b) for seek
 * operations.  Again, the code is usable without these.  An ANSI libc
 * should be available, given the requirement of an ANSI compiler.  Some
 * reasons why not: (a) the vendor didn't supply a complete ANSI
 * environment, or (b) the ANSI libc calls are buggy, or (c) you ported
 * gcc to your system but not glibc (for whatever reason).  Note: without
 * an ANSI C lib, if you ^C out of a program using one of the PLplot tek
 * drivers, your terminal may be left in a strange state.
\*--------------------------------------------------------------------------*/

#include "plConfig.h"
#ifdef caddr_t
#undef caddr_t
typedef char * caddr_t;
#endif

/* System headers */

#include <ctype.h>
#include <math.h>
#include <string.h>

#ifdef _POSIX_SOURCE
#define POSIX_TTY
#endif

/* Hacks to deal with non-ANSI libc */

#if defined (STDC_HEADERS) && ! defined(USE_FSEEK)
#define STDC_FPOS_T
#endif

#ifdef STDC_FPOS_T
#define FPOS_T fpos_t
#define pl_fsetpos(a,b) fsetpos(a, b)
#define pl_fgetpos(a,b) fgetpos(a, b)

#else
#define FPOS_T long
#define pl_fsetpos(a,b) fseek(a, *b, 0)
#define pl_fgetpos(a,b) (-1L == (*b = ftell(a)))
#endif

/* Include all externally-visible definitions and prototypes */
/* plplot.h also includes some handy system header files */

#include "plplot.h"

/* plstream definition */

#include "plstrm.h"

/* If not including this file from inside of plcore.h, declare plsc */

#ifndef __PLCORE_H__
extern PLStream	*plsc;
#include "pldebug.h"
#endif

/*--------------------------------------------------------------------------*\
 *                       Utility macros
\*--------------------------------------------------------------------------*/

#ifndef TRUE
#define TRUE  1
#define FALSE 0
#endif

/* Used to help ensure everything malloc'ed gets freed */

#define free_mem(a) \
    if (a != NULL) { free((void *) a); a = NULL; }

/* Allows multi-argument setup calls to not affect selected arguments */

#define plsetvar(a, b) \
    if (b != PL_NOTSET) a = b;

/* Lots of cool math macros */

#ifndef MAX
#define MAX(a,b)    (((a) > (b)) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a,b)    (((a) < (b)) ? (a) : (b))
#endif
#ifndef ABS
#define ABS(a)      ((a)<0 ? -(a) : (a))
#endif
#ifndef ROUND
#define ROUND(a)    (PLINT)((a)<0. ? ((a)-.5) : ((a)+.5))
#endif
#ifndef BETW
#define BETW(ix,ia,ib)  (((ix)<=(ia)&&(ix)>=(ib)) || ((ix)>=(ia)&&(ix)<=(ib)))
#endif
#ifndef SSQR
#define SSQR(a,b)       sqrt((a)*(a)+(b)*(b))
#endif
#ifndef SIGN
#define SIGN(a)         ((a)<0 ? -1 : 1)
#endif

/* A coordinate value that should never occur */

#define UNDEFINED -9999999

/*--------------------------------------------------------------------------*\
 *                       PLPLOT control macros
\*--------------------------------------------------------------------------*/

/* Some constants */

#define PL_MAXPOLY	256	/* Max segments in polyline or polygon */
#define PL_NSTREAMS	100	/* Max number of concurrent streams. */
#define PL_RGB_COLOR	1<<7	/* A hack */

#define TEXT_MODE	0
#define GRAPHICS_MODE	1
#ifndef PI
#define PI		3.1415926535897932384
#endif

/* These define the metafile & X driver (virtual) coordinate systems */

#define PIXELS_X	32768		/* Number of virtual pixels in x */
#define PIXELS_Y	32768		/* Number of virtual pixels in x */
#define LPAGE_X		254.0		/* Page length in x in virtual mm */
#define LPAGE_Y		190.5		/* Page length in y in virtual mm */

/* Switches for state function call. */

#define PLSTATE_WIDTH		1	/* pen width */
#define PLSTATE_COLOR0		2	/* change to color in cmap 0 */
#define PLSTATE_COLOR1		3	/* change to color in cmap 1 */
#define PLSTATE_FILL		4	/* set area fill attribute */
#define PLSTATE_CMAP0		5	/* change to cmap 0 */
#define PLSTATE_CMAP1		6	/* change to cmap 1 */

/* Bit switches used in the driver interface */

#define PLDI_MAP	0x01
#define PLDI_ORI	0x02
#define PLDI_PLT	0x04
#define PLDI_DEV	0x08

/* Default size for family files, in KB. */

#ifndef PL_FILESIZE_KB
#define PL_FILESIZE_KB 1000
#endif

/* Font file names. */

#define PLPLOT5_FONTS

#ifdef PLPLOT5_FONTS
#define PL_XFONT	"plxtnd5.fnt"
#define PL_SFONT	"plstnd5.fnt"
#else
#define PL_XFONT	"plxtnd4.fnt"
#define PL_SFONT	"plstnd4.fnt"
#endif

/*--------------------------------------------------------------------------*\
 * The following environment variables are defined:
 * 
 *	PLPLOT_BIN      # where to find executables
 *	PLPLOT_LIB      # where to find library files (fonts, maps, etc)
 *	PLPLOT_TCL      # where to find tcl scripts
 *
 *	PLPLOT_HOME     # basename of plplot hierarchy
 *
 * search order:
 *	1)	the most specific possible locators, one of
 *			$(PLPLOT_BIN)
 *			$(PLPLOT_LIB)
 *			$(PLPLOT_TCL)
 *		as appropriate
 *
 *	2)	the current directory
 *
 *	3)	one of  $(PLPLOT_HOME)/bin
 *			$(PLPLOT_HOME)/lib
 *			$(PLPLOT_HOME)/tcl
 *		as appropriate
 *
 *	4)	as appropriate, the compile-time (Makefile)
 *		BIN_DIR, LIB_DIR, TCL_DIR
 *
 *  8 Jun 1994  mj olesen (olesen@weber.me.queensu.ca)
 *
 * Other notes:
 *
 * In addition to the directories above, the following are also used:
 *
 * Lib file search path: PLLIBDEV (see plctrl.c).  This is checked last,
 * and is a system-dependent hardwired location.
 *
 * Tcl search path: $HOME/tcl is searched before the install location,
 * TCL_DIR.
\*--------------------------------------------------------------------------*/

#define PLPLOT_BIN_ENV          "PLPLOT_BIN"
#define PLPLOT_LIB_ENV          "PLPLOT_LIB"
#define PLPLOT_TCL_ENV          "PLPLOT_TCL"
#define PLPLOT_HOME_ENV         "PLPLOT_HOME"

/*--------------------------------------------------------------------------*\
 *		Function Prototypes
 *
 * These typically should not be called directly by the user.
\*--------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/* Determines interval between numeric labels */

void
pldtik(PLFLT vmin, PLFLT vmax, PLFLT *tick, PLINT *nsubt);

/* Determines precision of box labels */

void
pldprec(PLFLT vmin, PLFLT vmax, PLFLT tick, PLINT lf, 
	PLINT *mode, PLINT *prec, PLINT digmax, PLINT *scale);

/* Draws a polyline within the clip limits. */

void
plP_pllclp(PLINT *x, PLINT *y, PLINT npts,
	   PLINT xmin, PLINT xmax, PLINT ymin, PLINT ymax, 
	   void (*draw) (short *, short *, PLINT));

/* Fills a polygon within the clip limits. */

void
plP_plfclp(PLINT *x, PLINT *y, PLINT npts,
	   PLINT xmin, PLINT xmax, PLINT ymin, PLINT ymax, 
	   void (*draw) (short *, short *, PLINT));

/* Pattern fills in software the polygon bounded by the input points. */

void
plfill_soft(short *x, short *y, PLINT npts);

/* In case of an abort this routine is called.  It just prints out an */
/* error message and tries to clean up as much as possible. */

void
plexit(char *errormsg);

/* Just a front-end to exit().  */

void
pl_exit(void);

/* A handy way to issue warnings, if need be. */

void
plwarn(char *errormsg);

/* Same as plwarn(), but appends ", aborting plot" to the error message */

void
plabort(char *errormsg);

/* Loads either the standard or extended font. */

void
plfntld(PLINT fnt);

/* Release memory for fonts. */

void
plfontrel(void);

/* Bin up cmap 1 space and assign colors to make inverse mapping easy. */

void
plcmap1_calc(void);

/* Draws a slanting tick at position (mx,my) (measured in mm) of */
/* vector length (dx,dy). */

void 
plstik(PLFLT mx, PLFLT my, PLFLT dx, PLFLT dy);

/* Prints out a "string" at reference position with physical coordinates */
/* (refx,refy). */

void
plstr(PLINT base, PLFLT *xform, PLINT refx, PLINT refy, const char *string);

/* Draws a tick parallel to x. */

void
plxtik(PLINT x, PLINT y, PLINT below, PLINT above);

/* Draws a tick parallel to y. */

void
plytik(PLINT x, PLINT y, PLINT left, PLINT right);

/* Get x-y domain in world coordinates for 3d plots */

void
plP_gdom(PLFLT *p_xmin, PLFLT *p_xmax, PLFLT *p_ymin, PLFLT *p_ymax);

/* Get vertical (z) scale parameters for 3-d plot */

void
plP_grange(PLFLT *p_zscl, PLFLT *p_zmin, PLFLT *p_zmax);

/* Get parameters used in 3d plots */

void
plP_gw3wc(PLFLT *p_dxx, PLFLT *p_dxy, PLFLT *p_dyx, PLFLT *p_dyy,
	  PLFLT *p_dyz);

/* Get clip boundaries in physical coordinates */

void
plP_gclp(PLINT *p_ixmin, PLINT *p_ixmax, PLINT *p_iymin, PLINT *p_iymax);

/* Set clip boundaries in physical coordinates */

void
plP_sclp(PLINT ixmin, PLINT ixmax, PLINT iymin, PLINT iymax);

/* Get physical device limits in physical coordinates */

void
plP_gphy(PLINT *p_ixmin, PLINT *p_ixmax, PLINT *p_iymin, PLINT *p_iymax);

/* Get number of subpages on physical device and current subpage */

void
plP_gsub(PLINT *p_nx, PLINT *p_ny, PLINT *p_cs);

/* Set number of subpages on physical device and current subpage */

void
plP_ssub(PLINT nx, PLINT ny, PLINT cs);

/* Set up plot parameters according to the number of subpages. */

void
plP_subpInit(void);

/* Get viewport boundaries in normalized device coordinates */

void
plP_gvpd(PLFLT *p_xmin, PLFLT *p_xmax, PLFLT *p_ymin, PLFLT *p_ymax);

/* Get viewport boundaries in world coordinates */

void
plP_gvpw(PLFLT *p_xmin, PLFLT *p_xmax, PLFLT *p_ymin, PLFLT *p_ymax);

/* Get number of pixels to a millimeter */

void
plP_gpixmm(PLFLT *p_x, PLFLT *p_y);

/* All the drivers call this to set physical pixels/mm. */

void
plP_setpxl(PLFLT xpmm0, PLFLT ypmm0);

/* Get background parameters for 3d plot. */

void
plP_gzback(PLINT **zbf, PLINT **zbc, PLFLT **zbt);

/* Move to physical coordinates (x,y). */

void
plP_movphy(PLINT x, PLINT y);

/* Draw to physical coordinates (x,y). */

void
plP_draphy(PLINT x, PLINT y);

/* Move to world coordinates (x,y). */

void
plP_movwor(PLFLT x, PLFLT y);

/* Draw to world coordinates (x,y). */

void
plP_drawor(PLFLT x, PLFLT y);

/* Draw polyline in physical coordinates. */

void
plP_draphy_poly(PLINT *x, PLINT *y, PLINT n);

/* Draw polyline in world coordinates. */

void
plP_drawor_poly(PLFLT *x, PLFLT *y, PLINT n);

/* Sets up physical limits of plotting device. */

void
plP_setphy(PLINT xmin, PLINT xmax, PLINT ymin, PLINT ymax);

/* Set up the subpage boundaries according to the current subpage selected */

void
plP_setsub(void);

/* Get the floating point precision (in number of places) in numeric labels. */

void
plP_gprec(PLINT *p_setp, PLINT *p_prec);

/* Computes the length of a string in mm, including escape sequences. */

PLFLT
plstrl(const char *string);

/* Similar to strpos, but searches for occurence of string str2. */

PLINT
plP_stindex(const char *str1, const char *str2);

/* Searches string str for first occurence of character chr.  */

PLINT
plP_strpos(char *str, int chr);

/* Searches string str for character chr (case insensitive). */

PLINT
plP_stsearch(const char *str, int chr);

	/* Conversion functions */

/* device coords to physical coords (x) */

PLINT
plP_dcpcx(PLFLT x);

/* device coords to physical coords (y) */

PLINT
plP_dcpcy(PLFLT y);

/* millimeters from bottom left-hand corner to physical coords (x) */

PLINT
plP_mmpcx(PLFLT x);

/* millimeters from bottom left-hand corner to physical coords (y) */

PLINT
plP_mmpcy(PLFLT y);

/* world coords to physical coords (x) */

PLINT
plP_wcpcx(PLFLT x);

/* world coords to physical coords (y) */

PLINT
plP_wcpcy(PLFLT y);

/* physical coords to device coords (x) */

PLFLT
plP_pcdcx(PLINT x);

/* physical coords to device coords (y) */

PLFLT
plP_pcdcy(PLINT y);

/* millimeters from bottom left corner to device coords (x) */

PLFLT
plP_mmdcx(PLFLT x);

/* millimeters from bottom left corner to device coords (y) */

PLFLT
plP_mmdcy(PLFLT y);

/* subpage coords to device coords (x) */

PLFLT
plP_scdcx(PLFLT x);

/* subpage coords to device coords (y) */

PLFLT
plP_scdcy(PLFLT y);

/* device coords to millimeters from bottom left-hand corner (x) */

PLFLT
plP_dcmmx(PLFLT x);

/* device coords to millimeters from bottom left-hand corner (y) */

PLFLT
plP_dcmmy(PLFLT y);

/* world coords into millimeters (x) */

PLFLT
plP_wcmmx(PLFLT x);

/* world coords into millimeters (y) */

PLFLT
plP_wcmmy(PLFLT y);

/* device coords to subpage coords (x) */

PLFLT
plP_dcscx(PLFLT x);

/* device coords to subpage coords (y) */

PLFLT
plP_dcscy(PLFLT y);

/* 3-d coords to 2-d projection (x) */

PLFLT
plP_w3wcx(PLFLT x, PLFLT y, PLFLT z);

/* 3-d coords to 2-d projection (y) */

PLFLT
plP_w3wcy(PLFLT x, PLFLT y, PLFLT z);

	/* Driver calls */

/* Initialize device. */

void
plP_init(void);

/* Draw line between two points */

void
plP_line(short *x, short *y);

/* Draw polyline */

void
plP_polyline(short *x, short *y, PLINT npts);

/* Fill polygon */

void
plP_fill(short *x, short *y, PLINT npts);

/* End of page */

void
plP_eop(void);

/* End of page */

void
plP_bop(void);

/* Tidy up device (flush buffers, close file, etc.) */

void
plP_tidy(void);

/* Change state. */

void
plP_state(PLINT op);

/* Escape function, for driver-specific commands. */

void
plP_esc(PLINT op, void *ptr);

/* Set up plot window parameters. */

void
plP_swin(PLWindow *plwin);

/* Return file pointer to lib file. */

FILE *
plLibOpen(char *fn);

/* Does required startup initialization of a stream.  */

void
plstrm_init(void);

#ifdef __cplusplus
}
#endif

#endif	/* __PLPLOTP_H__ */
