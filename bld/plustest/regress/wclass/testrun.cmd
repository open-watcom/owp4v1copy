pushd
wmake -h global
wmake -h
wmake -h global
wmake -h test1=
wmake -h global
wmake -h test2=
wmake -h global
if [%extra_arch%] == [] goto no_extra
wmake -h global
wmake -h arch=%extra_arch%
wmake -h global
wmake -h arch=%extra_arch% test1=
wmake -h global
wmake -h arch=%extra_arch% test2=
wmake -h global
:no_extra
popd
