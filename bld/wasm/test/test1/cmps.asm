.386
.MODEL small
.data
foo db ?
bar dw ?
sam dd ?
.CODE
;    rep        cmps dx,WORD PTR es:[si]

cmpsb
cmpsw
cmpsd

rep cmpsb
rep cmpsw
rep cmpsd

cmps foo, foo
cmps bar, bar
cmps sam, sam

rep cmps foo, foo
rep cmps bar, bar
rep cmps sam, sam

cmps byte ptr [eax], byte ptr [eax]
cmps word ptr [eax], word ptr [eax]
cmps dword ptr [eax], dword ptr [eax]

cmps byte ptr cs:[eax], byte ptr [eax]
cmps word ptr cs:[eax], word ptr [eax]
cmps dword ptr cs:[eax], dword ptr [eax]

cmps byte ptr cs:[eax],[eax]
cmps word ptr cs:[eax],[eax]
cmps dword ptr cs:[eax],[eax]

rep cmps byte ptr [eax], byte ptr [eax]
rep cmps word ptr [eax], word ptr [eax]
rep cmps dword ptr [eax], dword ptr [eax]

rep cmps byte ptr cs:[eax], byte ptr [eax]
rep cmps word ptr cs:[eax], word ptr [eax]
rep cmps dword ptr cs:[eax], dword ptr [eax]

rep cmps byte ptr cs:[eax],[eax]
rep cmps word ptr cs:[eax],[eax]
rep cmps dword ptr cs:[eax],[eax]

cmps byte ptr [bx], byte ptr [bx]
cmps word ptr [bx], word ptr [bx]
cmps dword ptr [bx], dword ptr [bx]

cmps byte ptr cs:[bx], byte ptr [bx]
cmps word ptr cs:[bx], word ptr [bx]
cmps dword ptr cs:[bx], dword ptr [bx]

cmps byte ptr cs:[bx],[bx]
cmps word ptr cs:[bx],[bx]
cmps dword ptr cs:[bx],[bx]

rep cmps byte ptr [bx], byte ptr [bx]
rep cmps word ptr [bx], word ptr [bx]
rep cmps dword ptr [bx], dword ptr [bx]

rep cmps byte ptr cs:[bx], byte ptr [bx]
rep cmps word ptr cs:[bx], word ptr [bx]
rep cmps dword ptr cs:[bx], dword ptr [bx]

rep cmps byte ptr cs:[bx],[bx]
rep cmps word ptr cs:[bx],[bx]
rep cmps dword ptr cs:[bx],[bx]

end
