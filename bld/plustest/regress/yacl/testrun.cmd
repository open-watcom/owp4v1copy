pushd
cd base
wmake -h global
wmake -h
cd ..\io
wmake -h global
wmake -h
cd ..\test
wmake -h global
wmake -h
cd ..\base
wmake -h global
wmake -h test1=
cd ..\io
wmake -h global
wmake -h test1=
cd ..\test
wmake -h global
wmake -h test1=
cd ..\base
wmake -h global
wmake -h test2=
cd ..\io
wmake -h global
wmake -h test2=
cd ..\test
wmake -h global
wmake -h test2=
cd ..\base
wmake -h global
wmake -h test3=
cd ..\io
wmake -h global
wmake -h test3=
cd ..\test
wmake -h global
wmake -h test3=
cd ..\base
wmake -h global
cd ..\io
wmake -h global
cd ..\test
wmake -h global
cd ..
if [%extra_arch%] == [] goto no_extra
cd base
wmake -h global
wmake -h arch=%extra_arch%
cd ..\io
wmake -h global
wmake -h arch=%extra_arch%
cd ..\test
wmake -h global
wmake -h arch=%extra_arch%
cd ..\base
wmake -h global
wmake -h arch=%extra_arch% test1=
cd ..\io
wmake -h global
wmake -h arch=%extra_arch% test1=
cd ..\test
wmake -h global
wmake -h arch=%extra_arch% test1=
cd ..\base
wmake -h global
wmake -h arch=%extra_arch% test2=
cd ..\io
wmake -h global
wmake -h arch=%extra_arch% test2=
cd ..\test
wmake -h global
wmake -h arch=%extra_arch% test2=
cd ..\base
wmake -h global
wmake -h arch=%extra_arch% test3=
cd ..\io
wmake -h global
wmake -h arch=%extra_arch% test3=
cd ..\test
wmake -h global
wmake -h arch=%extra_arch% test3=
cd ..\base
wmake -h global
cd ..\io
wmake -h global
cd ..\test
wmake -h global
cd ..
:no_extra
popd
