/* $Id: plxwd.h,v 1.1.1.1 2003-12-28 21:07:39 wyo Exp $
 * $Log: not supported by cvs2svn $
 * Revision 1.1.1.1  1999/09/01 14:01:45  fhuebner
 * Start of wxPlot
 *
 * Revision 1.2  1996/10/31  05:05:08  furnish
 * Add new member for tracking whether the chosen visual supports mutable
 * color cells.
 *
 * Revision 1.1  1995/06/01  21:45:26  mjl
 * Created to hold definitions and prototypes specific to the X-driver.  The
 * user shouldn't need to access this file but it's here just in case.
 *
 *
 * Old entries from plplotX.h:
 *
 * Revision 1.17  1995/05/26  20:20:13  mjl
 * Changed plstream.h -> plstrm.h everywhere.
 *
 * Revision 1.16  1995/04/12  21:10:26  mjl
 * Made the ordinary graphics context and the current color a device-dependent
 * quantity rather than a display-dependent one, to fix problems with color
 * selection when plotting to separate streams/xwindows.  Thanks to Radey
 * Shouman for pointing it out.
 *
 * Revision 1.15  1995/03/16  23:43:46  mjl
 * Old cursorX, cursorY variables replaced with a PLGraphicsIn structure.
 * Also variables added for dealing with graphic crosshairs.
 *
 * Revision 1.14  1994/11/02  19:55:50  mjl
 * Added cursorX, cursorY for holding coordinates at mouse click.
 *
 * Revision 1.13  1994/10/11  18:59:57  mjl
 * Added prototypes for plD_open_xw() and plX_setBGFG().
*/

/*	plxwd.h

	Holds system header includes, prototypes of xwin driver
	utility functions, and definition of the state structure.
*/

#ifndef __PLXWD_H__
#define __PLXWD_H__

#include "plplot.h"
#include "plstrm.h"

/* System headers */

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <X11/keysym.h>

/* Specify max number of displays in use */

#define PLXDISPLAYS 100

/* One of these holds the display info, shared by all streams on a given */
/* display */

typedef struct {
    int		nstreams;		/* Number of streams using display */
    int		ixwd;			/* Specifies xwDisplay number */
    char	*displayName;		/* Name of X display */
    int		screen;			/* X screen */
    Display	*display;		/* X display */
    Visual	*visual;		/* X Visual */
    GC		gcXor;			/* Graphics context for XOR draws */
    Colormap	map;			/* Colormap */
    unsigned	depth;			/* display depth */
    int		color;			/* Set to 1 if a color output device */
    int		ncol0;			/* Number of cmap 0 colors allocated */
    int		ncol1;			/* Number of cmap 1 colors allocated */
    XColor	cmap0[16];		/* Color entries for cmap 0 */
    XColor	cmap1[256];		/* Color entries for cmap 1 */
    XColor	fgcolor;		/* Foreground color (if grayscale) */
    Cursor	xhair_cursor;		/* Crosshair cursor */
    int		rw_cmap;		/* Can we allocate r/w color cells? */
} XwDisplay;

/* One of these holds the X driver state information */

typedef struct {
    XwDisplay	*xwd;			/* Pointer to display info */

    int		is_main;		/* Set if the toplevel X window */
    Window	window;			/* X window id */
    Pixmap	pixmap;			/* Off-screen pixmap */
    GC		gc;			/* Graphics context */
    XColor	curcolor;		/* Current pen color */

    long	event_mask;		/* Event mask */
    int		exit_eventloop;		/* Breaks the event loop when set */
    long	init_width;		/* Initial window width */
    long	init_height;		/* Initial window height */

    unsigned	width, height, border;	/* Current window dimensions */

    double	xscale_init;		/* initial pixels/lx (virt. coords) */
    double	yscale_init;		/* initial pixels/ly (virt. coords) */
    double	xscale;			/* as above, but current value */
    double	yscale;			/*  (after possible resizing)  */

    short	xlen, ylen;		/* Lengths of device coord space */

    int		write_to_window;	/* Set if plotting direct to window */
    int		write_to_pixmap;	/* Set if plotting to pixmap */

    int		instr;			/* Instruction timer */
    int		max_instr;		/* Limit before X server is queried */

    PLGraphicsIn gin;			/* Graphics input structure */

    int		locate_mode;		/* Set while in locate mode */
    int		drawing_xhairs;		/* Set during xhair draws */
    XPoint	xhair_x[2], xhair_y[2];	/* Crosshair lines */

    void (*MasterEH) (PLStream *, XEvent *);	/* Master X event handler */
} XwDev;

/*--------------------------------------------------------------------------*\
 *		Function Prototypes
\*--------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/* Performs basic driver initialization. */

void
plD_open_xw(PLStream *pls);

/* Copies the supplied PLColor to an XColor */

void
PLColor_to_XColor(PLColor *plcolor, XColor *xcolor);

/* Copies the supplied XColor to a PLColor */

void
PLColor_from_XColor(PLColor *plcolor, XColor *xcolor);

/* Determines if we're using a monochrome or grayscale device */

int
pl_AreWeGrayscale(Display *display);

/* Set background & foreground colors.  */

void
plX_setBGFG(PLStream *pls);

/* Saves RGB components of given colormap */

void
PLX_save_colormap(Display *display, Colormap map);

#ifdef __cplusplus
}
#endif

#endif	/* __PLXWD_H__ */
