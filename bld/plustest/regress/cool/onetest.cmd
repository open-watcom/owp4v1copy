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
cd ..\examples
wmake -h global
cd ..\tests
wmake -h global
cd ..
popd
