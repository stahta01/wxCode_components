/*
  pbm.c

  PLplot PBM (PPM) device driver.

  Contributed by John C. Atkinson and Zulfi Cumali.
  Slightly modified by Geoffrey Furnish.

 * $Log: not supported by cvs2svn $
 * Revision 1.1.1.1  1999/09/01 14:01:44  fhuebner
 * Start of wxPlot
 *
 * Revision 1.2  1996/10/11  23:10:41  furnish
 * Improve performance by writing out data in one shot.
 *
 * Revision 1.1  1996/10/08  18:37:11  furnish
 * First cut on a new driver for portable bitmaps.
 *

*/
#include "plDevs.h"

#ifdef PLD_pbm

#include "plplotP.h"
#include "drivers.h"

#undef PIXELS_X
#undef PIXELS_Y
#define PIXELS_X 640
#define PIXELS_Y 480

static char cmap[PIXELS_Y][PIXELS_X][3];

#undef MAX
#undef ABS
#define MAX(a,b) ((a>b) ? a : b)
#define ABS(a) ((a<0) ? -a : a)

#define MAX_INTENSITY 255
/*--------------------------------------------------------------------------*\
 * plD_init_pbm()
 *
 * Initialize device (terminal).
\*--------------------------------------------------------------------------*/

void
plD_init_pbm(PLStream *pls)
{
#if 1

/* Initialize family file info */

    plFamInit(pls);

    plP_setpxl((PLFLT) 5.905, (PLFLT) 5.905);

#endif

    pls->color = 1;		/* Is a color device */
    pls->dev_fill0 = 0;		/* Handle solid fills */
    pls->dev_fill1 = 0;		/* Handle pattern fills */
    pls->nopause = 1;		/* Don`t pause between frames */

/* Prompt for a file name if not already set */

    plOpenFile(pls);
    pls->pdfs = pdf_finit(pls->OutFile);

/* Allocate and initialize device-specific data */

    pls->dev = NULL;

/* Set up device parameters */

    plP_setphy(0, PIXELS_X, 0, PIXELS_Y);
}

#if 0
void
plD_line_pbm(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
    int steps, i, dx, dy;
    double x_off, y_off, dx_step, dy_step;

/* This algoritm is by Steven Harrington
   From "Computer Graphics: A Proogramming Approach */

    dx = x2a - x1a;
    dy = y2a - y1a;
    steps = MAX(ABS(dx), ABS(dy)) + 1;
    steps *= 2;
    dx_step = dx/steps;
    dy_step = dy/steps;
    x_off = x1a + 0.5;
    y_off = y1a + 0.5;

    for (i = 0; i < steps; i++) {
	cmap[(int)y_off][(int)x_off][0] = pls->curcolor.r;
	cmap[(int)y_off][(int)x_off][1] = pls->curcolor.g;
	cmap[(int)y_off][(int)x_off][2] = pls->curcolor.b;
	x_off += dx_step;
	y_off += dy_step;
    }

    cmap[(int)y_off][(int)x_off][0] = pls->curcolor.r;
    cmap[(int)y_off][(int)x_off][1] = pls->curcolor.g;
    cmap[(int)y_off][(int)x_off][2] = pls->curcolor.b;

    return;
}
#endif

#define sign(a) ((a<0) ? -1 : ((a == 0) ? 0 : 1))

#if 0
#define plot(x,y,c) {cmap[y-1][x-1][0] = (c)->curcolor.r; \
					 cmap[y-1][x-1][1] = (c)->curcolor.g; \
					 cmap[y-1][x-1][2] = (c)->curcolor.b; }

/* Bresnham's  algorithm for line plotting on a scan lines */

void
plD_line_pbm(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
    int e, x, y, dx, dy, s1, s2, temp, change, i;

    x = x1a;
    y = y1a;

    dx = ABS(x2a - x1a);
    dy = ABS(y2a - y1a);
    s1 = sign(x2a - x1a);
    s2 = sign(y2a - y1a);

    if( dy > dx ){
	temp= dx;
	dx  = dy;
	dy  = temp;
	change = 1;
    }
    else {
	change = 0;
    }
    e = 2 * dy - dx;

    for( i=1; i<dx;i++){
	plot(x,y,pls);
	while( e>= 0 ) {
	    if(change == 1) x += s1;
	    else            y += s2;		
	    e = e - 2 * dx;
	}
	if(change == 1) y += s2;
	else            x += s1;
	e = e + 2 * dy;
    }
}
#else
#define plot(x,y,c) {cmap[y][x][0] = (c)->curcolor.r; \
					 cmap[y][x][1] = (c)->curcolor.g; \
					 cmap[y][x][2] = (c)->curcolor.b; }

/* Modified version of the ljii routine (see ljii.c) */
void
plD_line_pbm(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
    int i;
    int x1 = x1a, y1 = y1a, x2 = x2a, y2 = y2a;
    PLINT x1b, y1b, x2b, y2b;
    float length, fx, fy, dx, dy;

/* Take mirror image, since PCL expects (0,0) to be at top left */

    y1 = PIXELS_Y - (y1 - 0);
    y2 = PIXELS_Y - (y2 - 0);

/* Rotate by 90 degrees */
/*
  plRotPhy(1, 0, 0, PIXELS_X, PIXELS_Y, &x1, &y1);
  plRotPhy(1, 0, 0, PIXELS_X, PIXELS_Y, &x2, &y2);
  */
    x1b = x1, x2b = x2, y1b = y1, y2b = y2;
    length = (float) sqrt((double)
			  ((x2b - x1b) * (x2b - x1b) + (y2b - y1b) * (y2b - y1b)));

    if (length == 0.)
	length = 1.;
    dx = (x2 - x1) / length;
    dy = (y2 - y1) / length;

    fx = x1;
    fy = y1;
    plot((PLINT) x1, (PLINT) y1, pls);
    plot((PLINT) x2, (PLINT) y2, pls);

    for (i = 1; i <= (int) length; i++) {
	fx += dx; fy += dy;
	plot((PLINT) fx, (PLINT) fy, pls);
    }
}
#endif

void
plD_polyline_pbm(PLStream *pls, short *xa, short *ya, PLINT npts)
{
    int i;
    for (i=0; i<npts - 1; i++)
	plD_line_pbm(pls, xa[i], ya[i], xa[i + 1], ya[i + 1]); 
}

void
plD_eop_pbm(PLStream *pls)
{
    FILE *fp = pls->OutFile;
    int i, j, k;

    if (fp != NULL) {
	fprintf(fp, "%s\n", "P6");
	fprintf(fp, "%d %d\n", PIXELS_X, PIXELS_Y);
	fprintf(fp, "%d\n", MAX_INTENSITY);
    /*
	for (i=0; i<PIXELS_Y; i++)
	    for (j=0; j<PIXELS_X; j++)
		for (k=0; k<3; k++)
		    fprintf(fp, "%c", cmap[i][j][k]);
		    */
	fwrite( cmap, 1, PIXELS_X * PIXELS_Y * 3, fp );

	fclose(fp);
    } 
}

void
plD_bop_pbm(PLStream *pls)
{
/* Nothing to do here */
}

void
plD_tidy_pbm(PLStream *pls)
{
/* Nothing to do here */
}

void 
plD_state_pbm(PLStream *pls, PLINT op)
{
/* Nothing to do here */
}

void
plD_esc_pbm(PLStream *pls, PLINT op, void *ptr)
{
/* Nothing to do here */
}

#else
int 
pldummy_pbm()
{
    return 0;
}

#endif				/* PLD_pbm */


