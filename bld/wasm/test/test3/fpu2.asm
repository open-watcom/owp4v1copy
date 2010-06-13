.286
.8087

S1 segment public 'DATA'
   assume ds:S1
   assume cs:S1
status dw 0
S1 ends

S2 segment public 'CODE'
   assume ds:S1
   assume cs:S2

   FLD tbyte ptr [bx + 1]
   FCLEX
   FNCLEX
   FINIT
   FNINIT
   FDISI
   FNDISI
   FENI
   FNENI
   FSAVE status
   FNSAVE status
   FRSTOR status
   FSTENV status
   FNSTENV status
   FLDENV status
   FSTCW status
   FNSTCW status
   FSTSW status
   FNSTSW status
;   FSTSW AX
;   FNSTSW AX
   FWAIT

S2 ends

end
