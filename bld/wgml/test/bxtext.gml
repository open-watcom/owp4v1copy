:CMT.This is from fl\frecord.gml and is in the FORTRAN 77 Language Reference
:CMT.Topics: "Unions"
.np
The organization of the record in memory is as follows:
.millust begin
offset  +0          +4          +8
.* .bx on 7 17 27 37
.sr c0=&INDlvl+11
.sr c1=&INDlvl+21
.sr c2=&INDlvl+31
.sr c3=&INDlvl+41
.bx on &c0 &c1 &c2 &c3
          integer     logical     (slack)
.* .bx 17 27 37
.bx &c1 &c2 &c3
                      integer     (slack)

