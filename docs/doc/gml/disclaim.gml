.if '&isbn' ne '&amp.isbn' .do begin
.if '&isbn' ne '' .do begin
.np
ISBN &isbn
.do end
.do end
.if &e'&prtd eq 0 .sr prtd='U.S.A.'
.np
Printed in &prtd.
.if '&docnum' ne '&amp.docnum' .do begin
.sk 22
.np
.ri &docnum
.do end
