/* $Id: hpgl.c,v 1.1.1.1 2003-12-28 21:07:16 wyo Exp $
 * $Log: not supported by cvs2svn $
 * Revision 1.1.1.1  1999/09/01 14:01:44  fhuebner
 * Start of wxPlot
 *
 * Revision 1.6  1995/03/11  20:27:07  mjl
 * All drivers: eliminated unnecessary variable initializations, other cleaning
 * up.
 *
 * Revision 1.5	 1995/01/06  07:40:28  mjl
 * All drivers: pls->width now more sensibly handled.  If the driver supports
 * multiple widths, it first checks to see if it has been initialized
 * already (e.g. from the command line) before initializing it.	 For drivers
 * that don't support multiple widths, pls->width is ignored.
 *
 *  File:	hpgl.c
 *
 *  Descript:	hp7470, hp7580, and lj_hpgl drivers
 *
 *  Library:	---
 *
 *  Requires:	---
 *
 *  Public:	plD_init_hp7470()
 *		plD_init_hp7580()
 *		plD_init_lj_hpgl()
 *		plD_line_hpgl()
 *		plD_polyline_hpgl()
 *		plD_eop_hpgl()
 *		plD_bop_hpgl()
 *		plD_tidy_hpgl()
 *		plD_state_hpgl()
 *		plD_esc_hpgl()
 *
 *		pldummy_hpgl()
 *
 *  Private:	initialize_hpgl_pls()
 *
 *  Notes:	---
 *
\*--------------------------------------------------------------------------*/

#include "plDevs.h"

#if defined(PLD_hp7470) || defined(PLD_hp7580) || defined(PLD_lj_hpgl)

#include "plplotP.h"
#include <stdio.h>
#include <string.h>
#include "drivers.h"

/* top level declarations */

/* Plotter sizes */

#define HP7470_XMIN	  0
#define HP7470_XMAX   10299
#define HP7470_YMIN	  0
#define HP7470_YMAX    7649

#define HP7580_XMIN  -4500
#define HP7580_XMAX   4500
#define HP7580_YMIN  -2790
#define HP7580_YMAX   2790

#define LJIII_XMIN	0
#define LJIII_XMAX  11000
#define LJIII_YMIN    500
#define LJIII_YMAX   7700

#define OF		pls->OutFile
#define MIN_WIDTH	1		/* Minimum pen width */
#define MAX_WIDTH	10		/* Maximum pen width */
#define DEF_WIDTH	1		/* Default pen width */

/*--------------------------------------------------------------------------*\
 * initialize_hpgl_pls()
 *
 * Initialize plot stream
\*--------------------------------------------------------------------------*/

static void
initialize_hpgl_pls(PLStream *pls)
{
    PLDev *dev = (PLDev *) pls->dev;

    if (pls->width == 0)	/* Is 0 if uninitialized */
	pls->width = 1;

    plFamInit(pls);		/* Initialize family file info */
    plOpenFile(pls);		/* get file name if not already set */

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;
    dev->xlen = dev->xmax - dev->xmin;
    dev->ylen = dev->ymax - dev->ymin;

    plP_setpxl((PLFLT) 40., (PLFLT) 40.);
    plP_setphy(dev->xmin, dev->xmax, dev->ymin, dev->ymax);
}

/*--------------------------------------------------------------------------*\
 * plD_init_hp7470()
 *
 * Initialize device.
\*--------------------------------------------------------------------------*/

#ifdef PLD_hp7470
void
plD_init_hp7470(PLStream *pls)
{
    PLDev *dev;

    pls->color = 1;
    dev = plAllocDev(pls);	/* Allocate device-specific data */
    dev->xmin = HP7470_XMIN;
    dev->xmax = HP7470_XMAX;
    dev->ymin = HP7470_YMIN;
    dev->ymax = HP7470_YMAX;

    initialize_hpgl_pls(pls);	/* initialize plot stream */

    fputs( "\x1b.I200;;17:\x1b.N;19:\x1b.M;;;10:IN;\n", OF );
}
#endif		/* PLD_hp7470 */

/*--------------------------------------------------------------------------*\
 * plD_init_hp7580()
 *
 * Initialize device.
\*--------------------------------------------------------------------------*/

#ifdef PLD_hp7580
void
plD_init_hp7580(PLStream *pls)
{
    PLDev *dev;

    pls->color = 1;
    dev = plAllocDev(pls);	/* Allocate device-specific data */
    dev->xmin = HP7580_XMIN;
    dev->xmax = HP7580_XMAX;
    dev->ymin = HP7580_YMIN;
    dev->ymax = HP7580_YMAX;

    initialize_hpgl_pls(pls);	/* initialize plot stream */

    fputs( "\x1b.I200;;17:\x1b.N;19:\x1b.M;;;10:IN;\n", OF );
    fputs( "RO90;IP;SP4;PA;\n", OF );
}
#endif	/* PLD_hp7580 */

/*--------------------------------------------------------------------------*\
 * plD_init_lj_hpgl()
 *
 * Initialize device.
\*--------------------------------------------------------------------------*/

#ifdef PLD_lj_hpgl
void
plD_init_lj_hpgl(PLStream *pls)
{
    PLDev *dev;

    dev = plAllocDev(pls);	/* Allocate device-specific data */
    dev->xmin = LJIII_XMIN;
    dev->xmax = LJIII_XMAX;
    dev->ymin = LJIII_YMIN;
    dev->ymax = LJIII_YMAX;

    initialize_hpgl_pls(pls);	/* initialize plot stream */

/* HP III changes here up to .I200 puts printer in HPGL/2 emulation
 * with 300DPI printing.
 * Next line : added pw 0.2 for pen width 0.2 (of an inch ?)
*/
    fputs("\x1b*T300R\x1b%1B;\x1b.I200;;17:\x1b.N;19:\x1b.M;;;10:IN;\n", OF);
    fputs("RO90;IP;PW 0.2;SP 1;PA;", OF);
}
#endif	/* PLD_lj_hpgl */

/*--------------------------------------------------------------------------*\
 * plD_line_hpgl()
 *
 * Draw a line in the current color from (x1,y1) to (x2,y2).
\*--------------------------------------------------------------------------*/

void
plD_line_hpgl(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
    PLDev *dev = (PLDev *) pls->dev;
    int x1 = x1a, y1 = y1a, x2 = x2a, y2 = y2a;

/* Write out old path */

    if (x1 != dev->xold || y1 != dev->yold)
	pls->bytecnt += fprintf( OF, "PU%d %d;", x1, y1 );

/* Add new point to path */

    pls->bytecnt += fprintf( OF, "PD%d %d\n", x2, y2);

    dev->xold = x2;
    dev->yold = y2;
}

/*--------------------------------------------------------------------------*\
 * plD_polyline_hpgl()
 *
 * Draw a polyline in the current color.
\*--------------------------------------------------------------------------*/

void
plD_polyline_hpgl(PLStream *pls, short *xa, short *ya, PLINT npts)
{
  register PLINT i;
  PLDev *dev = (PLDev *) pls->dev;

/* Write out old path */

  if (xa[0] != dev->xold || ya[0] != dev->yold)
      pls->bytecnt += fprintf( OF, "PU%d %d;", xa[0], ya[0] );

/* Add new point to path */

  for (i = 1; i < npts; i++)
      pls->bytecnt += fprintf( OF, "PD%d %d\n", xa[i], ya[i] );

  dev->xold = xa[ npts - 1 ];
  dev->yold = ya[ npts - 1 ];
}

/*--------------------------------------------------------------------------*\
 * plD_eop_hpgl()
 *
 * End of page.
\*--------------------------------------------------------------------------*/

void
plD_eop_hpgl(PLStream *pls)
{
}

/*--------------------------------------------------------------------------*\
 * plD_bop_hpgl()
 *
 * Set up for the next page.
 * Advance to next family file if necessary (file output).
\*--------------------------------------------------------------------------*/

void
plD_bop_hpgl(PLStream *pls)
{
    PLDev *dev = (PLDev *) pls->dev;

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;

    fputs( "PG;\n", OF );
    if (!pls->termin)
	plGetFam(pls);

    pls->page++;
}

/*--------------------------------------------------------------------------*\
 * plD_tidy_hpgl()
 *
 * Close graphics file or otherwise clean up.
\*--------------------------------------------------------------------------*/

void
plD_tidy_hpgl(PLStream *pls)
{
    fputs( "SP0\n", OF );
    fclose(OF);
}

/*--------------------------------------------------------------------------*\
 * plD_state_hpgl()
 *
 * Handle change in PLStream state (color, pen width, fill attribute, etc).
\*--------------------------------------------------------------------------*/

void
plD_state_hpgl(PLStream *pls, PLINT op)
{
    switch (op) {

    case PLSTATE_WIDTH:
    case PLSTATE_COLOR0:{
	int width = 
	    (pls->width < MIN_WIDTH) ? DEF_WIDTH :
	    (pls->width > MAX_WIDTH) ? MAX_WIDTH : pls->width;

	if ( pls->icol0 < 1 || pls->icol0 > 8)
	    fputs( "\nInvalid pen selection.", stderr );
	else
	    fprintf( OF, "SP%d %d\n", pls->icol0, width );

	break;
    }
    case PLSTATE_COLOR1:
	break;
    }
}

/*--------------------------------------------------------------------------*\
 * plD_esc_hpgl()
 *
 * Escape function.
\*--------------------------------------------------------------------------*/

void
plD_esc_hpgl(PLStream *pls, PLINT op, void *ptr)
{
}

#else
int
pldummy_hpgl(void)
{
    return 0;
}

#endif		/* PLD_hp7470 || PLD_hp7580 || PLD_lj_hpgl */
