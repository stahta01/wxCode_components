/* $Id: plcont.c,v 1.1.1.1 2003-12-28 21:07:46 wyo Exp $
 * $Log: not supported by cvs2svn $
 * Revision 1.1.1.1  1999/09/01 14:01:45  fhuebner
 * Start of wxPlot
 *
 * Revision 1.13  1995/03/16  23:59:57  mjl
 * Eliminated a #pragma message() that some people found objectionable :-).
 *
 * Revision 1.12  1994/06/30  18:22:02  mjl
 * All core source files: made another pass to eliminate warnings when using
 * gcc -Wall.  Lots of cleaning up: got rid of includes of math.h or string.h
 * (now included by plplot.h), and other minor changes.  Now each file has
 * global access to the plstream pointer via extern; many accessor functions
 * eliminated as a result.
*/

/*	plcont.c

	Contour plotter.
*/

#include "plplotP.h"

#ifdef MSDOS
#pragma optimize("",off)
#endif

/* Static function prototypes. */

static void
plcntr(PLFLT (*plf2eval) (PLINT, PLINT, PLPointer),
       PLPointer plf2eval_data,
       PLINT nx, PLINT ny, PLINT kx, PLINT lx,
       PLINT ky, PLINT ly, PLFLT flev, PLINT *iscan,
       PLINT *ixstor, PLINT *iystor, PLINT nstor,
       void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer),
       PLPointer pltr_data);

static void
pldrawcn(PLFLT (*plf2eval) (PLINT, PLINT, PLPointer),
	 PLPointer plf2eval_data,
	 PLINT nx, PLINT ny, PLINT kx, PLINT lx,
	 PLINT ky, PLINT ly, PLFLT flev, PLINT kcol, PLINT krow,
	 PLINT *p_kscan, PLINT *p_kstor, PLINT *iscan,
	 PLINT *ixstor, PLINT *iystor, PLINT nstor,
	 void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer),
	 PLPointer pltr_data);

static void
plccal(PLFLT (*plf2eval) (PLINT, PLINT, PLPointer),
       PLPointer plf2eval_data,
       PLFLT flev, PLINT ix, PLINT iy,
       PLINT ixg, PLINT iyg, PLFLT *dist);

static void 
plr45 (PLINT *ix, PLINT *iy, PLINT isens);

static void 
plr135 (PLINT *ix, PLINT *iy, PLINT isens);

/* Error flag for aborts */

static int error;

/*--------------------------------------------------------------------------*\
 * plf2eval2()
 *
 * Does a lookup from a 2d function array.  Array is of type (PLFLT **),
 * and is column dominant (normal C ordering).
\*--------------------------------------------------------------------------*/

PLFLT
plf2eval2(PLINT ix, PLINT iy, PLPointer plf2eval_data)
{
    PLFLT value;
    PLfGrid2 *grid = (PLfGrid2 *) plf2eval_data;

    value = grid->f[ix][iy];

    return value;
}

/*--------------------------------------------------------------------------*\
 * plf2eval()
 *
 * Does a lookup from a 2d function array.  Array is of type (PLFLT *), and
 * is column dominant (normal C ordering).  You MUST fill the ny maximum
 * array index entry in the PLfGrid struct.
\*--------------------------------------------------------------------------*/

PLFLT
plf2eval(PLINT ix, PLINT iy, PLPointer plf2eval_data)
{
    PLFLT value;
    PLfGrid *grid = (PLfGrid *) plf2eval_data;

    value = grid->f[ix * grid->ny + iy];

    return value;
}

/*--------------------------------------------------------------------------*\
 * plf2evalr()
 *
 * Does a lookup from a 2d function array.  Array is of type (PLFLT *), and
 * is row dominant (Fortran ordering).  You MUST fill the nx maximum array
 * index entry in the PLfGrid struct.
\*--------------------------------------------------------------------------*/

PLFLT
plf2evalr(PLINT ix, PLINT iy, PLPointer plf2eval_data)
{
    PLFLT value;
    PLfGrid *grid = (PLfGrid *) plf2eval_data;

    value = grid->f[ix + iy * grid->nx];

    return value;
}

/*--------------------------------------------------------------------------*\
 * void plcont()
 *
 * Draws a contour plot from data in f(nx,ny).  Is just a front-end to
 * plfcont, with a particular choice for f2eval and f2eval_data.
\*--------------------------------------------------------------------------*/

void
c_plcont(PLFLT **f, PLINT nx, PLINT ny, PLINT kx, PLINT lx,
	 PLINT ky, PLINT ly, PLFLT *clevel, PLINT nlevel,
	 void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer),
	 PLPointer pltr_data)
{
    PLfGrid2 grid;

    grid.f = f;
    plfcont(plf2eval2, (PLPointer) &grid,
	    nx, ny, kx, lx, ky, ly, clevel, nlevel,
	    pltr, pltr_data);
}

/*--------------------------------------------------------------------------*\
 * void plfcont()
 *
 * Draws a contour plot using the function evaluator f2eval and data stored
 * by way of the f2eval_data pointer.  This allows arbitrary organizations
 * of 2d array data to be used.
 *
 * The subrange of indices used for contouring is kx to lx in the x
 * direction and from ky to ly in the y direction. The array of contour
 * levels is clevel(nlevel), and "pltr" is the name of a function which
 * transforms array indicies into world coordinates.
 *
 * Note that the fortran-like minimum and maximum indices (kx, lx, ky, ly)
 * are translated into more C-like ones.  I've only kept them as they are
 * for the plcontf() argument list because of backward compatibility.
\*--------------------------------------------------------------------------*/

void
plfcont(PLFLT (*f2eval) (PLINT, PLINT, PLPointer),
	PLPointer f2eval_data,
	PLINT nx, PLINT ny, PLINT kx, PLINT lx,
	PLINT ky, PLINT ly, PLFLT *clevel, PLINT nlevel,
	void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer),
	PLPointer pltr_data)
{
    PLINT i, mx, my, nstor, *heapc;

    mx = lx - kx + 1;
    my = ly - ky + 1;

    if (kx < 1 || kx >= lx) {
	plabort("plfcont: indices must satisfy  1 <= kx <= lx <= nx");
	return;
    }
    if (ky < 1 || ky >= ly) {
	plabort("plfcont: indices must satisfy  1 <= ky <= ly <= ny");
	return;
    }

    nstor = mx * my;
    heapc = (PLINT *) malloc((size_t) (mx + 2 * nstor) * sizeof(PLINT));
    if (heapc == NULL) {
	plabort("plfcont: out of memory in heap allocation");
	return;
    }

    for (i = 0; i < nlevel; i++) {
	plcntr(f2eval, f2eval_data,
	       nx, ny, kx-1, lx-1, ky-1, ly-1, clevel[i], &heapc[0],
	       &heapc[nx], &heapc[nx + nstor], nstor, pltr, pltr_data);

	if (error) {
	    error = 0;
	    goto done;
	}
    }

  done:
    free((void *) heapc);
}

/*--------------------------------------------------------------------------*\
 * void plcntr()
 *
 * The contour for a given level is drawn here.  Note iscan has nx
 * elements. ixstor and iystor each have nstor elements.
\*--------------------------------------------------------------------------*/

static void
plcntr(PLFLT (*f2eval) (PLINT, PLINT, PLPointer),
       PLPointer f2eval_data,
       PLINT nx, PLINT ny, PLINT kx, PLINT lx,
       PLINT ky, PLINT ly, PLFLT flev, PLINT *iscan,
       PLINT *ixstor, PLINT *iystor, PLINT nstor,
       void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer),
       PLPointer pltr_data)
{
    PLINT kcol, krow, kstor, kscan, l, ixt, iyt, jstor, next;

/* Initialize memory pointers */

    kstor = 0;
    kscan = 0;

    for (krow = ky; krow <= ly; krow++) {
	for (kcol = kx + 1; kcol <= lx; kcol++) {

	/* Follow and draw a contour */

	    pldrawcn(f2eval, f2eval_data,
		     nx, ny, kx, lx, ky, ly, flev, kcol, krow,
		     &kscan, &kstor, iscan, ixstor, iystor, nstor,
		     pltr, pltr_data);

	    if (error)
		return;
	}

    /* Search of row complete */
    /* Set up memory of next row in iscan and edit ixstor and iystor */

	if (krow < ny-1) {
	    jstor = 0;
	    kscan = 0;
	    next = krow + 1;
	    for (l = 1; l <= kstor; l++) {
		ixt = ixstor[l - 1];
		iyt = iystor[l - 1];

	    /* Memory of next row into iscan */

		if (iyt == next) {
		    kscan = kscan + 1;
		    iscan[kscan - 1] = ixt;
		}

	    /* Retain memory of rows to come, and forget rest */

		else if (iyt > next) {
		    jstor = jstor + 1;
		    ixstor[jstor - 1] = ixt;
		    iystor[jstor - 1] = iyt;
		}
	    }
	    kstor = jstor;
	}
    }
}

/*--------------------------------------------------------------------------*\
 * void pldrawcn()
 *
 * Follow and draw a contour.
\*--------------------------------------------------------------------------*/

static void
pldrawcn(PLFLT (*f2eval) (PLINT, PLINT, PLPointer),
	 PLPointer f2eval_data,
	 PLINT nx, PLINT ny, PLINT kx, PLINT lx,
	 PLINT ky, PLINT ly, PLFLT flev, PLINT kcol, PLINT krow,
	 PLINT *p_kscan, PLINT *p_kstor, PLINT *iscan,
	 PLINT *ixstor, PLINT *iystor, PLINT nstor,
	 void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer),
	 PLPointer pltr_data)
{
    PLINT iwbeg, ixbeg, iybeg, izbeg;
    PLINT iboun, iw, ix, iy, iz, ifirst, istep, ixgo, iygo;
    PLINT l, ixg, iyg, ia, ib;

    PLFLT dist, dx, dy, xnew, ynew, fxl, fxr;
    PLFLT xlas = 0., ylas = 0., tpx, tpy, xt, yt;
    PLFLT f1, f2, f3, f4, fcheck;

/* Check if a contour has been crossed */

    fxl = f2eval(kcol-1, krow, f2eval_data);
    fxr = f2eval(kcol, krow, f2eval_data);

    if (fxl < flev && fxr >= flev) {
	ixbeg = kcol - 1;
	iwbeg = kcol;
    }
    else if (fxr < flev && fxl >= flev) {
	ixbeg = kcol;
	iwbeg = kcol - 1;
    }
    else
	return;

    iybeg = krow;
    izbeg = krow;

/* A contour has been crossed. */
/* Check to see if it is a new one. */

    for (l = 0; l < *p_kscan; l++) {
	if (ixbeg == iscan[l])
	    return;
    }

    for (iboun = 1; iboun >= -1; iboun -= 2) {

    /* Set up starting point and initial search directions */

	ix = ixbeg;
	iy = iybeg;
	iw = iwbeg;
	iz = izbeg;
	ifirst = 1;
	istep = 0;
	ixgo = iw - ix;
	iygo = iz - iy;

	for (;;) {
	    plccal(f2eval, f2eval_data,
		   flev, ix, iy, ixgo, iygo, &dist);

	    dx = dist * ixgo;
	    dy = dist * iygo;
	    xnew = ix + dx;
	    ynew = iy + dy;

	/* Has a step occured in search? */

	    if (istep != 0) {
		if (ixgo * iygo == 0) {

		/* This was a diagonal step, so interpolate missed point. */
		/* Rotating 45 degrees to get it */

		    ixg = ixgo;
		    iyg = iygo;
		    plr45(&ixg, &iyg, iboun);
		    ia = iw - ixg;
		    ib = iz - iyg;
		    plccal(f2eval, f2eval_data,
			   flev, ia, ib, ixg, iyg, &dist);

		    (*pltr) (xlas, ylas, &tpx, &tpy, pltr_data);

		    plP_drawor(tpx, tpy);
		    dx = dist * ixg;
		    dy = dist * iyg;
		    xlas = ia + dx;
		    ylas = ib + dy;
		}
		else {
		    if (dist > 0.5) {
			xt = xlas;
			xlas = xnew;
			xnew = xt;
			yt = ylas;
			ylas = ynew;
			ynew = yt;
		    }
		}
	    }
	    if (ifirst != 1) {
		(*pltr) (xlas, ylas, &tpx, &tpy, pltr_data);
		plP_drawor(tpx, tpy);
	    }
	    else {
		(*pltr) (xnew, ynew, &tpx, &tpy, pltr_data);
		plP_movwor(tpx, tpy);
	    }
	    xlas = xnew;
	    ylas = ynew;

	/* Check if the contour is closed */

	    if (ifirst != 1 &&
		ix == ixbeg && iy == iybeg && iw == iwbeg && iz == izbeg) {
		(*pltr) (xlas, ylas, &tpx, &tpy, pltr_data);
		plP_drawor(tpx, tpy);
		return;
	    }
	    ifirst = 0;

	/* Now the rotation */

	    istep = 0;
	    plr45(&ixgo, &iygo, iboun);
	    iw = ix + ixgo;
	    iz = iy + iygo;

	/* Check if out of bounds */

	    if (iw < kx || iw > lx || iz < ky || iz > ly)
		break;

	/* Has contact been lost with the contour? */

	    if (ixgo * iygo == 0)
		fcheck = f2eval(iw, iz, f2eval_data);
	    else {
		f1 = f2eval(ix, iy, f2eval_data);
		f2 = f2eval(iw, iz, f2eval_data);
		f3 = f2eval(ix, iz, f2eval_data);
		f4 = f2eval(iw, iy, f2eval_data);

		fcheck = MAX(f2, (f1 + f2 + f3 + f4) / 4.);
	    }

	    if (fcheck < flev) {

	    /* Yes, lost contact => step to new center */

		istep = 1;
		ix = iw;
		iy = iz;
		plr135(&ixgo, &iygo, iboun);
		iw = ix + ixgo;
		iz = iy + iygo;

	    /* And do the contour memory */

		if (iy == krow) {
		    *p_kscan = *p_kscan + 1;
		    iscan[*p_kscan - 1] = ix;
		}
		else if (iy > krow) {
		    *p_kstor = *p_kstor + 1;
		    if (*p_kstor > nstor) {
			plabort("plfcont: heap exhausted");
			error = 1;
			return;
		    }
		    ixstor[*p_kstor - 1] = ix;
		    iystor[*p_kstor - 1] = iy;
		}
	    }
	}
	/* Reach here only if boundary encountered - Draw last bit */

	(*pltr) (xnew, ynew, &tpx, &tpy, pltr_data);
	plP_drawor(tpx, tpy);
    }
}

/*--------------------------------------------------------------------------*\
 * void plccal()
 *
 * Function to interpolate the position of a contour which is known to be
 * next to ix,iy in the direction ixg,iyg. The unscaled distance along
 * ixg,iyg is returned as dist.
\*--------------------------------------------------------------------------*/

static void
plccal(PLFLT (*f2eval) (PLINT, PLINT, PLPointer),
       PLPointer f2eval_data,
       PLFLT flev, PLINT ix, PLINT iy,
       PLINT ixg, PLINT iyg, PLFLT *dist)
{
    PLINT ia, ib;
    PLFLT dbot, dtop, fmid;
    PLFLT fxy, fab, fay, fxb, flow;

    ia = ix + ixg;
    ib = iy + iyg;
    fxy = f2eval(ix, iy, f2eval_data);
    fab = f2eval(ia, ib, f2eval_data);
    fxb = f2eval(ix, ib, f2eval_data);
    fay = f2eval(ia, iy, f2eval_data);

    if (ixg == 0 || iyg == 0) {
	dtop = flev - fxy;
	dbot = fab - fxy;
	*dist = 0.0;
	if (dbot != 0.0)
	    *dist = dtop / dbot;
    }
    else {
	fmid = (fxy + fab + fxb + fay) / 4.0;
	*dist = 0.5;

	if ((fxy - flev) * (fab - flev) <= 0.) {

	    if (fmid >= flev) {
		dtop = flev - fxy;
		dbot = fmid - fxy;
		if (dbot != 0.0)
		    *dist = 0.5 * dtop / dbot;
	    }
	    else {
		dtop = flev - fab;
		dbot = fmid - fab;
		if (dbot != 0.0)
		    *dist = 1.0 - 0.5 * dtop / dbot;
	    }
	}
	else {
	    flow = (fxb + fay) / 2.0;
	    dtop = fab - flev;
	    dbot = fab + fxy - 2.0 * flow;
	    if (dbot != 0.0)
		*dist = 1. - dtop / dbot;
	}
    }
    if (*dist > 1.)
	*dist = 1.;
}

/*--------------------------------------------------------------------------*\
 * Rotators
\*--------------------------------------------------------------------------*/

static void 
plr45 (PLINT *ix, PLINT *iy, PLINT isens)
{
    PLINT ixx, iyy;

    ixx = *ix - isens * (*iy);
    iyy = *ix * isens + *iy;
    *ix = ixx / MAX(1, ABS(ixx));
    *iy = iyy / MAX(1, ABS(iyy));
}

static void 
plr135 (PLINT *ix, PLINT *iy, PLINT isens)
{
    *ix = -*ix;
    *iy = -*iy;
    plr45(ix, iy, isens);
}

/*--------------------------------------------------------------------------*\
 * pltr0()
 *
 * Identity transformation.
\*--------------------------------------------------------------------------*/

void
pltr0(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, PLPointer pltr_data)
{
    *tx = x;
    *ty = y;
}

/*--------------------------------------------------------------------------*\
 * pltr1()
 *
 * Does linear interpolation from singly dimensioned coord arrays.
 *
 * Just abort for now if coordinates are out of bounds (don't think it's
 * possible, but if so we could use linear extrapolation).
\*--------------------------------------------------------------------------*/

void
pltr1(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, PLPointer pltr_data)
{
    PLINT ul, ur, vl, vr;
    PLFLT du, dv;
    PLFLT xl, xr, yl, yr;

    PLcGrid *grid = (PLcGrid *) pltr_data;
    PLFLT *xg = grid->xg;
    PLFLT *yg = grid->yg;
    PLINT nx = grid->nx;
    PLINT ny = grid->ny;

    ul = (PLINT) x;
    ur = ul + 1;
    du = x - ul;

    vl = (PLINT) y;
    vr = vl + 1;
    dv = y - vl;

    if (x < 0 || x > nx - 1 || y < 0 || y > ny - 1) {
	plexit("pltr1: Invalid coordinates");
    }

/* Look up coordinates in row-dominant array.
 * Have to handle right boundary specially -- if at the edge, we'd better
 * not reference the out of bounds point.
 */

    xl = xg[ul];
    yl = yg[vl];

    if (ur == nx) {
	*tx = xl;
    }
    else {
	xr = xg[ur];
	*tx = xl * (1 - du) + xr * du;
    }
    if (vr == ny) {
	*ty = yl;
    }
    else {
	yr = yg[vr];
	*ty = yl * (1 - dv) + yr * dv;
    }
}

/*--------------------------------------------------------------------------*\
 * pltr2()
 *
 * Does linear interpolation from doubly dimensioned coord arrays (column
 * dominant, as per normal C 2d arrays).
 *
 * This routine includes lots of checks for out of bounds.  This would occur
 * occasionally due to some bugs in the contour plotter (now fixed).  If an
 * out of bounds coordinate is obtained, the boundary value is provided
 * along with a warning.  These checks should stay since no harm is done if
 * if everything works correctly.
\*--------------------------------------------------------------------------*/

void
pltr2(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, PLPointer pltr_data)
{
    PLINT ul, ur, vl, vr;
    PLFLT du, dv;
    PLFLT xll, xlr, xrl, xrr;
    PLFLT yll, ylr, yrl, yrr;
    PLFLT xmin, xmax, ymin, ymax;

    PLcGrid2 *grid = (PLcGrid2 *) pltr_data;
    PLFLT **xg = grid->xg;
    PLFLT **yg = grid->yg;
    PLINT nx = grid->nx;
    PLINT ny = grid->ny;

    ul = (PLINT) x;
    ur = ul + 1;
    du = x - ul;

    vl = (PLINT) y;
    vr = vl + 1;
    dv = y - vl;

    xmin = 0;
    xmax = nx - 1;
    ymin = 0;
    ymax = ny - 1;

    if (x < xmin || x > xmax || y < ymin || y > ymax) {
	plwarn("pltr2: Invalid coordinates");
	if (x < xmin) {

	    if (y < ymin) {
		*tx = xg[0][0];
		*ty = yg[0][0];
	    }
	    else if (y > ymax) {
		*tx = xg[0][ny-1];
		*ty = yg[0][ny-1];
	    }
	    else {
		xll = xg[0][vl];
		yll = yg[0][vl];
		xlr = xg[0][vr];
		ylr = yg[0][vr];

		*tx = xll * (1 - dv) + xlr * (dv);
		*ty = yll * (1 - dv) + ylr * (dv);
	    }
	}
	else if (x > xmax) {

	    if (y < ymin) {
		*tx = xg[nx-1][0];
		*ty = yg[nx-1][0];
	    }
	    else if (y > ymax) {
		*tx = xg[nx-1][ny-1];
		*ty = yg[nx-1][ny-1];
	    }
	    else {
		xll = xg[nx-1][vl];
		yll = yg[nx-1][vl];
		xlr = xg[nx-1][vr];
		ylr = yg[nx-1][vr];

		*tx = xll * (1 - dv) + xlr * (dv);
		*ty = yll * (1 - dv) + ylr * (dv);
	    }
	}
	else {
	    if (y < ymin) {
		xll = xg[ul][0];
		xrl = xg[ur][0];
		yll = yg[ul][0];
		yrl = yg[ur][0];

		*tx = xll * (1 - du) + xrl * (du);
		*ty = yll * (1 - du) + yrl * (du);
	    }
	    else if (y > ymax) {
		xlr = xg[ul][ny-1];
		xrr = xg[ur][ny-1];
		ylr = yg[ul][ny-1];
		yrr = yg[ur][ny-1];

		*tx = xlr * (1 - du) + xrr * (du);
		*ty = ylr * (1 - du) + yrr * (du);
	    }
	}
    }

/* Normal case.
 * Look up coordinates in row-dominant array.
 * Have to handle right boundary specially -- if at the edge, we'd
 * better not reference the out of bounds point. 
 */

    else {

	xll = xg[ul][vl];
	yll = yg[ul][vl];

    /* ur is out of bounds */

	if (ur == nx && vr < ny) {

	    xlr = xg[ul][vr];
	    ylr = yg[ul][vr];

	    *tx = xll * (1 - dv) + xlr * (dv);
	    *ty = yll * (1 - dv) + ylr * (dv);
	}

    /* vr is out of bounds */

	else if (ur < nx && vr == ny) {

	    xrl = xg[ur][vl];
	    yrl = yg[ur][vl];

	    *tx = xll * (1 - du) + xrl * (du);
	    *ty = yll * (1 - du) + yrl * (du);
	}

    /* both ur and vr are out of bounds */

	else if (ur == nx && vr == ny) {

	    *tx = xll;
	    *ty = yll;
	}

    /* everything in bounds */

	else {

	    xrl = xg[ur][vl];
	    xlr = xg[ul][vr];
	    xrr = xg[ur][vr];

	    yrl = yg[ur][vl];
	    ylr = yg[ul][vr];
	    yrr = yg[ur][vr];

	    *tx = xll * (1 - du) * (1 - dv) + xlr * (1 - du) * (dv) +
		xrl * (du) * (1 - dv) + xrr * (du) * (dv);

	    *ty = yll * (1 - du) * (1 - dv) + ylr * (1 - du) * (dv) +
		yrl * (du) * (1 - dv) + yrr * (du) * (dv);
	}
    }
}

/*--------------------------------------------------------------------------*\
 * pltr2p()
 *
 * Just like pltr2() but uses pointer arithmetic to get coordinates from 2d
 * grid tables.  This form of grid tables is compatible with those from
 * PLplot 4.0.  The grid data must be pointed to by a PLcGrid structure.
\*--------------------------------------------------------------------------*/

void
pltr2p(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, PLPointer pltr_data)
{
    PLINT ul, ur, vl, vr;
    PLFLT du, dv;
    PLFLT xll, xlr, xrl, xrr;
    PLFLT yll, ylr, yrl, yrr;
    PLFLT xmin, xmax, ymin, ymax;

    PLcGrid *grid = (PLcGrid *) pltr_data;
    PLFLT *xg = grid->xg;
    PLFLT *yg = grid->yg;
    PLINT nx = grid->nx;
    PLINT ny = grid->ny;

    ul = (PLINT) x;
    ur = ul + 1;
    du = x - ul;

    vl = (PLINT) y;
    vr = vl + 1;
    dv = y - vl;

    xmin = 0;
    xmax = nx - 1;
    ymin = 0;
    ymax = ny - 1;

    if (x < xmin || x > xmax || y < ymin || y > ymax) {
	plwarn("pltr2p: Invalid coordinates");
	if (x < xmin) {

	    if (y < ymin) {
		*tx = *xg;
		*ty = *yg;
	    }
	    else if (y > ymax) {
		*tx = *(xg + (ny - 1));
		*ty = *(yg + (ny - 1));
	    }
	    else {
		ul = 0;
		xll = *(xg + ul * ny + vl);
		yll = *(yg + ul * ny + vl);
		xlr = *(xg + ul * ny + vr);
		ylr = *(yg + ul * ny + vr);

		*tx = xll * (1 - dv) + xlr * (dv);
		*ty = yll * (1 - dv) + ylr * (dv);
	    }
	}
	else if (x > xmax) {

	    if (y < ymin) {
		*tx = *(xg + (ny - 1) * nx);
		*ty = *(yg + (ny - 1) * nx);
	    }
	    else if (y > ymax) {
		*tx = *(xg + (ny - 1) + (nx - 1) * ny);
		*ty = *(yg + (ny - 1) + (nx - 1) * ny);
	    }
	    else {
		ul = nx - 1;
		xll = *(xg + ul * ny + vl);
		yll = *(yg + ul * ny + vl);
		xlr = *(xg + ul * ny + vr);
		ylr = *(yg + ul * ny + vr);

		*tx = xll * (1 - dv) + xlr * (dv);
		*ty = yll * (1 - dv) + ylr * (dv);
	    }
	}
	else {
	    if (y < ymin) {
		vl = 0;
		xll = *(xg + ul * ny + vl);
		xrl = *(xg + ur * ny + vl);
		yll = *(yg + ul * ny + vl);
		yrl = *(yg + ur * ny + vl);

		*tx = xll * (1 - du) + xrl * (du);
		*ty = yll * (1 - du) + yrl * (du);
	    }
	    else if (y > ymax) {
		vr = ny - 1;
		xlr = *(xg + ul * ny + vr);
		xrr = *(xg + ur * ny + vr);
		ylr = *(yg + ul * ny + vr);
		yrr = *(yg + ur * ny + vr);

		*tx = xlr * (1 - du) + xrr * (du);
		*ty = ylr * (1 - du) + yrr * (du);
	    }
	}
    }

/* Normal case.
 * Look up coordinates in row-dominant array.
 * Have to handle right boundary specially -- if at the edge, we'd better
 * not reference the out of bounds point.
 */

    else {

	xll = *(xg + ul * ny + vl);
	yll = *(yg + ul * ny + vl);

    /* ur is out of bounds */

	if (ur == nx && vr < ny) {

	    xlr = *(xg + ul * ny + vr);
	    ylr = *(yg + ul * ny + vr);

	    *tx = xll * (1 - dv) + xlr * (dv);
	    *ty = yll * (1 - dv) + ylr * (dv);
	}

    /* vr is out of bounds */

	else if (ur < nx && vr == ny) {

	    xrl = *(xg + ur * ny + vl);
	    yrl = *(yg + ur * ny + vl);

	    *tx = xll * (1 - du) + xrl * (du);
	    *ty = yll * (1 - du) + yrl * (du);
	}

    /* both ur and vr are out of bounds */

	else if (ur == nx && vr == ny) {

	    *tx = xll;
	    *ty = yll;
	}

    /* everything in bounds */

	else {

	    xrl = *(xg + ur * ny + vl);
	    xlr = *(xg + ul * ny + vr);
	    xrr = *(xg + ur * ny + vr);

	    yrl = *(yg + ur * ny + vl);
	    ylr = *(yg + ul * ny + vr);
	    yrr = *(yg + ur * ny + vr);

	    *tx = xll * (1 - du) * (1 - dv) + xlr * (1 - du) * (dv) +
		xrl * (du) * (1 - dv) + xrr * (du) * (dv);

	    *ty = yll * (1 - du) * (1 - dv) + ylr * (1 - du) * (dv) +
		yrl * (du) * (1 - dv) + yrr * (du) * (dv);
	}
    }
}
