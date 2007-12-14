pushd
wmake -h
wmake -h test1=
wmake -h test2=
wmake -h test3=
wmake -h test4=
wmake -h test5=
wmake -h test6=
wmake -h test7=
wmake -h test8=
wmake -h test9=
rem
if [%extra_arch%] == [] goto no_extra
wmake -h arch=%extra_arch%
wmake -h arch=%extra_arch% test1=
wmake -h arch=%extra_arch% test2=
wmake -h arch=%extra_arch% test3=
wmake -h arch=%extra_arch% test4=
wmake -h arch=%extra_arch% test5=
wmake -h arch=%extra_arch% test6=
wmake -h arch=%extra_arch% test7=
wmake -h arch=%extra_arch% test8=
wmake -h arch=%extra_arch% test9=
:no_extra
popd
