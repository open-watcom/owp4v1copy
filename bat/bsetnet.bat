copy %distroot%\wc\csetup\install.exe q:\wc10.1
copy %distroot%\wc\csetup\dossetup.exe q:\wc10.1
copy %distroot%\wc\csetup\setup32.exe q:\wc10.1
copy %distroot%\wc\csetup\setup.exe q:\wc10.1
copy %distroot%\wc\csetup\install.sym q:\wc10.1
copy %distroot%\wc\csetup\dossetup.sym q:\wc10.1
copy %distroot%\wc\csetup\setup32.sym q:\wc10.1
copy %distroot%\wc\csetup\setup.sym q:\wc10.1
wtouch /f q:\wc10.1\setup.inf q:\wc10.1\install.exe
wtouch /f q:\wc10.1\setup.inf q:\wc10.1\dossetup.exe
wtouch /f q:\wc10.1\setup.inf q:\wc10.1\setup32.exe
wtouch /f q:\wc10.1\setup.inf q:\wc10.1\setup.exe
copy %distroot%\wc\csetup\install.exe q:\wc10.1\diskimgs\disk01
copy %distroot%\wc\csetup\dossetup.exe q:\wc10.1\diskimgs\disk01
copy %distroot%\wc\csetup\setup32.exe q:\wc10.1\diskimgs\disk01
copy %distroot%\wc\csetup\setup.exe q:\wc10.1\diskimgs\disk01
wtouch /f q:\wc10.1\setup.inf q:\wc10.1\diskimgs\disk01\install.exe
wtouch /f q:\wc10.1\setup.inf q:\wc10.1\diskimgs\disk01\dossetup.exe
wtouch /f q:\wc10.1\setup.inf q:\wc10.1\diskimgs\disk01\setup32.exe
wtouch /f q:\wc10.1\setup.inf q:\wc10.1\diskimgs\disk01\setup.exe

copy %distroot%\wc\f77setup\install.exe q:\wf10.1
copy %distroot%\wc\f77setup\dossetup.exe q:\wf10.1
copy %distroot%\wc\f77setup\setup32.exe q:\wf10.1
copy %distroot%\wc\f77setup\setup.exe q:\wf10.1
copy %distroot%\wc\f77setup\install.sym q:\wf10.1
copy %distroot%\wc\f77setup\dossetup.sym q:\wf10.1
copy %distroot%\wc\f77setup\setup32.sym q:\wf10.1
copy %distroot%\wc\f77setup\setup.sym q:\wf10.1
wtouch /f q:\wf10.1\setup.inf q:\wf10.1\install.exe
wtouch /f q:\wf10.1\setup.inf q:\wf10.1\dossetup.exe
wtouch /f q:\wf10.1\setup.inf q:\wf10.1\setup32.exe
wtouch /f q:\wf10.1\setup.inf q:\wf10.1\setup.exe
copy %distroot%\wc\f77setup\install.exe q:\wf10.1\diskimgs\disk01
copy %distroot%\wc\f77setup\dossetup.exe q:\wf10.1\diskimgs\disk01
copy %distroot%\wc\f77setup\setup32.exe q:\wf10.1\diskimgs\disk01
copy %distroot%\wc\f77setup\setup.exe q:\wf10.1\diskimgs\disk01
wtouch /f q:\wf10.1\setup.inf q:\wf10.1\diskimgs\disk01\install.exe
wtouch /f q:\wf10.1\setup.inf q:\wf10.1\diskimgs\disk01\dossetup.exe
wtouch /f q:\wf10.1\setup.inf q:\wf10.1\diskimgs\disk01\setup32.exe
wtouch /f q:\wf10.1\setup.inf q:\wf10.1\diskimgs\disk01\setup.exe
