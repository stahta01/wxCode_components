# Microsoft Developer Studio Project File - Name="rarextract" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=rarextract - Win32 Debug ENDIAN_big
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "avm_rarextract.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "avm_rarextract.mak" CFG="rarextract - Win32 Debug ENDIAN_big"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "rarextract - Win32 DLL Unicode Release ENDIAN_ little" (based on "Win32 (x86) Application")
!MESSAGE "rarextract - Win32 DLL Unicode Release ENDIAN_big" (based on "Win32 (x86) Application")
!MESSAGE "rarextract - Win32 DLL Unicode Debug ENDIAN_ little" (based on "Win32 (x86) Application")
!MESSAGE "rarextract - Win32 DLL Unicode Debug ENDIAN_big" (based on "Win32 (x86) Application")
!MESSAGE "rarextract - Win32 DLL Release ENDIAN_ little" (based on "Win32 (x86) Application")
!MESSAGE "rarextract - Win32 DLL Release ENDIAN_big" (based on "Win32 (x86) Application")
!MESSAGE "rarextract - Win32 DLL Debug ENDIAN_ little" (based on "Win32 (x86) Application")
!MESSAGE "rarextract - Win32 DLL Debug ENDIAN_big" (based on "Win32 (x86) Application")
!MESSAGE "rarextract - Win32 Unicode Release ENDIAN_ little" (based on "Win32 (x86) Application")
!MESSAGE "rarextract - Win32 Unicode Release ENDIAN_big" (based on "Win32 (x86) Application")
!MESSAGE "rarextract - Win32 Unicode Debug ENDIAN_ little" (based on "Win32 (x86) Application")
!MESSAGE "rarextract - Win32 Unicode Debug ENDIAN_big" (based on "Win32 (x86) Application")
!MESSAGE "rarextract - Win32 Release ENDIAN_ little" (based on "Win32 (x86) Application")
!MESSAGE "rarextract - Win32 Release ENDIAN_big" (based on "Win32 (x86) Application")
!MESSAGE "rarextract - Win32 Debug ENDIAN_ little" (based on "Win32 (x86) Application")
!MESSAGE "rarextract - Win32 Debug ENDIAN_big" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "rarextract - Win32 DLL Unicode Release ENDIAN_ little"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\bin"
# PROP BASE Intermediate_Dir "Release\rarextract"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\bin"
# PROP Intermediate_Dir "Release\rarextract"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /GR /GX /O1 /I "..\..\include" /D "WIN32" /D "_UNICODE" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_WINDOWS" /Fd"..\..\bin\rarextract.pdb" /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /O1 /I "..\..\include" /D "WIN32" /D "_UNICODE" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_WINDOWS" /Fd"..\..\bin\rarextract.pdb" /FD /c
# ADD BASE MTL /nologo /D "WIN32" /D "_UNICODE" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_UNICODE" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /i "..\..\include" /d "_UNICODE" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d "SILENT" /d "_WINDOWS"
# ADD RSC /l 0x405 /i "..\..\include" /d "_UNICODE" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d "SILENT" /d "_WINDOWS"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw25u_core.lib wxbase25u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /subsystem:windows /machine:I386 /out:"..\..\bin\rarextract.exe"
# ADD LINK32 wxmsw25u_core.lib wxbase25u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /subsystem:windows /machine:I386 /out:"..\..\bin\rarextract.exe"

!ELSEIF  "$(CFG)" == "rarextract - Win32 DLL Unicode Release ENDIAN_big"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\bin"
# PROP BASE Intermediate_Dir "Release\rarextract"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\bin"
# PROP Intermediate_Dir "Release\rarextract"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /GR /GX /O1 /I "..\..\include" /D "WIN32" /D "_UNICODE" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_WINDOWS" /Fd"..\..\bin\rarextract.pdb" /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /O1 /I "..\..\include" /D "WIN32" /D "_UNICODE" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_WINDOWS" /Fd"..\..\bin\rarextract.pdb" /FD /c
# ADD BASE MTL /nologo /D "WIN32" /D "_UNICODE" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_UNICODE" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /i "..\..\include" /d "_UNICODE" /d "BIG_ENDIAN" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d "SILENT" /d "_WINDOWS"
# ADD RSC /l 0x405 /i "..\..\include" /d "_UNICODE" /d "BIG_ENDIAN" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d "SILENT" /d "_WINDOWS"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw25u_core.lib wxbase25u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /subsystem:windows /machine:I386 /out:"..\..\bin\rarextract.exe"
# ADD LINK32 wxmsw25u_core.lib wxbase25u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /subsystem:windows /machine:I386 /out:"..\..\bin\rarextract.exe"

!ELSEIF  "$(CFG)" == "rarextract - Win32 DLL Unicode Debug ENDIAN_ little"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\bin"
# PROP BASE Intermediate_Dir "Debug\rarextract"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\bin"
# PROP Intermediate_Dir "Debug\rarextract"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /I "..\..\include" /D "WIN32" /D "_UNICODE" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /D "_WINDOWS" /Fd"..\..\bin\rarextract.pdb" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /I "..\..\include" /D "WIN32" /D "_UNICODE" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /D "_WINDOWS" /Fd"..\..\bin\rarextract.pdb" /FD /GZ /c
# ADD BASE MTL /nologo /D "WIN32" /D "_UNICODE" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_UNICODE" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /i "..\..\include" /d "_UNICODE" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d "SILENT" /d "_DEBUG" /d "_WINDOWS"
# ADD RSC /l 0x405 /i "..\..\include" /d "_UNICODE" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d "SILENT" /d "_DEBUG" /d "_WINDOWS"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw25ud_core.lib wxbase25ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\bin\rarextract.exe"
# ADD LINK32 wxmsw25ud_core.lib wxbase25ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\bin\rarextract.exe"

!ELSEIF  "$(CFG)" == "rarextract - Win32 DLL Unicode Debug ENDIAN_big"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\bin"
# PROP BASE Intermediate_Dir "Debug\rarextract"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\bin"
# PROP Intermediate_Dir "Debug\rarextract"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /I "..\..\include" /D "WIN32" /D "_UNICODE" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /D "_WINDOWS" /Fd"..\..\bin\rarextract.pdb" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /I "..\..\include" /D "WIN32" /D "_UNICODE" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /D "_WINDOWS" /Fd"..\..\bin\rarextract.pdb" /FD /GZ /c
# ADD BASE MTL /nologo /D "WIN32" /D "_UNICODE" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_UNICODE" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /i "..\..\include" /d "_UNICODE" /d "BIG_ENDIAN" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d "SILENT" /d "_DEBUG" /d "_WINDOWS"
# ADD RSC /l 0x405 /i "..\..\include" /d "_UNICODE" /d "BIG_ENDIAN" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d "SILENT" /d "_DEBUG" /d "_WINDOWS"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw25ud_core.lib wxbase25ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\bin\rarextract.exe"
# ADD LINK32 wxmsw25ud_core.lib wxbase25ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\bin\rarextract.exe"

!ELSEIF  "$(CFG)" == "rarextract - Win32 DLL Release ENDIAN_ little"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\bin"
# PROP BASE Intermediate_Dir "Release\rarextract"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\bin"
# PROP Intermediate_Dir "Release\rarextract"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /GR /GX /O1 /I "..\..\include" /D "WIN32" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_WINDOWS" /Fd"..\..\bin\rarextract.pdb" /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /O1 /I "..\..\include" /D "WIN32" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_WINDOWS" /Fd"..\..\bin\rarextract.pdb" /FD /c
# ADD BASE MTL /nologo /D "WIN32" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /i "..\..\include" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d "SILENT" /d "_WINDOWS"
# ADD RSC /l 0x405 /i "..\..\include" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d "SILENT" /d "_WINDOWS"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw25_core.lib wxbase25.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /subsystem:windows /machine:I386 /out:"..\..\bin\rarextract.exe"
# ADD LINK32 wxmsw25_core.lib wxbase25.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /subsystem:windows /machine:I386 /out:"..\..\bin\rarextract.exe"

!ELSEIF  "$(CFG)" == "rarextract - Win32 DLL Release ENDIAN_big"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\bin"
# PROP BASE Intermediate_Dir "Release\rarextract"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\bin"
# PROP Intermediate_Dir "Release\rarextract"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /GR /GX /O1 /I "..\..\include" /D "WIN32" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_WINDOWS" /Fd"..\..\bin\rarextract.pdb" /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /O1 /I "..\..\include" /D "WIN32" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_WINDOWS" /Fd"..\..\bin\rarextract.pdb" /FD /c
# ADD BASE MTL /nologo /D "WIN32" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /i "..\..\include" /d "BIG_ENDIAN" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d "SILENT" /d "_WINDOWS"
# ADD RSC /l 0x405 /i "..\..\include" /d "BIG_ENDIAN" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d "SILENT" /d "_WINDOWS"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw25_core.lib wxbase25.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /subsystem:windows /machine:I386 /out:"..\..\bin\rarextract.exe"
# ADD LINK32 wxmsw25_core.lib wxbase25.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /subsystem:windows /machine:I386 /out:"..\..\bin\rarextract.exe"

!ELSEIF  "$(CFG)" == "rarextract - Win32 DLL Debug ENDIAN_ little"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\bin"
# PROP BASE Intermediate_Dir "Debug\rarextract"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\bin"
# PROP Intermediate_Dir "Debug\rarextract"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /I "..\..\include" /D "WIN32" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /D "_WINDOWS" /Fd"..\..\bin\rarextract.pdb" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /I "..\..\include" /D "WIN32" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /D "_WINDOWS" /Fd"..\..\bin\rarextract.pdb" /FD /GZ /c
# ADD BASE MTL /nologo /D "WIN32" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /i "..\..\include" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d "SILENT" /d "_DEBUG" /d "_WINDOWS"
# ADD RSC /l 0x405 /i "..\..\include" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d "SILENT" /d "_DEBUG" /d "_WINDOWS"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw25d_core.lib wxbase25d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\bin\rarextract.exe"
# ADD LINK32 wxmsw25d_core.lib wxbase25d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\bin\rarextract.exe"

!ELSEIF  "$(CFG)" == "rarextract - Win32 DLL Debug ENDIAN_big"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\bin"
# PROP BASE Intermediate_Dir "Debug\rarextract"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\bin"
# PROP Intermediate_Dir "Debug\rarextract"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /I "..\..\include" /D "WIN32" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /D "_WINDOWS" /Fd"..\..\bin\rarextract.pdb" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /I "..\..\include" /D "WIN32" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /D "_WINDOWS" /Fd"..\..\bin\rarextract.pdb" /FD /GZ /c
# ADD BASE MTL /nologo /D "WIN32" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /i "..\..\include" /d "BIG_ENDIAN" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d "SILENT" /d "_DEBUG" /d "_WINDOWS"
# ADD RSC /l 0x405 /i "..\..\include" /d "BIG_ENDIAN" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d "SILENT" /d "_DEBUG" /d "_WINDOWS"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw25d_core.lib wxbase25d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\bin\rarextract.exe"
# ADD LINK32 wxmsw25d_core.lib wxbase25d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\bin\rarextract.exe"

!ELSEIF  "$(CFG)" == "rarextract - Win32 Unicode Release ENDIAN_ little"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\bin"
# PROP BASE Intermediate_Dir "Release\rarextract"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\bin"
# PROP Intermediate_Dir "Release\rarextract"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /GR /GX /O1 /I "..\..\include" /D "WIN32" /D "_UNICODE" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_WINDOWS" /Fd"..\..\bin\rarextract.pdb" /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /O1 /I "..\..\include" /D "WIN32" /D "_UNICODE" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_WINDOWS" /Fd"..\..\bin\rarextract.pdb" /FD /c
# ADD BASE MTL /nologo /D "WIN32" /D "_UNICODE" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_UNICODE" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /i "..\..\include" /d "_UNICODE" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d "SILENT" /d "_WINDOWS"
# ADD RSC /l 0x405 /i "..\..\include" /d "_UNICODE" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d "SILENT" /d "_WINDOWS"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw25u_core.lib wxbase25u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /subsystem:windows /machine:I386 /out:"..\..\bin\rarextract.exe"
# ADD LINK32 wxmsw25u_core.lib wxbase25u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /subsystem:windows /machine:I386 /out:"..\..\bin\rarextract.exe"

!ELSEIF  "$(CFG)" == "rarextract - Win32 Unicode Release ENDIAN_big"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\bin"
# PROP BASE Intermediate_Dir "Release\rarextract"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\bin"
# PROP Intermediate_Dir "Release\rarextract"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /GR /GX /O1 /I "..\..\include" /D "WIN32" /D "_UNICODE" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_WINDOWS" /Fd"..\..\bin\rarextract.pdb" /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /O1 /I "..\..\include" /D "WIN32" /D "_UNICODE" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_WINDOWS" /Fd"..\..\bin\rarextract.pdb" /FD /c
# ADD BASE MTL /nologo /D "WIN32" /D "_UNICODE" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_UNICODE" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /i "..\..\include" /d "_UNICODE" /d "BIG_ENDIAN" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d "SILENT" /d "_WINDOWS"
# ADD RSC /l 0x405 /i "..\..\include" /d "_UNICODE" /d "BIG_ENDIAN" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d "SILENT" /d "_WINDOWS"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw25u_core.lib wxbase25u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /subsystem:windows /machine:I386 /out:"..\..\bin\rarextract.exe"
# ADD LINK32 wxmsw25u_core.lib wxbase25u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /subsystem:windows /machine:I386 /out:"..\..\bin\rarextract.exe"

!ELSEIF  "$(CFG)" == "rarextract - Win32 Unicode Debug ENDIAN_ little"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\bin"
# PROP BASE Intermediate_Dir "Debug\rarextract"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\bin"
# PROP Intermediate_Dir "Debug\rarextract"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /I "..\..\include" /D "WIN32" /D "_UNICODE" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /D "_WINDOWS" /Fd"..\..\bin\rarextract.pdb" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /I "..\..\include" /D "WIN32" /D "_UNICODE" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /D "_WINDOWS" /Fd"..\..\bin\rarextract.pdb" /FD /GZ /c
# ADD BASE MTL /nologo /D "WIN32" /D "_UNICODE" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_UNICODE" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /i "..\..\include" /d "_UNICODE" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d "SILENT" /d "_DEBUG" /d "_WINDOWS"
# ADD RSC /l 0x405 /i "..\..\include" /d "_UNICODE" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d "SILENT" /d "_DEBUG" /d "_WINDOWS"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw25ud_core.lib wxbase25ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\bin\rarextract.exe"
# ADD LINK32 wxmsw25ud_core.lib wxbase25ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\bin\rarextract.exe"

!ELSEIF  "$(CFG)" == "rarextract - Win32 Unicode Debug ENDIAN_big"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\bin"
# PROP BASE Intermediate_Dir "Debug\rarextract"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\bin"
# PROP Intermediate_Dir "Debug\rarextract"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /I "..\..\include" /D "WIN32" /D "_UNICODE" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /D "_WINDOWS" /Fd"..\..\bin\rarextract.pdb" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /I "..\..\include" /D "WIN32" /D "_UNICODE" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /D "_WINDOWS" /Fd"..\..\bin\rarextract.pdb" /FD /GZ /c
# ADD BASE MTL /nologo /D "WIN32" /D "_UNICODE" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_UNICODE" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /i "..\..\include" /d "_UNICODE" /d "BIG_ENDIAN" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d "SILENT" /d "_DEBUG" /d "_WINDOWS"
# ADD RSC /l 0x405 /i "..\..\include" /d "_UNICODE" /d "BIG_ENDIAN" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d "SILENT" /d "_DEBUG" /d "_WINDOWS"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw25ud_core.lib wxbase25ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\bin\rarextract.exe"
# ADD LINK32 wxmsw25ud_core.lib wxbase25ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\bin\rarextract.exe"

!ELSEIF  "$(CFG)" == "rarextract - Win32 Release ENDIAN_ little"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\bin"
# PROP BASE Intermediate_Dir "Release\rarextract"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\bin"
# PROP Intermediate_Dir "Release\rarextract"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /GR /GX /O1 /I "..\..\include" /D "WIN32" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_WINDOWS" /Fd"..\..\bin\rarextract.pdb" /FD /c
# ADD CPP /nologo /MT /W4 /GR /GX /O1 /I "..\..\include" /D "WIN32" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_WINDOWS" /Fd"..\..\bin\rarextract.pdb" /FD /c
# ADD BASE MTL /nologo /D "WIN32" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /i "..\..\include" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d "SILENT" /d "_WINDOWS"
# ADD RSC /l 0x405 /i "..\..\include" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d "SILENT" /d "_WINDOWS"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw25_core.lib wxbase25.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /subsystem:windows /machine:I386 /out:"..\..\bin\rarextract.exe"
# ADD LINK32 wxmsw25_core.lib wxbase25.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /subsystem:windows /machine:I386 /out:"..\..\bin\rarextract.exe"

!ELSEIF  "$(CFG)" == "rarextract - Win32 Release ENDIAN_big"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\bin"
# PROP BASE Intermediate_Dir "Release\rarextract"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\bin"
# PROP Intermediate_Dir "Release\rarextract"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /GR /GX /O1 /I "..\..\include" /D "WIN32" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_WINDOWS" /Fd"..\..\bin\rarextract.pdb" /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /O1 /I "..\..\include" /D "WIN32" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_WINDOWS" /Fd"..\..\bin\rarextract.pdb" /FD /c
# ADD BASE MTL /nologo /D "WIN32" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /i "..\..\include" /d "BIG_ENDIAN" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d "SILENT" /d "_WINDOWS"
# ADD RSC /l 0x405 /i "..\..\include" /d "BIG_ENDIAN" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d "SILENT" /d "_WINDOWS"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw25_core.lib wxbase25.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /subsystem:windows /machine:I386 /out:"..\..\bin\rarextract.exe"
# ADD LINK32 wxmsw25_core.lib wxbase25.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /subsystem:windows /machine:I386 /out:"..\..\bin\rarextract.exe"

!ELSEIF  "$(CFG)" == "rarextract - Win32 Debug ENDIAN_ little"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\bin"
# PROP BASE Intermediate_Dir "Debug\rarextract"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\bin"
# PROP Intermediate_Dir "Debug\rarextract"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /I "..\..\include" /D "WIN32" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /D "_WINDOWS" /Fd"..\..\bin\rarextract.pdb" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /I "..\..\include" /D "WIN32" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /D "_WINDOWS" /Fd"..\..\bin\rarextract.pdb" /FD /GZ /c
# ADD BASE MTL /nologo /D "WIN32" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /i "..\..\include" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d "SILENT" /d "_DEBUG" /d "_WINDOWS"
# ADD RSC /l 0x405 /i "..\..\include" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d "SILENT" /d "_DEBUG" /d "_WINDOWS"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw25d_core.lib wxbase25d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\bin\rarextract.exe"
# ADD LINK32 wxmsw25d_core.lib wxbase25d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\bin\rarextract.exe"

!ELSEIF  "$(CFG)" == "rarextract - Win32 Debug ENDIAN_big"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\bin"
# PROP BASE Intermediate_Dir "Debug\rarextract"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\bin"
# PROP Intermediate_Dir "Debug\rarextract"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /I "..\..\include" /D "WIN32" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /D "_WINDOWS" /Fd"..\..\bin\rarextract.pdb" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /I "..\..\include" /D "WIN32" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /D "_WINDOWS" /Fd"..\..\bin\rarextract.pdb" /FD /GZ /c
# ADD BASE MTL /nologo /D "WIN32" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "BIG_ENDIAN" /D FILE_OFFSET_BITS=64 /D "_LARGEFILE_SOURCE" /D "RARDLL" /D "SILENT" /D "_DEBUG" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /i "..\..\include" /d "BIG_ENDIAN" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d "SILENT" /d "_DEBUG" /d "_WINDOWS"
# ADD RSC /l 0x405 /i "..\..\include" /d "BIG_ENDIAN" /d FILE_OFFSET_BITS=64 /d "_LARGEFILE_SOURCE" /d "RARDLL" /d "SILENT" /d "_DEBUG" /d "_WINDOWS"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw25d_core.lib wxbase25d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\bin\rarextract.exe"
# ADD LINK32 wxmsw25d_core.lib wxbase25d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\bin\rarextract.exe"

!ENDIF 

# Begin Target

# Name "rarextract - Win32 DLL Unicode Release ENDIAN_ little"
# Name "rarextract - Win32 DLL Unicode Release ENDIAN_big"
# Name "rarextract - Win32 DLL Unicode Debug ENDIAN_ little"
# Name "rarextract - Win32 DLL Unicode Debug ENDIAN_big"
# Name "rarextract - Win32 DLL Release ENDIAN_ little"
# Name "rarextract - Win32 DLL Release ENDIAN_big"
# Name "rarextract - Win32 DLL Debug ENDIAN_ little"
# Name "rarextract - Win32 DLL Debug ENDIAN_big"
# Name "rarextract - Win32 Unicode Release ENDIAN_ little"
# Name "rarextract - Win32 Unicode Release ENDIAN_big"
# Name "rarextract - Win32 Unicode Debug ENDIAN_ little"
# Name "rarextract - Win32 Unicode Debug ENDIAN_big"
# Name "rarextract - Win32 Release ENDIAN_ little"
# Name "rarextract - Win32 Release ENDIAN_big"
# Name "rarextract - Win32 Debug ENDIAN_ little"
# Name "rarextract - Win32 Debug ENDIAN_big"
# Begin Group "Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\..\..\samples\rarextract\rarextract.cpp
# End Source File
# End Group
# End Target
# End Project
