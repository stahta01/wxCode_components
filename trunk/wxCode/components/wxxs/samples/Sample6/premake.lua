--*****************************************************************************
--*	Author:		Michal Bližňák
--*	Date:		16/03/2009
--*	Version:	1.00
--*	
--*	NOTES:
--*		- use the '/' slash for all paths.
--*****************************************************************************

dofile('../../build/premake/scripts/init.lua')

--******* Initial Setup ************
--*	Most of the setting are set here.
--**********************************

-- Set the name of your package.
package.name = "SimpleTreeSample"

-- Set this if you want a different name for your target than the package's name.
targetName = "SimpleTreeSample"

-- Set the kind of package you want to create.
--		Options: exe | winexe | lib | dll
package.kind = "exe"

-- Setup the package compiler settings.
if ( target == "vs2005" ) then
	-- Windows and Visual C++ 2005
	package.defines = { "_CRT_SECURE_NO_DEPRECATE", "_DISWARNINGS_MSVC" }
	package.buildflags = { "no-main" }
end
if( ( target == "vs2003" or target == "vs2005" ) and options["no-builtin-wchar"] ) then
	table.insert(package.buildoptions, "/Zc:wchar_t-")
end

-- Set the files to include.
package.files = { matchrecursive( "*.cpp", "*.h", "*.fbp" ) }

-- Set the include paths.
package.includepaths = { "../../include" }

-- Set the libraries it links to.
package.links = { "wxXmlSerializer" }

-- Setup the output directory options.
--		Note: Use 'libdir' for "lib" kind only.

if ( ( target == "vs2003" ) or ( target == "vs2005" ) ) then
	package.bindir = "bin/vc"
else
	package.bindir = "bin/gcc"
end

dofile('../../build/premake/scripts/wxpresets.lua')

