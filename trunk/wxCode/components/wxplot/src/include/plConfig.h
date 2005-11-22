/* -*-C-*- */
/* $Id: plConfig.h,v 1.2 2005-11-22 15:56:49 carreracg Exp $

    Maurice LeBrun
    IFS, University of Texas at Austin
    18-Jul-1994

    Copyright (C) 2004  Alan W. Irwin
    Copyright (C) 2004  Rafael Laboissiere
    Copyright (C) 2004  Joao Cardoso

    This file is part of PLplot.

    PLplot is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Library Public License as published
    by the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    PLplot is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with PLplot; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA


    Contains macro definitions that determine miscellaneous PLplot library
    configuration defaults, such as macros for bin, font, lib, and tcl
    install directories, and various system dependencies.  On a Unix
    system, typically the configure script builds plConfig.h from
    plConfig.h.in.  Elsewhere, it's best to hand-configure a plConfig.h
    file and keep it with the system-specific files.
*/

#ifndef __PLCONFIG_H__
#define __PLCONFIG_H__

#if HAVE_CONFIG_H
#  include <config.h>
#endif

/* Define if dynamic drivers are enabled.*/
#undef ENABLE_DYNDRIVERS

/* Define if [incr Tcl] is available */
#undef HAVE_ITCL

/* Define if [incr Tk] is available */
#undef HAVE_ITK

/* Define if [freetype] is available */
#undef HAVE_FREETYPE

/* Define if finite is available */
#undef HAVE_FINITE

/* Define if isinf is available */
#undef HAVE_ISINF

/* Define if isnan is available */
//#undef HAVE_ISNAN

#define HAVE_ISNAN

/* Define to 1 if you have the <unistd.h> header file. */
#undef HAVE_UNISTD_H

/* Define if usleep is available */
#undef HAVE_USLEEP

/* Define if you want PLplot's float type to be double */
#undef PL_DOUBLE

/* Define if C++ compiler accepts using namespace */
#undef USE_NAMESPACE

/* Define if <cmath> header file is available */
#undef HAVE_CMATH

/* Define if isnan is present in <math.h> but not in <cmath> 
 * - broken Mac OSX systems */
#undef BROKEN_ISNAN_CXX

#endif	/* __PLCONFIG_H__ */

//********************************************
//wxPlot
#define VERSION "wxPlot-plplot-5.5.1"
#define BUILD_DIR "./"


