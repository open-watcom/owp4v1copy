.386
.MODEL small
.data
foo db ?
bar dw ?
sam dd ?
.CODE
;    rep        scas dx,WORD PTR es:[si]

scasb
scasw
scasd

rep scasb
rep scasw
rep scasd

scas foo
scas bar
scas sam

rep scas foo
rep scas bar
rep scas sam

scas byte ptr [eax]
scas word ptr [eax]
scas dword ptr [eax]

rep scas byte ptr [eax]
rep scas word ptr [eax]
rep scas dword ptr [eax]

scas byte ptr [bx]
scas word ptr [bx]
scas dword ptr [bx]

rep scas byte ptr [bx]
rep scas word ptr [bx]
rep scas dword ptr [bx]

end
