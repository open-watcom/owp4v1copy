pushd
wmake -h global
wmake -h
wmake -h global
if [%extra_arch%] == [] goto no_extra
wmake -h global
wmake -h arch=%extra_arch%
wmake -h global
:no_extra
popd
