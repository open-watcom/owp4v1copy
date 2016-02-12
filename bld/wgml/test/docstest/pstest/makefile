#pmake: build pstest

psdate = 2/8/2016
pstime = 11:05_AM_PST

all : .SYMBOLIC
    @cd pdf4
    @wmake -h hbook=$(hbook) psdate=$(psdate) pstime=$(pstime)
    @cd ../pdf
    @wmake -h hbook=$(hbook) psdate=$(psdate) pstime=$(pstime)
    @cd ..

clean : .SYMBOLIC
    @cd pdf4
    wmake -h clean
    @cd ../pdf
    wmake -h clean
    cd ..

