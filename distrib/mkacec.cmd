@echo off
rm -f %distroot%\archives\*.ace
rm mkacec.log
cd %relroot%\%defrel%
rm filesc
echo Creating file list
comstrip %distroot%\manifest\clib_a16 > filesc
comstrip %distroot%\manifest\clib_a16 >> filesc
comstrip %distroot%\manifest\clib_d16 >> filesc
comstrip %distroot%\manifest\clib_o16 >> filesc
comstrip %distroot%\manifest\clib_o32 >> filesc
comstrip %distroot%\manifest\clib_w16 >> filesc
comstrip %distroot%\manifest\clib_w32 >> filesc
comstrip %distroot%\manifest\cm_clib_a16 >> filesc
comstrip %distroot%\manifest\cm_clib_a32 >> filesc
comstrip %distroot%\manifest\cm_clib_d16 >> filesc
comstrip %distroot%\manifest\cm_clib_d32 >> filesc
comstrip %distroot%\manifest\cm_clib_hdr >> filesc
comstrip %distroot%\manifest\cm_clib_n32 >> filesc
comstrip %distroot%\manifest\cm_clib_o16 >> filesc
comstrip %distroot%\manifest\cm_clib_o32 >> filesc
comstrip %distroot%\manifest\cm_clib_w16 >> filesc
comstrip %distroot%\manifest\cm_clib_w32 >> filesc
comstrip %distroot%\manifest\cm_core_all >> filesc
comstrip %distroot%\manifest\cm_core_dos >> filesc
comstrip %distroot%\manifest\cm_core_doswin >> filesc
comstrip %distroot%\manifest\cm_core_nt >> filesc
comstrip %distroot%\manifest\cm_core_os2 >> filesc
comstrip %distroot%\manifest\cm_core_win >> filesc
comstrip %distroot%\manifest\cm_dbg_all >> filesc
comstrip %distroot%\manifest\cm_dbg_dos >> filesc
comstrip %distroot%\manifest\cm_dbg_dosos2 >> filesc
comstrip %distroot%\manifest\cm_dbg_doswin >> filesc
comstrip %distroot%\manifest\cm_dbg_misc1 >> filesc
comstrip %distroot%\manifest\cm_dbg_misc2 >> filesc
comstrip %distroot%\manifest\cm_dbg_net >> filesc
comstrip %distroot%\manifest\cm_dbg_nt >> filesc
comstrip %distroot%\manifest\cm_dbg_os2 >> filesc
comstrip %distroot%\manifest\cm_dbg_os2nt >> filesc
comstrip %distroot%\manifest\cm_dbg_win >> filesc
comstrip %distroot%\manifest\cm_ide_all >> filesc
comstrip %distroot%\manifest\cm_ide_nt >> filesc
comstrip %distroot%\manifest\cm_ide_os2 >> filesc
comstrip %distroot%\manifest\cm_ide_dos >> filesc
comstrip %distroot%\manifest\cm_ide_win >> filesc
comstrip %distroot%\manifest\cm_hlp_dos >> filesc
comstrip %distroot%\manifest\cm_hlp_nt >> filesc
comstrip %distroot%\manifest\cm_hlp_os2 >> filesc
comstrip %distroot%\manifest\cm_hlp_win >> filesc
comstrip %distroot%\manifest\cm_plib_a16 >> filesc
comstrip %distroot%\manifest\cm_plib_a32 >> filesc
comstrip %distroot%\manifest\cm_plib_o32 >> filesc
comstrip %distroot%\manifest\cm_plib_w32 >> filesc
comstrip %distroot%\manifest\core_all >> filesc
comstrip %distroot%\manifest\core_doswin >> filesc
comstrip %distroot%\manifest\core_nt >> filesc
comstrip %distroot%\manifest\core_os2 >> filesc
comstrip %distroot%\manifest\cpp_doswin >> filesc
comstrip %distroot%\manifest\cpp_nt >> filesc
comstrip %distroot%\manifest\cpp_os2 >> filesc
comstrip %distroot%\manifest\c_doswin >> filesc
comstrip %distroot%\manifest\c_nt >> filesc
comstrip %distroot%\manifest\c_os2 >> filesc
rem comstrip %distroot%\manifest\fc_doswin >> filesc
rem comstrip %distroot%\manifest\fc_hlp_dos >> filesc
rem comstrip %distroot%\manifest\fc_hlp_nt >> filesc
rem comstrip %distroot%\manifest\fc_hlp_os2 >> filesc
rem comstrip %distroot%\manifest\fc_hlp_win >> filesc
rem comstrip %distroot%\manifest\fc_nt >> filesc
rem comstrip %distroot%\manifest\fc_os2 >> filesc
rem comstrip %distroot%\manifest\flib_d16 >> filesc
rem comstrip %distroot%\manifest\flib_d32 >> filesc
rem comstrip %distroot%\manifest\flib_n32 >> filesc
rem comstrip %distroot%\manifest\flib_o16 >> filesc
rem comstrip %distroot%\manifest\flib_o32 >> filesc
rem comstrip %distroot%\manifest\flib_w16 >> filesc
rem comstrip %distroot%\manifest\flib_w32 >> filesc
comstrip %distroot%\manifest\hlp_dos >> filesc
comstrip %distroot%\manifest\hlp_nt >> filesc
comstrip %distroot%\manifest\hlp_os2 >> filesc
comstrip %distroot%\manifest\hlp_win >> filesc
comstrip %distroot%\manifest\plib_a16 >> filesc
comstrip %distroot%\manifest\plib_a32 >> filesc
comstrip %distroot%\manifest\plib_hdr >> filesc
comstrip %distroot%\manifest\plib_o16 >> filesc
comstrip %distroot%\manifest\plib_o32 >> filesc
comstrip %distroot%\manifest\plib_w16 >> filesc
comstrip %distroot%\manifest\plib_w32 >> filesc
comstrip %distroot%\manifest\cm_samples >> filesc
comstrip %distroot%\manifest\clib_samples >> filesc
comstrip %distroot%\manifest\plib_samples >> filesc
comstrip %distroot%\manifest\win_samples >> filesc
comstrip %distroot%\manifest\ide_samples >> filesc
rem comstrip %distroot%\manifest\f_samples >> filesc
comstrip %distroot%\manifest\misc_src >> filesc
comstrip %distroot%\manifest\os2api >> filesc
comstrip %distroot%\manifest\w32api >> filesc
echo ACEing up files...
ace a %distroot%\archives\owc.ace @filesc >> %distroot%\mkacec.log 2>&1
echo All done!
