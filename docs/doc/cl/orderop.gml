.ix 'precedence'
.ix 'order of operation'
.ix 'expression' 'precedence'
.ix 'expression' 'priority'
.ix 'operator' 'precedence'
.ix 'operator' 'priority'
..sk 1 c
.if &e'&nobox eq 0 .do begin
..fk begin
..sr *start=&SYSIN.+1
..bx on  &*start. +13 +24
..sr *start=&*start.+1
..tb     &*start. +13 +6 +6 +6 +6 +6
..tb set ›
:SF font=13.
›Expression Type›Operators
:eSF.
..bx
:SF font=13.
›primary       ›identifier  ››constant
›              ›string      ››(expression)
:eSF.
..bx
:SF font=14.
›:SF font=13.postfix:eSF.›a[b] ›f()
:eSF.:SF font=14.
››a.b   ›a->b›a++   ›a--
:eSF.
..bx
:SF font=14.
›:SF font=13.unary:eSF.›sizeof u   ›sizeof( a )
:eSF.:SF font=14.
››++a  ›--a  ›&a    ›*a
››+a   ›-a   ›~~a   ›!a
:eSF.
..bx
:SF font=14.
›:SF font=13.cast:eSF.›(type) a
:eSF.
..bx
:SF font=14.
›:SF font=13.multiplicative:eSF.›a * b   ›a / b   ›a % b
:eSF.
..bx
:SF font=14.
›:SF font=13.additive:eSF.›a + b   ›a - b
:eSF.
..bx
:SF font=14.
›:SF font=13.shift:eSF.›a << b  ›a >> b
:eSF.
..bx
:SF font=14.
›:SF font=13.relational:eSF.›a < b   ›a > b   ›a <= b   ›a >= b
:eSF.
..bx
:SF font=14.
›:SF font=13.equality:eSF.›a == b  ›a != b
:eSF.
..bx
:SF font=14.
›:SF font=13.bitwise AND:eSF.›a & b
:eSF.
..bx
:SF font=14.
›:SF font=13.bitwise exclusive OR:eSF.›a ^ b
:eSF.
..bx
:SF font=14.
›:SF font=13.bitwise inclusive OR:eSF.›a | b
:eSF.
..bx
:SF font=14.
›:SF font=13.logical AND:eSF.›a && b
:eSF.
..bx
:SF font=14.
›:SF font=13.logical OR:eSF.›a || b
:eSF.
..bx
:SF font=14.
›:SF font=13.conditional &dagger.:eSF.›a ? b : c
:eSF.
..bx
:SF font=14.
›:SF font=13.assignment &dagger.:eSF.›a = b ›a += b ›a -= b ›a *= b
:eSF.:SF font=14.
››a /= b  ›a %= b  ›a &= b  ›a ^= b
››a |= b  ›a <<= b ›a >>= b
:eSF.
..bx
:SF font=14.
›:SF font=13.comma:eSF.›a,b
:eSF.
..bx off
:SF font=13.
›&dagger. associates from right to left
..tb
:eSF.
..fk end
.do end
.el .do begin
.millust begin
Expression Type         Operators
----------------------  ------------------------------
primary                 identifier
                        constant
                        string
                        (expression)
----------------------  ------------------------------
postfix                 a[b]
                        f()
                        a.b     a->b
                        a++     a--
----------------------  ------------------------------
unary                   sizeof u   sizeof( a )
                        ++a     --a
                        &a      *a
                        +a      -a      ~~a      !a
----------------------  ------------------------------
cast                    (type) a
----------------------  ------------------------------
multiplicative          a * b   a / b   a % b
----------------------  ------------------------------
additive                a + b   a - b
----------------------  ------------------------------
shift                   a << b  a >> b
----------------------  ------------------------------
relational              a < b   a > b   a <= b  a >= b
----------------------  ------------------------------
equality                a == b  a != b
----------------------  ------------------------------
bitwise AND             a & b
----------------------  ------------------------------
bitwise exclusive OR    a ^ b
----------------------  ------------------------------
bitwise inclusive OR    a | b
----------------------  ------------------------------
logical AND             a && b
----------------------  ------------------------------
logical OR              a || b
----------------------  ------------------------------
conditional (*)         a ? b : c
----------------------  ------------------------------
assignment (*)          a = b
                        a += b     a -= b
                        a *= b     a /= b     a %= b
                        a &= b     a ^= b     a |= b
                        a <<= b    a >>= b
----------------------  ------------------------------
comma                   a,b
----------------------  ------------------------------
.millust end
.pp
(*) associates from right to left
.do end
..br
