if not [%1] == [] goto runit
:runit
copy h:\rel2\getviper.bat r:\dlang
copy h:\rel2\ide.doc r:\dlang
copy h:\rel2\binw\ide.cfg r:\dlang\binw
copy h:\rel2\binw\idedos.cfg r:\dlang\binw
copy h:\rel2\binw\idewin.cfg r:\dlang\binw
copy h:\rel2\binw\ideos2.cfg r:\dlang\binw
copy h:\rel2\binw\idew32.cfg r:\dlang\binw
copy h:\rel2\binw\ideads.cfg r:\dlang\binw
copy h:\rel2\binw\idenlm.cfg r:\dlang\binw

copy h:\rel2\binw\wsrv.pif r:\dlang\binw
copy h:\rel2\binw\idex.cfg r:\dlang\binw
copy h:\rel2\binw\ide.exe r:\dlang\binw
copy h:\rel2\binw\ide.hlp r:\dlang\binw
copy h:\rel2\binw\weditviw.dll r:\dlang\binw

copy h:\rel2\binp\idex.cfg r:\dlang\binp
copy h:\rel2\binp\ide.exe r:\dlang\binp
copy h:\rel2\binp\dll\epmlink.dll r:\dlang\binp\dll

copy h:\rel2\binnt\idex.cfg r:\dlang\binnt
copy h:\rel2\binnt\ide.exe r:\dlang\binnt
copy h:\rel2\binnt\weditviw.dll r:\dlang\binnt
