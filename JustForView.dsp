# Microsoft Developer Studio Project File - Name="JustForView" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=JustForView - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "JustForView.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "JustForView.mak" CFG="JustForView - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "JustForView - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "JustForView - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "JustForView - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f JustForView.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "JustForView.exe"
# PROP BASE Bsc_Name "JustForView.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f JustForView.mak"
# PROP Rebuild_Opt "/a"
# PROP Target_File "JustForView.exe"
# PROP Bsc_Name "JustForView.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "JustForView - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f JustForView.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "JustForView.exe"
# PROP BASE Bsc_Name "JustForView.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "nmake /f "JustForView.mak""
# PROP Rebuild_Opt "/a"
# PROP Target_File "JustForView.exe"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "JustForView - Win32 Release"
# Name "JustForView - Win32 Debug"

!IF  "$(CFG)" == "JustForView - Win32 Release"

!ELSEIF  "$(CFG)" == "JustForView - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "api"

# PROP Default_Filter ""
# Begin Group "DW_Frontend"

# PROP Default_Filter ""
# Begin Group "R850"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\api\DW_Frontend\R850\R850.c
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\R850\R850.h
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\R850\R850_DataDefine.h
# End Source File
# End Group
# Begin Group "RT710"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\api\DW_Frontend\RT710\RT710.c
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\RT710\RT710.h
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\RT710\RT710_DataDefine.h
# End Source File
# End Group
# Begin Group "TC90522"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\api\DW_Frontend\TC90522\TC90522.c
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\TC90522\TC90522.h
# End Source File
# End Group
# Begin Group "CXD2856"

# PROP Default_Filter ""
# Begin Group "dvb_cable"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\dvb_cable\sony_demod_dvbc.c
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\dvb_cable\sony_demod_dvbc.h
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\dvb_cable\sony_demod_dvbc2.c
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\dvb_cable\sony_demod_dvbc2.h
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\dvb_cable\sony_demod_dvbc2_monitor.c
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\dvb_cable\sony_demod_dvbc2_monitor.h
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\dvb_cable\sony_demod_dvbc_monitor.c
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\dvb_cable\sony_demod_dvbc_monitor.h
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\dvb_cable\sony_dvbc.h
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\dvb_cable\sony_dvbc2.h
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\dvb_cable\sony_integ_dvbc.c
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\dvb_cable\sony_integ_dvbc.h
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\dvb_cable\sony_integ_dvbc2.c
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\dvb_cable\sony_integ_dvbc2.h
# End Source File
# End Group
# Begin Group "dvb_terr"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\dvb_terr\sony_demod_dvbt.c
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\dvb_terr\sony_demod_dvbt.h
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\dvb_terr\sony_demod_dvbt2.c
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\dvb_terr\sony_demod_dvbt2.h
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\dvb_terr\sony_demod_dvbt2_monitor.c
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\dvb_terr\sony_demod_dvbt2_monitor.h
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\dvb_terr\sony_demod_dvbt_monitor.c
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\dvb_terr\sony_demod_dvbt_monitor.h
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\dvb_terr\sony_dvbt.h
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\dvb_terr\sony_dvbt2.h
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\dvb_terr\sony_integ_dvbt.c
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\dvb_terr\sony_integ_dvbt.h
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\dvb_terr\sony_integ_dvbt2.c
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\dvb_terr\sony_integ_dvbt2.h
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\dvb_terr\sony_integ_dvbt_t2.c
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\dvb_terr\sony_integ_dvbt_t2.h
# End Source File
# End Group
# Begin Group "i2c"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\i2c\sony_i2c.c
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\i2c\sony_i2c.h
# End Source File
# End Group
# Begin Group "isdb_terr"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\isdb_terr\sony_demod_isdbt.c
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\isdb_terr\sony_demod_isdbt.h
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\isdb_terr\sony_demod_isdbt_monitor.c
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\isdb_terr\sony_demod_isdbt_monitor.h
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\isdb_terr\sony_integ_isdbt.c
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\isdb_terr\sony_integ_isdbt.h
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\isdb_terr\sony_isdbt.h
# End Source File
# End Group
# Begin Group "lib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\lib\sony_math.c
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\lib\sony_math.h
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\lib\sony_stdlib.h
# End Source File
# End Group
# Begin Group "tuner"

# PROP Default_Filter ""
# Begin Group "terr_cable_sat_helene"

# PROP Default_Filter ""
# Begin Group "refcode"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\tuner\terr_cable_sat_helene\refcode\sony_helene.c
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\tuner\terr_cable_sat_helene\refcode\sony_helene.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\tuner\terr_cable_sat_helene\sony_tuner_helene.c
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\tuner\terr_cable_sat_helene\sony_tuner_helene.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\tuner\sony_tuner_sat.h
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\tuner\sony_tuner_terr_cable.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\CXD2856.c
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\CXD2856.h
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\CXD2856_DataDefine.h
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\sony_common.c
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\sony_common.h
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\sony_demod.c
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\sony_demod.h
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\sony_demod_driver_version.h
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\sony_dtv.h
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\sony_integ.c
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\sony_integ.h
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\CXD2856\sony_stopwatch_port.c
# End Source File
# End Group
# Begin Source File

SOURCE=.\api\DW_Frontend\MyDebug.h
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\MyDependency.h
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\MyI2CPortingLayer.c
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\MyI2CPortingLayer.h
# End Source File
# Begin Source File

SOURCE=.\api\DW_Frontend\MyTypedef.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\api\IT9133.h
# End Source File
# End Group
# Begin Group "src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\src\CXD285-drv.c"
# End Source File
# Begin Source File

SOURCE=".\src\DIB9090-drv.c"
# End Source File
# Begin Source File

SOURCE=.\src\inttype.h
# End Source File
# Begin Source File

SOURCE=.\src\iocontrol.c
# End Source File
# Begin Source File

SOURCE=.\src\iocontrol.h
# End Source File
# Begin Source File

SOURCE=".\src\it913x-drv.c"
# End Source File
# Begin Source File

SOURCE=".\src\it930x-core.c"
# End Source File
# Begin Source File

SOURCE=".\src\it930x-core.h"
# End Source File
# Begin Source File

SOURCE=".\src\it930x-drv.c"
# End Source File
# Begin Source File

SOURCE=".\src\MXL69X-drv.c"
# End Source File
# Begin Source File

SOURCE=".\src\MyFrontend-drv.c"
# End Source File
# Begin Source File

SOURCE=".\src\Si2168B-drv.c"
# End Source File
# Begin Source File

SOURCE=.\src\type.h
# End Source File
# Begin Source File

SOURCE=.\src\usb2impl.c
# End Source File
# Begin Source File

SOURCE=.\src\usb2impl.h
# End Source File
# Begin Source File

SOURCE=.\src\user.c
# End Source File
# Begin Source File

SOURCE=.\src\user.h
# End Source File
# Begin Source File

SOURCE=.\src\userdef.h
# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Makefile
# End Source File
# End Group
# End Target
# End Project
