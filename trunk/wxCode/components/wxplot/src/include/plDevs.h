/* -*-C-*- */
/* $Id: plDevs.h,v 1.2 2005-11-22 15:56:49 carreracg Exp $

    Maurice LeBrun
    IFS, University of Texas at Austin
    18-Jul-1994

    Contains macro definitions that determine what device drivers are
    compiled into the PLplot library.  On a Unix system, typically the
    configure script builds plDevs.h from plDevs.h.in.  Elsewhere, it's
    best to hand-configure a plDevs.h file and keep it with the
    system-specific files.

    Copyright (C) 2004  Andrew Roach
    Copyright (C) 2005  Thomas J. Duck

    This file is part of PLplot.

    PLplot is free software; you can redistribute it and/or modify
    it under the terms of the GNU Library General Public License as published
    by the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    PLplot is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with PLplot; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

*/

#ifndef __PLDEVS_H__
#define __PLDEVS_H__

#undef PDL_aqt
#undef PLD_plmeta
#undef PLD_null
#undef PLD_xterm
#undef PLD_tek4010
#undef PLD_tek4010f
#undef PLD_tek4107
#undef PLD_tek4107f
#undef PLD_mskermit
#undef PLD_vlt
#undef PLD_versaterm
#undef PLD_conex
#undef PLD_linuxvga
#undef PLD_dg300
#undef PLD_png
#undef PLD_jpeg
#undef PLD_gif
#undef PLD_cgm
#undef PLD_ps
#undef PLD_xfig
#undef PLD_ljiip
#undef PLD_ljii
#undef PLD_lj_hpgl
#undef PLD_hp7470
#undef PLD_hp7580
#undef PLD_imp
#undef PLD_xwin
#undef PLD_tk
#undef PLD_pbm
#undef PLD_gcw
#undef PLD_gnome
#undef PLD_pstex
#undef PLD_ntk
#undef PLD_tkwin
#undef PLD_mem
#undef PLD_wingcc

//********************************************************
//wxPlot
#define PLD_wxwin 

#endif	/* __PLDEVS_H__ */
