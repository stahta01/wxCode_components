/* $Id: pldtik.cpp,v 1.1.1.1 2003-12-28 21:06:10 wyo Exp $
 * $Log: not supported by cvs2svn $
 * Revision 1.1.1.1  1999/09/01 14:01:40  fhuebner
 * Start of wxPlot
 *
 * Revision 1.1.1.1  1999/09/01 12:55:12  fhuebner
 * Start of wxPlot
 *
 * Revision 1.8  1995/01/06  07:56:10  mjl
 * Broke pldtik() into two functions -- pldtik(), which handles only tick
 * spacing, and pldprec(), which handles only precision and significant
 * digits settings.  Each substantially clarified in the process.
 *
 * Revision 1.7  1994/06/30  18:22:06  mjl
 * All core source files: made another pass to eliminate warnings when using
 * gcc -Wall.  Lots of cleaning up: got rid of includes of math.h or string.h
 * (now included by plplot.h), and other minor changes.  Now each file has
 * global access to the plstream pointer via extern; many accessor functions
 * eliminated as a result.
 *
*/

/*	pldtik.c

	Determines tick spacing and mode (fixed or floating) of
	numeric axis labels.
*/

#include "plplotP.h"

/*----------------------------------------------------------------------*\
 * void pldtik()
 *
 * Determine tick spacing: works out a "nice" interval (if tick == 0) such
 * that there are between 3 and 7.5 major tick intervals in the input
 * range vmin to vmax.  The recommended number of subticks is returned in
 * "nsubt" unless the routine is entered with a non-zero value of "nsubt".
\*----------------------------------------------------------------------*/

void
pldtik(PLFLT vmin, PLFLT vmax, PLFLT *tick, PLINT *nsubt)
{
    PLFLT t1, t2;
    PLINT np, ns;

/* Magnitude of min/max difference to get tick spacing */

    t1 = (PLFLT) log10(ABS(vmax - vmin));
    np = (PLINT) floor(t1);
    t1 = t1 - np;

/* Get tick spacing. */

    if (t1 > 0.7781512503) {
	t2 = 2.0;
	ns = 4;
    }
    else if (t1 > 0.4771212549) {
	t2 = 1.0;
	ns = 5;
    }
    else if (t1 > 0.1760912591) {
	t2 = 5.0;
	ns = 5;
	np = np - 1;
    }
    else {
	t2 = 2.0;
	ns = 4;
	np = np - 1;
    }

/* Now compute tick spacing */

    if (*tick == 0) {
	*tick = t2 * pow(10.0, (double) np);
	if (vmin > vmax)
	    *tick = -*tick;
    }
    if (*nsubt == 0)
	*nsubt = ns;
}

/*----------------------------------------------------------------------*\
 * void pldprec()
 *
 * Determine precision: the output variable "mode" is set to 0 if labels
 * are to be written in floating-point format, or to 1 if they are to be
 * written in scientific format.  For mode = 1, the exponent will be
 * placed at:
 *
 * 	top left	for vertical axis on left
 * 	top right	for vertical axis on right
 * 	bottom right	for horizontal axis
 *
 * The digmax flag can be set by the user, and represents the maximum
 * number of digits a label may occupy.  If digmax<0, it is disregarded,
 * and if digmax=0 the default value is used.  For digmax>0, mode=1 is
 * chosen if there is insufficient room for the label within the specified
 * # of digits.
 *
 * In the case of mode=0, the actual # of digits will become too large
 * when the magnitude of the labels become too large.  The mode=1 case
 * offers the greatest precision for the smallest field length.
 *
 * The determination of maximum length for fixed point quantities is
 * complicated by the fact that very long fixed point representations look
 * much worse than the same sized floating point representation.  Further,
 * a fixed point number with a large negative exponent will actually gain
 * in precision when written as floating point.  Thus we use certain fuzz
 * factors to get 'digfix' from 'digmax', however it will always be true
 * that digfix<=digmax.
 *
 * Finally, if 'digmax' is set, 'prec' is reduced in size if necessary so
 * that the labels fit the requested field length.
\*----------------------------------------------------------------------*/

#define MIN_FLTDIG	3	/* disregarded if fractional part is 0 */
#define MAX_FIXDIG_POS	6
#define MAX_FIXDIG_NEG	4
#define DIGMAX_DEF	5

void
pldprec(PLFLT vmin, PLFLT vmax, PLFLT tick, PLINT lf, 
	PLINT *mode, PLINT *prec, PLINT digmax, PLINT *scale)
{
    PLFLT vmod, t0;
    PLINT msd, np, digmin, digfix;

    *mode = 0;
    *scale = 0;

    if (digmax == 0)
	digmax = DIGMAX_DEF;

/* Magnitute of min/max to get number of significant digits */

    vmod = MAX(ABS(vmin), ABS(vmax));
    t0 = (PLFLT) log10(vmod);
    msd = (PLINT) floor(t0);

/* Autoselect the mode flag */
/* 'digmin' is the minimum number of places taken up by the label */

    if (msd >= 0) {
	digmin = msd + 1;
	digfix = MAX_FIXDIG_POS;
	if (digmax > 0)
	    digfix = MIN(digmax, MAX_FIXDIG_POS);
    }
    else {
	digmin = -msd + 2;
	digfix = MAX_FIXDIG_NEG;
	if (digmax > 0)
	    digfix = MIN(digmax, MAX_FIXDIG_NEG);
    }

    if (digmin > digfix && !lf) {
	*mode = 1;
	*scale = msd;
    }

/* Establish precision.  */
/* It must be fine enough to resolve the tick spacing */

    np = (PLINT) floor(log10(ABS(tick)));

    if (*mode != 0)
	*prec = msd - np;
    else
	*prec = MAX(-np, 0);

/* One last hack required: if exponent < 0, i.e. number has leading '0.',
 * it's better to change to floating point form if the number of digits
 * is insufficient to represent the tick spacing.
*/
    if (*mode == 0 && digmax > 0) {
	if (t0 < 0.0) {
	    if (digmax - 2 - *prec < 0) {
		*mode = 1;
		*scale = msd;
	    }
	}
	else
	    *prec = MAX(MIN(*prec, digmax - msd - 1), 0);
    }
    if (*mode != 0) {
	*prec = msd - np;
	*prec = MAX(MIN(*prec, MAX(digmax-1, MIN_FLTDIG)), 0);
    }
}
