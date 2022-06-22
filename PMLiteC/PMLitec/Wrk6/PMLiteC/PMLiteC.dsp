# Microsoft Developer Studio Project File - Name="PMLiteC" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=PMLiteC - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PMLiteC.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PMLiteC.mak" CFG="PMLiteC - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PMLiteC - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "PMLiteC - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PMLiteC - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\..\..\PMLiteC\Common" /I "..\..\..\PMLiteC\WinNT" /I "..\..\..\PMLiteC\Common\Stream" /I ".." /I "..\..\WinNT" /I "..\..\..\PMLiteC\Common\Serial" /I "..\..\..\PMLiteC\Common\Socket" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "PMLiteC - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\..\..\PMLiteC\Common" /I "..\..\..\PMLiteC\WinNT" /I "..\..\..\PMLiteC\Common\Stream" /I ".." /I "..\..\WinNT" /I "..\..\..\PMLiteC\Common\Serial" /I "..\..\..\PMLiteC\Common\Socket" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "PMLiteC - Win32 Release"
# Name "PMLiteC - Win32 Debug"
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Group "Stream"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Stream\cB64Dec.c
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Stream\cB64Dec.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Stream\cB64Enc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Stream\cB64Enc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Stream\cBaseSt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Stream\cBaseSt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Stream\cConcSt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Stream\cConcSt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Stream\cCountSt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Stream\cCountSt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Stream\cCRLFSt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Stream\cCRLFSt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Stream\cDotSt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Stream\cDotSt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Stream\cEndPt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Stream\cEndPt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Stream\cFileSt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Stream\cFileSt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Stream\cGenSt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Stream\cGenSt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Stream\cMemSt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Stream\cMemSt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Stream\cPartSt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Stream\cPartSt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Stream\cQPDec.c
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Stream\cQPDec.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Stream\cQPEnc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Stream\cQPEnc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Stream\cSerSt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Stream\cSerSt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Stream\cSockSt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Stream\cSockSt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Stream\cStopSt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Stream\cStopSt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Stream\cStream.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Stream\cUndotSt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Stream\cUndotSt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Stream\cUnfSt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Stream\cUnfSt.h
# End Source File
# End Group
# Begin Group "Socket"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Socket\cEnvPPP.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Socket\cEnvSock.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Socket\cPPP.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Socket\cPPPDef.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Socket\cSockDef.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Socket\cSocket.h
# End Source File
# End Group
# Begin Group "Serial"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Serial\cSerial.c
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\Serial\cSerial.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\cCore.c
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\cCore.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\cMemory.c
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\cMemory.h
# End Source File
# Begin Source File

SOURCE=..\..\Common\cTrace.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\cTypes.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\cXBorder.c
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\cXBorder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\cXCore.c
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\cXCore.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\cXDLList.c
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\cXDLList.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\cXMem.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\cXMemDbg.c
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\cXStack.c
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\cXStack.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\cXTrace.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\cXTypes.h
# End Source File
# Begin Source File

SOURCE=..\..\Common\pmDebug.h
# End Source File
# Begin Source File

SOURCE=..\..\Common\pmTrace.h
# End Source File
# Begin Source File

SOURCE=..\..\Common\pmXDebug.h
# End Source File
# Begin Source File

SOURCE=..\..\Common\pmXError.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\Common\pmXTrace.h
# End Source File
# End Group
# Begin Group "WinNT"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\PMLiteC\WinNT\ASTrace.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\WinNT\cConsole.c
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\WinNT\cConsole.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\WinNT\cEnv.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\WinNT\cInput.c
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\WinNT\cInput.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\WinNT\cMenu.c
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\WinNT\cMenu.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\WinNT\cMenuDis.c
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\WinNT\cMenuDis.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\WinNT\cStdArg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\WinNT\cXCoreP.c
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\WinNT\cXEnv.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\WinNT\cXStackP.c
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\WinNT\cXThread.c
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\WinNT\cXThread.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\WinNT\cXTrace.c
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\WinNT\OSWinInc.h
# End Source File
# Begin Source File

SOURCE=..\..\WinNT\pmDebug.c
# End Source File
# Begin Source File

SOURCE=..\..\WinNT\pmTrace.c
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\WinNT\pmTypes.h
# End Source File
# Begin Source File

SOURCE=..\..\WinNT\pmXDebug.c
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\WinNT\pmXEnv.h
# End Source File
# Begin Source File

SOURCE=..\..\WinNT\pmXError.c
# End Source File
# Begin Source File

SOURCE=..\..\..\PMLiteC\WinNT\pmXTrace.c
# End Source File
# End Group
# End Target
# End Project
