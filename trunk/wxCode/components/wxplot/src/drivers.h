/* $Id: drivers.h,v 1.1.1.1 2003-12-28 21:05:56 wyo Exp $
 * $Log: not supported by cvs2svn $
 * Revision 1.1.1.1  1999/09/01 14:01:40  fhuebner
 * Start of wxPlot
 *
 * Revision 1.1.1.1  1999/09/01 12:55:12  fhuebner
 * Start of wxPlot
 *
 * Revision 1.24  1996/10/08  18:37:46  furnish
 * First cut on a new driver for portable bitmaps.
 *
 * Revision 1.23  1995/05/26  20:20:11  mjl
 * Changed plstream.h -> plstrm.h everywhere.
 *
 * Revision 1.22  1995/04/11  09:40:25  mjl
 * Prototypes for Mac driver.
 *
 * Revision 1.21  1994/08/26  19:17:42  mjl
 * Entries for conex device added; contributed by Mark Olesen.
 *
 * Revision 1.20  1994/07/25  06:04:58  mjl
 * Added prototype for new lj_hpgl driver.
 *
 * Revision 1.19  1994/07/19  22:35:14  mjl
 * Cruft elimination.
 *
 * Revision 1.18  1994/05/25  21:58:25  mjl
 * Function prototypes for Win3 driver, contributed by Paul Casteels.
 *
 * Revision 1.17  1994/05/25  09:36:57  mjl
 * All VGA driver function names now end with "_vga", a nice simplification.
 * Since all are compiler-dependent and mutually exclusive, this should pose
 * no problem.  Also HP pen plotter driver were consolidated.  Both
 * contributions by Mark Olesen (olesen@weber.me.queensu.ca).
*/

/*	drivers.h

	Contains all prototypes for driver functions.
*/

#ifndef __DRIVERS_H__
#define __DRIVERS_H__

#include "pdf.h"
#include "plstrm.h"

#ifdef __cplusplus
extern "C" {
#endif

void plD_init_tk		(PLStream *);
void plD_init_dp		(PLStream *);
void plD_line_tk		(PLStream *, short, short, short, short);
void plD_polyline_tk		(PLStream *, short *, short *, PLINT);
void plD_eop_tk			(PLStream *);
void plD_bop_tk			(PLStream *);
void plD_tidy_tk		(PLStream *);
void plD_state_tk		(PLStream *, PLINT);
void plD_esc_tk			(PLStream *, PLINT, void *);

void plD_init_xw		(PLStream *);
void plD_line_xw		(PLStream *, short, short, short, short);
void plD_polyline_xw		(PLStream *, short *, short *, PLINT);
void plD_eop_xw			(PLStream *);
void plD_bop_xw			(PLStream *);
void plD_tidy_xw		(PLStream *);
void plD_state_xw		(PLStream *, PLINT);
void plD_esc_xw			(PLStream *, PLINT, void *);

void plD_init_xterm		(PLStream *);
void plD_init_tekt		(PLStream *);
void plD_init_tekf		(PLStream *);
void plD_init_tek4107t		(PLStream *);
void plD_init_tek4107f		(PLStream *);
void plD_init_mskermit		(PLStream *);
void plD_init_versaterm		(PLStream *);
void plD_init_vlt		(PLStream *);
void plD_init_conex		(PLStream *);

void plD_line_tek		(PLStream *, short, short, short, short);
void plD_polyline_tek		(PLStream *, short *, short *, PLINT);
void plD_eop_tek		(PLStream *);
void plD_bop_tek		(PLStream *);
void plD_tidy_tek		(PLStream *);
void plD_state_tek		(PLStream *, PLINT);
void plD_esc_tek		(PLStream *, PLINT, void *);

void plD_init_mgr		(PLStream *);
void plD_line_mgr		(PLStream *, short, short, short, short);
void plD_polyline_mgr		(PLStream *, short *, short *, PLINT);
void plD_eop_mgr		(PLStream *);
void plD_bop_mgr		(PLStream *);
void plD_tidy_mgr		(PLStream *);
void plD_state_mgr		(PLStream *, PLINT);
void plD_esc_mgr		(PLStream *, PLINT, void *);

void plD_init_dg		(PLStream *);
void plD_line_dg		(PLStream *, short, short, short, short);
void plD_polyline_dg		(PLStream *, short *, short *, PLINT);
void plD_eop_dg			(PLStream *);
void plD_bop_dg			(PLStream *);
void plD_tidy_dg		(PLStream *);
void plD_state_dg		(PLStream *, PLINT);
void plD_esc_dg			(PLStream *, PLINT, void *);

void plD_init_hp7470		(PLStream *);
void plD_init_hp7580		(PLStream *);
void plD_init_lj_hpgl		(PLStream *);

void plD_line_hpgl		(PLStream *, short, short, short, short);
void plD_polyline_hpgl		(PLStream *, short *, short *, PLINT);
void plD_eop_hpgl		(PLStream *);
void plD_bop_hpgl		(PLStream *);
void plD_tidy_hpgl		(PLStream *);
void plD_state_hpgl		(PLStream *, PLINT);
void plD_esc_hpgl		(PLStream *, PLINT, void *);

void plD_init_imp		(PLStream *);
void plD_line_imp		(PLStream *, short, short, short, short);
void plD_polyline_imp		(PLStream *, short *, short *, PLINT);
void plD_eop_imp		(PLStream *);
void plD_bop_imp		(PLStream *);
void plD_tidy_imp		(PLStream *);
void plD_state_imp		(PLStream *, PLINT);
void plD_esc_imp		(PLStream *, PLINT, void *);

void plD_init_xfig		(PLStream *);
void plD_line_xfig		(PLStream *, short, short, short, short);
void plD_polyline_xfig		(PLStream *, short *, short *, PLINT);
void plD_eop_xfig		(PLStream *);
void plD_bop_xfig		(PLStream *);
void plD_tidy_xfig		(PLStream *);
void plD_state_xfig		(PLStream *, PLINT);
void plD_esc_xfig		(PLStream *, PLINT, void *);

void plD_init_ljiip		(PLStream *);
void plD_line_ljiip		(PLStream *, short, short, short, short);
void plD_polyline_ljiip		(PLStream *, short *, short *, PLINT);
void plD_eop_ljiip		(PLStream *);
void plD_bop_ljiip		(PLStream *);
void plD_tidy_ljiip		(PLStream *);
void plD_state_ljiip		(PLStream *, PLINT);
void plD_esc_ljiip		(PLStream *, PLINT, void *);

void plD_init_ljii		(PLStream *);
void plD_line_ljii		(PLStream *, short, short, short, short);
void plD_polyline_ljii		(PLStream *, short *, short *, PLINT);
void plD_eop_ljii		(PLStream *);
void plD_bop_ljii		(PLStream *);
void plD_tidy_ljii		(PLStream *);
void plD_state_ljii		(PLStream *, PLINT);
void plD_esc_ljii		(PLStream *, PLINT, void *);

void plD_init_psm		(PLStream *);
void plD_init_psc		(PLStream *);
void plD_line_ps		(PLStream *, short, short, short, short);
void plD_polyline_ps		(PLStream *, short *, short *, PLINT);
void plD_eop_ps			(PLStream *);
void plD_bop_ps			(PLStream *);
void plD_tidy_ps		(PLStream *);
void plD_state_ps		(PLStream *, PLINT);
void plD_esc_ps			(PLStream *, PLINT, void *);

void plD_init_nx		(PLStream *);
void plD_line_nx		(PLStream *, short, short, short, short);
void plD_polyline_nx		(PLStream *, short *, short *, PLINT);
void plD_eop_nx			(PLStream *);
void plD_bop_nx			(PLStream *);
void plD_tidy_nx		(PLStream *);
void plD_state_nx		(PLStream *, PLINT);
void plD_esc_nx			(PLStream *, PLINT, void *);

void plD_init_plm		(PLStream *);
void plD_line_plm		(PLStream *, short, short, short, short);
void plD_polyline_plm		(PLStream *, short *, short *, PLINT);
void plD_eop_plm		(PLStream *);
void plD_bop_plm		(PLStream *);
void plD_tidy_plm		(PLStream *);
void plD_state_plm		(PLStream *, PLINT);
void plD_esc_plm		(PLStream *, PLINT, void *);

void plD_init_vga		(PLStream *);
void plD_line_vga		(PLStream *, short, short, short, short);
void plD_polyline_vga		(PLStream *, short *, short *, PLINT);
void plD_eop_vga		(PLStream *);
void plD_bop_vga		(PLStream *);
void plD_tidy_vga		(PLStream *);
void plD_state_vga		(PLStream *, PLINT);
void plD_esc_vga		(PLStream *, PLINT, void *);

void plD_init_mac		(PLStream *);
void plD_line_mac		(PLStream *, short, short, short, short);
void plD_polyline_mac		(PLStream *, short *, short *, PLINT);
void plD_eop_mac		(PLStream *);
void plD_bop_mac		(PLStream *);
void plD_tidy_mac		(PLStream *);
void plD_state_mac		(PLStream *, PLINT);
void plD_esc_mac		(PLStream *, PLINT, void *);

void plD_init_win3		(PLStream *);
void plD_line_win3		(PLStream *, short, short, short, short);
void plD_polyline_win3		(PLStream *, short *, short *, PLINT);
void plD_eop_win3		(PLStream *);
void plD_bop_win3		(PLStream *);
void plD_tidy_win3		(PLStream *);
void plD_state_win3		(PLStream *, PLINT);
void plD_esc_win3		(PLStream *, PLINT, void *);

void plD_init_os2		(PLStream *);				 
void plD_line_os2		(PLStream *, short, short, short, short);
void plD_polyline_os2		(PLStream *, short *, short *, PLINT);
void plD_eop_os2		(PLStream *);				 
void plD_bop_os2		(PLStream *);				 
void plD_tidy_os2		(PLStream *);				 
void plD_state_os2		(PLStream *, PLINT);
void plD_esc_os2		(PLStream *, PLINT, void *);		 

void plD_init_amiwn		(PLStream *);
void plD_line_amiwn		(PLStream *, short, short, short, short);
void plD_polyline_amiwn		(PLStream *, short *, short *, PLINT);
void plD_eop_amiwn		(PLStream *);
void plD_bop_amiwn		(PLStream *);
void plD_tidy_amiwn		(PLStream *);
void plD_state_amiwn		(PLStream *, PLINT);
void plD_esc_amiwn		(PLStream *, PLINT, void *);

void plD_init_amipr		(PLStream *);
void plD_line_amipr		(PLStream *, short, short, short, short);
void plD_polyline_amipr		(PLStream *, short *, short *, PLINT);
void plD_eop_amipr		(PLStream *);
void plD_bop_amipr		(PLStream *);
void plD_tidy_amipr		(PLStream *);
void plD_state_amipr		(PLStream *, PLINT);
void plD_esc_amipr		(PLStream *, PLINT, void *);

void plD_init_iff		(PLStream *);
void plD_line_iff		(PLStream *, short, short, short, short);
void plD_polyline_iff		(PLStream *, short *, short *, PLINT);
void plD_eop_iff		(PLStream *);
void plD_bop_iff		(PLStream *);
void plD_tidy_iff		(PLStream *);
void plD_state_iff		(PLStream *, PLINT);
void plD_esc_iff		(PLStream *, PLINT, void *);

void plD_init_aegis		(PLStream *);
void plD_line_aegis		(PLStream *, short, short, short, short);
void plD_polyline_aegis		(PLStream *, short *, short *, PLINT);
void plD_eop_aegis		(PLStream *);
void plD_bop_aegis		(PLStream *);
void plD_tidy_aegis		(PLStream *);
void plD_state_aegis		(PLStream *, PLINT);
void plD_esc_aegis		(PLStream *, PLINT, void *);

void plD_init_pbm		(PLStream *);
void plD_line_pbm		(PLStream *, short, short, short, short);
void plD_polyline_pbm		(PLStream *, short *, short *, PLINT);
void plD_eop_pbm		(PLStream *);
void plD_bop_pbm		(PLStream *);
void plD_tidy_pbm		(PLStream *);
void plD_state_pbm		(PLStream *, PLINT);
void plD_esc_pbm		(PLStream *, PLINT, void *);

void plD_init_null		(PLStream *);
void plD_line_null		(PLStream *, short, short, short, short);
void plD_polyline_null		(PLStream *, short *, short *, PLINT);
void plD_eop_null		(PLStream *);
void plD_bop_null		(PLStream *);
void plD_tidy_null		(PLStream *);
void plD_state_null		(PLStream *, PLINT);
void plD_esc_null		(PLStream *, PLINT, void *);

void plD_init_wxwin		(PLStream *);
void plD_line_wxwin		(PLStream *, short, short, short, short);
void plD_polyline_wxwin		(PLStream *, short *, short *, PLINT);
void plD_eop_wxwin		(PLStream *);
void plD_bop_wxwin		(PLStream *);
void plD_tidy_wxwin		(PLStream *);
void plD_state_wxwin		(PLStream *, PLINT);
void plD_esc_wxwin		(PLStream *, PLINT, void *);


/* Prototypes for plot buffer calls. */

void plbuf_init		(PLStream *);
void plbuf_line		(PLStream *, short, short, short, short);
void plbuf_polyline	(PLStream *, short *, short *, PLINT);
void plbuf_eop		(PLStream *);
void plbuf_bop		(PLStream *);
void plbuf_tidy		(PLStream *);
void plbuf_state	(PLStream *, PLINT);
void plbuf_esc		(PLStream *, PLINT, void *);

void plRemakePlot	(PLStream *);

#ifdef __cplusplus
}
#endif

#endif	/* __DRIVERS_H__ */
