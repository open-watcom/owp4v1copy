pushd
cd objs
wmake -h global
wmake -h
cd ..\examples
wmake -h global
wmake -h
cd ..\tests
wmake -h global
wmake -h
cd ..\objs
wmake -h global
wmake -h test1=
cd ..\examples
wmake -h global
wmake -h test1=
cd ..\tests
wmake -h global
wmake -h test1=
cd ..\objs
wmake -h global
wmake -h test2=
cd ..\examples
wmake -h global
wmake -h test2=
cd ..\tests
wmake -h global
wmake -h test2=
cd ..\objs
wmake -h global
wmake -h test3=
cd ..\examples
wmake -h global
wmake -h test3=
cd ..\tests
wmake -h global
wmake -h test3=
cd ..\objs
wmake -h global
cd ..\examples
wmake -h global
cd ..\tests
wmake -h global
cd ..
if [%extra_arch%] == [] goto no_extra
cd objs
wmake -h global
wmake -h arch=%extra_arch%
cd ..\examples
wmake -h global
wmake -h arch=%extra_arch%
cd ..\tests
wmake -h global
wmake -h arch=%extra_arch%
cd ..\objs
wmake -h global
wmake -h arch=%extra_arch% test1=
cd ..\examples
wmake -h global
wmake -h arch=%extra_arch% test1=
cd ..\tests
wmake -h global
wmake -h arch=%extra_arch% test1=
cd ..\objs
wmake -h global
wmake -h arch=%extra_arch% test2=
cd ..\examples
wmake -h global
wmake -h arch=%extra_arch% test2=
cd ..\tests
wmake -h global
wmake -h arch=%extra_arch% test2=
cd ..\objs
wmake -h global
wmake -h arch=%extra_arch% test3=
cd ..\examples
wmake -h global
wmake -h arch=%extra_arch% test3=
cd ..\tests
wmake -h global
wmake -h arch=%extra_arch% test3=
cd ..\objs
wmake -h global
cd ..\examples
wmake -h global
cd ..\tests
wmake -h global
cd ..
:no_extra
popd
