pushd
if exist *.sav del *.sav
wmake -h global
wmake -h
wmake -h save
wmake -h global
wmake -h test1=
wmake -h test1= save
wmake -h global
wmake -h test2=
wmake -h test2= save
wmake -h global
wmake -h test3=
wmake -h test3= save
wmake -h global
wmake -h test4=
wmake -h test4= save
wmake -h global
wmake -h test5=
wmake -h test5= save
wmake -h global
popd
