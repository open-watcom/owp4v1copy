#pmake: build pstest

!ifdef hbook
hbpar = hbook=$(hbook)
!endif

psdate = pstestDate
pstime = pstestTime

all : .SYMBOLIC
    @cd pdf4
    @wmake -h $(hbpar) psdate=$(psdate) pstime=$(pstime)
    @cd ../pdf
    @wmake -h $(hbpar) psdate=$(psdate) pstime=$(pstime)
    @cd ..

clean : .SYMBOLIC
    @cd pdf4
    wmake -h clean
    @cd ../pdf
    wmake -h clean
    cd ..

