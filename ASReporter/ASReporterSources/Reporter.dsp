# Microsoft Developer Studio Project File - Name="Reporter" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Reporter - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Reporter.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Reporter.mak" CFG="Reporter - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Reporter - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Reporter - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/ObjectSet/Win32/Reporter", ZFUAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Reporter - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "." /I "OS" /I "OV" /I "Res" /I "Res\Palm" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Reporter - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "." /I "OS" /I "OV" /I "Res" /I "Res\Palm" /D "_WINDOWS" /D "_AFXDLL" /D "WIN32" /D "_DEBUG" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Reporter - Win32 Release"
# Name "Reporter - Win32 Debug"
# Begin Group "ObjectSet"

# PROP Default_Filter ""
# Begin Group "Include Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\OS\PMANSIFM.h
# End Source File
# Begin Source File

SOURCE=.\OS\PMArray.h
# End Source File
# Begin Source File

SOURCE=.\OS\PMAstExc.h
# End Source File
# Begin Source File

SOURCE=.\OS\PMBuffer.h
# End Source File
# Begin Source File

SOURCE=.\OS\PMCharS.h
# End Source File
# Begin Source File

SOURCE=.\OS\PMCriSec.h
# End Source File
# Begin Source File

SOURCE=.\OS\PMData.h
# End Source File
# Begin Source File

SOURCE=.\OS\PMDebug.h
# End Source File
# Begin Source File

SOURCE=.\OS\PMDefs.h
# End Source File
# Begin Source File

SOURCE=.\OS\PMDefVC4.h
# End Source File
# Begin Source File

SOURCE=.\OS\PMError.h
# End Source File
# Begin Source File

SOURCE=.\OS\PMFileTr.h
# End Source File
# Begin Source File

SOURCE=.\OS\PMHTbl.h
# End Source File
# Begin Source File

SOURCE=.\OS\PMMedia.h
# End Source File
# Begin Source File

SOURCE=.\OS\PMMemSt.h
# End Source File
# Begin Source File

SOURCE=.\OS\PMMemVC4.h
# End Source File
# Begin Source File

SOURCE=.\OS\PMMoniCB.h
# End Source File
# Begin Source File

SOURCE=.\OS\PMMonit.h
# End Source File
# Begin Source File

SOURCE=.\OS\PMOSHelp.h
# End Source File
# Begin Source File

SOURCE=.\OS\PMPrintf.h
# End Source File
# Begin Source File

SOURCE=.\OS\PMPushB.h
# End Source File
# Begin Source File

SOURCE=.\OS\PMRC.h
# End Source File
# Begin Source File

SOURCE=.\OS\PMScan.h
# End Source File
# Begin Source File

SOURCE=.\OS\PMSocket.h
# End Source File
# Begin Source File

SOURCE=.\OS\PMStr.h
# End Source File
# Begin Source File

SOURCE=.\OS\PMStream.h
# End Source File
# Begin Source File

SOURCE=.\OS\PMStub.h
# End Source File
# Begin Source File

SOURCE=.\OS\PMSVC4.h
# End Source File
# Begin Source File

SOURCE=.\OS\PMTCPSoc.h
# End Source File
# Begin Source File

SOURCE=.\OS\PMTCPTr.h
# End Source File
# Begin Source File

SOURCE=.\OS\PMTLS.h
# End Source File
# Begin Source File

SOURCE=.\OS\PMTrace.h
# End Source File
# Begin Source File

SOURCE=.\OS\PMTypes.h
# End Source File
# Begin Source File

SOURCE=.\OS\PMW32Hlp.h
# End Source File
# Begin Source File

SOURCE=.\OS\PMWErr.h
# End Source File
# Begin Source File

SOURCE=.\OS\PMWFilTr.h
# End Source File
# Begin Source File

SOURCE=.\OS\PMWinInc.h
# End Source File
# Begin Source File

SOURCE=.\OS\PMWSoc11.h
# End Source File
# Begin Source File

SOURCE=.\OS\PMWTCPTr.h
# End Source File
# Begin Source File

SOURCE=.\OS\PMXTrace.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\OS\PMANSIFM.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OS\PMArray.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OS\PMBuffer.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OS\PMCharS.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OS\PMCriSec.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OS\PMData.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OS\PMDebug.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OS\PMError.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OS\PMFileTr.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OS\PMHTbl.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OS\PMMedia.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OS\PMMemVC4.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OS\PMMoniCB.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OS\PMMonit.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OS\PMOSHelp.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OS\PMPrintf.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OS\PMPushB.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OS\PMRC.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OS\PMScan.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OS\PMSocket.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OS\PMStr.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OS\PMStream.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OS\PMStub.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OS\PMSVC4.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OS\PMTCPSoc.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OS\PMTLS.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OS\PMTrace.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OS\PMW32Hlp.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OS\PMWFilTr.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OS\PMWSoc11.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OS\PMWTCPTr.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OS\PMXTrace.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# End Group
# Begin Group "ObjectSet Visual"

# PROP Default_Filter ""
# Begin Group "Include Files No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\OV\LBitmap.h
# End Source File
# Begin Source File

SOURCE=.\OV\LButton.h
# End Source File
# Begin Source File

SOURCE=.\OV\LContain.h
# End Source File
# Begin Source File

SOURCE=.\OV\LControl.h
# End Source File
# Begin Source File

SOURCE=.\OV\LDefines.h
# End Source File
# Begin Source File

SOURCE=.\OV\LDialog.h
# End Source File
# Begin Source File

SOURCE=.\OV\LDoc.h
# End Source File
# Begin Source File

SOURCE=.\OV\LFlatBar.h
# End Source File
# Begin Source File

SOURCE=.\OV\LHLine.h
# End Source File
# Begin Source File

SOURCE=.\OV\LImgList.h
# End Source File
# Begin Source File

SOURCE=.\OV\LMSWnd.h
# End Source File
# Begin Source File

SOURCE=.\OV\LMultDoc.h
# End Source File
# Begin Source File

SOURCE=.\OV\LOVReg.h
# End Source File
# Begin Source File

SOURCE=.\OV\LOVUtils.h
# End Source File
# Begin Source File

SOURCE=.\OV\LPPage.h
# End Source File
# Begin Source File

SOURCE=.\OV\LSizable.h
# End Source File
# Begin Source File

SOURCE=.\OV\LStatic.h
# End Source File
# Begin Source File

SOURCE=.\OV\LTab.h
# End Source File
# Begin Source File

SOURCE=.\OV\LTipInfo.h
# End Source File
# Begin Source File

SOURCE=.\OV\LURL.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\OV\LBitmap.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OV\LButton.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OV\LContain.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OV\LControl.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OV\LDialog.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OV\LDoc.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OV\LFlatBar.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OV\LHLine.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OV\LImgList.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OV\LMDICWnd.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OV\LMSWnd.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OV\LMultDoc.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OV\LOVReg.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OV\LOVUtils.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OV\LPPage.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OV\LSizable.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OV\LStatic.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OV\LTab.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OV\LTipInfo.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\OV\LURL.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# End Group
# Begin Group "Reporter"

# PROP Default_Filter ""
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CAboutDlg.h
# End Source File
# Begin Source File

SOURCE=.\CChildFrame.h
# End Source File
# Begin Source File

SOURCE=.\CFiltersDlg.h
# End Source File
# Begin Source File

SOURCE=.\CFiltersPP.h
# End Source File
# Begin Source File

SOURCE=.\CGotoDlg.h
# End Source File
# Begin Source File

SOURCE=.\CMainFrm.h
# End Source File
# Begin Source File

SOURCE=.\COSReportDoc.h
# End Source File
# Begin Source File

SOURCE=.\COSReportView.h
# End Source File
# Begin Source File

SOURCE=.\COSRThreads.h
# End Source File
# Begin Source File

SOURCE=.\CTipDlg.h
# End Source File
# Begin Source File

SOURCE=.\CTrGroups.h
# End Source File
# Begin Source File

SOURCE=.\OSReport.h
# End Source File
# Begin Source File

SOURCE=.\Res\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Res\ASTarget.rc
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\res\litebulb.bmp
# End Source File
# Begin Source File

SOURCE=.\res\lurl.cur
# End Source File
# Begin Source File

SOURCE=.\Res\ReportDoc.ico
# End Source File
# Begin Source File

SOURCE=.\Res\Reporter.ico
# End Source File
# Begin Source File

SOURCE=.\Res\Reporter.rc

!IF  "$(CFG)" == "Reporter - Win32 Release"

!ELSEIF  "$(CFG)" == "Reporter - Win32 Debug"

# ADD BASE RSC /l 0x409 /i "Res"
# ADD RSC /l 0x409 /i "Res" /d REP=Palm

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Res\Reporter.rc2
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\CAboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CChildFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\CFiltersDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CFiltersPP.cpp
# End Source File
# Begin Source File

SOURCE=.\CGotoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CMainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\COSReportDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\COSReportView.cpp
# End Source File
# Begin Source File

SOURCE=.\COSRThreads.cpp
# End Source File
# Begin Source File

SOURCE=.\CTipDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CTrGroups.cpp
# End Source File
# Begin Source File

SOURCE=.\OSReport.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Source File

SOURCE=.\Version.lib
# End Source File
# End Target
# End Project
