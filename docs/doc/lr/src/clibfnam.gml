.*
.dm imbed begin
.se *imb=0
.se *last=''
...loopimb
.  .se *imb=&*imb.+1
.  .if '&imblst(&*imb.)' ne '&*last.' .do begin
.  .  .im &imblst(&*imb.)
.  .  .sr *last=&imblst(&*imb.)
.  .  .if '&*last.' eq '&vol2.' .do begin
.  .  .  .if '&volm.' ne '' .do begin
.  .  .  .  .sepsect &company C Library Reference Volume 2
.  .  .  .do end
.  .  .do end
.  .do end
.  .if &*imb. lt &__idx .go loopimb
.dm imbed end
.*
.imbed
.*
