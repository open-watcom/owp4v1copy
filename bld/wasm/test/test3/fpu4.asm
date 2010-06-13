.386
.387

S1 segment use16 'DATA'
status dw 0
S1 ends

S2 segment use32 'CODE'
   assume ds:S1

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
   FSTSW AX
   FNSTSW AX
   FWAIT

S2 ends

end
