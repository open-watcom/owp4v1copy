
log_file=testcomp.log

.BEFORE
    @%create $(log_file)

all : .SYMBOLIC
    @wmake -h -f inline1.mif testcomp=wcc log_file=$(log_file)
    @wmake -h -f inline1.mif testcomp=wcc386 log_file=$(log_file)
    @wmake -h -f inline1.mif testcomp=wpp log_file=$(log_file)
    @wmake -h -f inline1.mif testcomp=wpp386 log_file=$(log_file)

clean : .SYMBOLIC
    @wmake -h -f inline1.mif clean
