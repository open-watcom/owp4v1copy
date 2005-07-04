.ix 'grammar' 'C language'
.ix 'syntax' 'C language'
.pp
This appendix presents the formal grammar of the C programming
language.
The following notation is used:
..sk 1 c
{digit}&SUPER0.
.discuss begin
Zero or more occurrences of
.us digit
are allowed.
.discuss end
{digit}&SUPER1.
.discuss begin
One or more occurrences of
.us digit
are allowed.
.discuss end
&LANGLE.integer-suffix&RANGLE.
.discuss begin
.us integer-suffix
is optional, with only one occurrence being allowed if present.
.discuss end
.mono A
|
.mono B
|
.mono C
.discuss begin
Choose one of
.mono A
..ct ,
.mono B
or
.mono C.
.discuss end
..tb set ›
..sk 1 c
..sr firsttab=&SYSIN.+5
..tb &firsttab. +5 +5 +5 +5 +5 +5 +5 +5 +5 +5
.section Lexical Grammar
.pp
The following topics are discussed:
.begbull
.bull
Tokens
.bull
Keywords
.bull
Identifiers
.bull
Constants
.bull
String Literals
.bull
Operators
.bull
Punctuators
.endbull
.*
.************************************************************************
.*
.beglevel
.keep begin
.if &e'&dohelp eq 0 .do begin
.section Tokens
.do end
.el .do begin
.section Definition of a Token
.do end
.pp
:ITAL.token:eITAL.
››keyword
›or›identifier
›or›constant
›or›string-literal
›or›operator
›or›punctuator
.keep break
.if &e'&dohelp eq 0 .do begin
.section Keywords
.do end
.el .do begin
.section Definition of a Keyword
.do end
.*
.************************************************************************
.*
.pp
..if '&target' eq 'PC' or '&target' eq 'PC 370' ..th ..do begin
:ITAL.keyword:eITAL.
››standard-keyword
›or›&company.-extended-keyword
.keep break
..sk 1 c
:ITAL.standard-keyword:eITAL.
..do end
..el ..do begin
:ITAL.keyword:eITAL.
..do end
.*
.************************************************************************
.*
.im kwlist
.keep break
.*
.************************************************************************
.*
..if '&target' eq 'PC' or '&target' eq 'PC 370' ..th ..do begin
..sk 1 c
:ITAL.&company.-extended-keyword:eITAL.
.sk 1
.im kwwatcom
..do end
.keep break
.*
.************************************************************************
.*
.if &e'&dohelp eq 0 .do begin
.section Identifiers
.do end
.el .do begin
.section Definition of an Identifier
.do end
.pp
:ITAL.identifier:eITAL.
››nondigit {nondigit | digit}&SUPER0.
.keep break
..sk 1 c
:ITAL.nondigit:eITAL.
.monoon
››a | b | ... | z | A | B | ... | Z | _
.monooff
.keep break
..sk 1 c
:ITAL.digit:eITAL.
.monoon
››0 | 1 | ... | 9
.monooff
.keep break
.if &e'&dohelp eq 0 .do begin
.section Constants
.do end
.el .do begin
.section Definition of a Constant
.do end
.pp
:ITAL.constant:eITAL.
››floating-constant
›or›integer-constant
›or›enumeration-constant
›or›character-constant
.keep break
..sk 1 c
:ITAL.floating-constant:eITAL.
››fractional-constant &LANGLE.exponent-part&RANGLE. &LANGLE.floating-suffix&RANGLE.
›or›digit-sequence exponent-part &LANGLE.floating-suffix&RANGLE.
.keep break
..sk 1 c
:ITAL.exponent-part:eITAL.
››:MONO.e|E:eMONO.
&LANGLE.:MONO.+|-:eMONO.
..ct &RANGLE. digit-sequence
.keep break
..sk 1 c
:ITAL.floating-suffix:eITAL.
››:MONO. f | F | l | L:eMONO.
.keep break
..sk 1 c
:ITAL.fractional-constant:eITAL.
››&LANGLE.digit-sequence&RANGLE. ~. digit-sequence
›or›digit-sequence ~.
.keep break
..sk 1 c
:ITAL.digit-sequence:eITAL.
››{digit}&SUPER1.
.keep break
..sk 1 c
:ITAL.integer-constant:eITAL.
›  ›decimal-constant &LANGLE.integer-suffix&RANGLE.
›or›octal-constant &LANGLE.integer-suffix&RANGLE.
›or›hexadecimal-constant &LANGLE.integer-suffix&RANGLE.
.keep break
..sk 1 c
:ITAL.integer-suffix:eITAL.
››:MONO.u|U:eMONO.
&LANGLE.:MONO.l|L:eMONO.
..ct &RANGLE.
›or›:MONO.l|L:eMONO.
..ct &LANGLE.:MONO.u|U:eMONO.
..ct &RANGLE.
.keep break
..sk 1 c
:ITAL.decimal-constant:eITAL.
››nonzero-digit{digit}&SUPER0.
.keep break
..sk 1 c
:ITAL.nonzero-digit:eITAL.
››:MONO. 1 | 2 | ... | 9:eMONO.
.keep break
..sk 1 c
:ITAL.octal-constant:eITAL.
››:MONO.0:eMONO.{octal-digit}&SUPER0.
.keep break
..sk 1 c
:ITAL.octal-digit:eITAL.
.monoon
››0 | 1 | ... | 7
.monooff
.keep break
..sk 1 c
:ITAL.hexadecimal-constant:eITAL.
››:MONO.0x:eMONO.|:MONO.0X:eMONO.{hexadecimal-digit}&SUPER1.
.keep break
..sk 1 c
:ITAL.hexadecimal-digit:eITAL.
.monoon
››0 | 1 | ... | 9 |
››a | b | ... | f | A | B | ... | F
.monooff
.keep break
..sk 1 c
:ITAL.enumeration-constant:eITAL.
››identifier
.keep break
..sk 1 c
:ITAL.character-constant:eITAL.
›  ›:MONO.':eMONO.{c-char}&SUPER1.:MONO.':eMONO.
›or›:MONO.L':eMONO.{c-char}&SUPER1.:MONO.':eMONO.
.keep break
..sk
:ITAL.c-char:eITAL.
››any character in the source character set except
››the single-quote
.mono '
..ct , backslash
.mono \
..ct , or new-line character
›or›escape-sequence
.keep break
..sk
:ITAL.escape-sequence:eITAL.
is one of
.monoon
››\' \" \\
››\:HP0.o:eHP0. \:HP0.oo:eHP0. \:HP0.ooo:eHP0.
››\x:HP0.{hexadecimal-digit}:eHP0.&SUPER1.
››\a   \b   \f   \n   \r   \t   \v
.monooff
.keep break
.*
.if &e'&dohelp eq 0 .do begin
.section String Literals
.do end
.el .do begin
.section Definition of a String Literal
.do end
.pp
:ITAL.string-literal:eITAL.
›  ›:MONO.":eMONO.{s-char}&SUPER0.:MONO.":eMONO.
›or›:MONO.L":eMONO.{s-char}&SUPER0.:MONO.":eMONO.
.keep break
..sk 1 c
:ITAL.s-char:eITAL.
››any character in the source character set except
››the double-quote
.mono "
..ct , backslash
.mono \
..ct , or new-line character
›or›escape-sequence
.keep break
.*
.if &e'&dohelp eq 0 .do begin
.section Operators
.do end
.el .do begin
.section Definition of an Operator
.do end
.*
.pp
:ITAL.operator:eITAL.
is one of
.monoon
..co off
..br
›[  ]  (  )  .  ->
›++  --  &  *  +  -  ~~  !  sizeof
›/  %  <<  >>  <  >  <=  >=  ==  !=  ^  |  &&  ||
›?  :
›=  *=  /=  %=  +=  -=  <<=  >>=  &=  ^=  |=
›,  #  ##
.*
.************************************************************************
.*
..if '&target' eq 'PC' ..th ..do begin
›:>
..do end
..if '&target' eq 'PC 370' ..th ..do begin
›:> :HP0.(&wcboth.):eHP0.
..do end
.*
.************************************************************************
.*
..co on
.monooff
.keep break
.if &e'&dohelp eq 0 .do begin
.section Punctuators
.do end
.el .do begin
.section Definition of a Punctuator
.do end
.pp
:ITAL.punctuator:eITAL.
.monoon
..co off
..br
››[  ]  (  )  {  }  *  ,  :  = ; ... #
..co on
.monooff
.endlevel
.*
.************************************************************************
.*
.keep break
.section Phrase Structure Grammar
.pp
The following topics are discussed:
.begbull
.bull
Expressions
.bull
Declarations
.bull
Statements
.bull
External Definitions
.endbull
.*
.************************************************************************
.*
.beglevel
.if &e'&dohelp eq 0 .do begin
.section Expressions
.do end
.el .do begin
.section Definition of Expressions
.do end
.pp
:ITAL.constant-expression:eITAL.
››conditional-expression
.keep break
..sk 1 c
:ITAL.expression:eITAL.
››assignment-expression{
.monoon
..ct ,
.monooff
assignment-expression}&SUPER0.
.keep break
..sk 1 c
:ITAL.assignment-expression:eITAL.
››conditional-expression
›or›unary-expression assignment-operator assignment-expression
.keep break
..sk 1 c
:ITAL.assignment-operator:eITAL.
is one of
.monoon
..co off
..br
››=  *=  /=  %=  +=  -=  <<=  >>=  &=  ^=  |=
..co on
.monooff
.keep break
..sk 1 c
:ITAL.conditional-expression:eITAL.
››logical-OR-expression &LANGLE.:MONO.?:eMONO.
expression
.mono :
conditional-expression&RANGLE.
.keep break
..sk 1 c
:ITAL.logical-OR-expression:eITAL.
››logical-AND-expression{:MONO.||:eMONO.
logical-AND-expression}&SUPER0.
.keep break
..sk 1 c
:ITAL.logical-AND-expression:eITAL.
››inclusive-OR-expression {:MONO.&&:eMONO.
inclusive-OR-expression}&SUPER0.
.keep break
..sk 1 c
:ITAL.inclusive-OR-expression:eITAL.
››exclusive-OR-expression {:MONO.|:eMONO.
exclusive-OR-expression}&SUPER0.
.keep break
..sk 1 c
:ITAL.exclusive-OR-expression:eITAL.
››AND-expression {:MONO.^:eMONO.
AND-expression}&SUPER0.
.keep break
..sk 1 c
:ITAL.AND-expression:eITAL.
››equality-expression {:MONO.&:eMONO.
equality-expression}&SUPER0.
.keep break
..sk 1 c
:ITAL.equality-expression:eITAL.
››relational-expression
{:MONO.==:eMONO.|:MONO.!=:eMONO.
relational-expression}&SUPER0.
.keep break
..sk 1 c
:ITAL.relational-expression:eITAL.
››shift-expression {:MONO.<:eMONO.|:MONO.>:eMONO.|:MONO.<=:eMONO.|:MONO.>=:eMONO.
shift-expression}&SUPER0.
.keep break
..sk 1 c
:ITAL.shift-expression:eITAL.
››additive-expression {:MONO.<<:eMONO.|:MONO.>>:eMONO.
additive-expression}&SUPER0.
.keep break
..sk 1 c
:ITAL.additive-expression:eITAL.
››multiplicative-expression {:MONO.+:eMONO.|:MONO.-:eMONO.
multiplicative-expression}&SUPER0.
.keep break
..sk 1 c
:ITAL.multiplicative-expression:eITAL.
››cast-expression {:MONO.*:eMONO.|:MONO./:eMONO.|:MONO.%:eMONO.
cast-expression}&SUPER0.
.keep break
..sk 1 c
:ITAL.cast-expression:eITAL.
››unary-expression
›or›:MONO.(:eMONO.
type-name
.mono )
cast-expression
.keep break
..sk 1 c
:ITAL.unary-expression:eITAL.
››postfix-expression
›or›:MONO.++:eMONO. | :MONO.--:eMONO. | :MONO.sizeof:eMONO.
unary-expression
›or›:MONO.sizeof(:eMONO.
type-name
.mono )
›or›unary-operator cast-expression
.keep break
..sk 1 c
:ITAL.unary-operator:eITAL.
is one of
.monoon
››&  *  +  -  ~~  !
.monooff
.keep break
..sk 1 c
:ITAL.postfix-expression:eITAL.
››primary-expression
›or›postfix-expression
.mono [
expression
.mono ]
›or›postfix-expression
.mono (
&LANGLE.argument-expression-list&RANGLE.
.mono )
›or›postfix-expression . identifier
›or›postfix-expression
.mono ->
identifier
›or›postfix-expression
.mono ++
›or›postfix-expression
.mono --
.keep break
..sk 1 c
:ITAL.argument-expression-list:eITAL.
››assignment-expression {:MONO.,:eMONO.
assignment-expression}&SUPER0.
.keep break
..sk 1 c
:ITAL.primary-expression:eITAL.
››identifier
›or›constant
›or›string-literal
›or›:MONO.(:eMONO. expression :MONO.):eMONO.
.keep break
.if &e'&dohelp eq 0 .do begin
.section Declarations
.do end
.el .do begin
.section Definition of a Declaration
.do end
.pp
:ITAL.declaration:eITAL.
››declaration-specifiers &LANGLE.init-declarator-list&RANGLE.:MSEMI.
.keep break
..sk 1 c
:ITAL.declaration-specifiers:eITAL.
››storage-class-specifier &LANGLE.declaration-specifiers&RANGLE.
›or›type-specifier &LANGLE.declaration-specifiers&RANGLE.
.keep break
..sk 1 c
:ITAL.init-declarator-list:eITAL.
››init-declarator {:MONO.,:eMONO.
init-declarator}&SUPER0.
.keep break
..sk 1 c
:ITAL.init-declarator:eITAL.
››declarator &LANGLE.:MONO.=:eMONO. initializer&RANGLE.
.keep break
..sk 1 c
:ITAL.storage-class-specifier:eITAL.
.monoon
››typedef | extern | static | auto | register
.monooff
.keep break
..sk 1 c
:ITAL.type-specifier:eITAL.
.monoon
››void | char | short | int | long | float |
››double | signed | unsigned
.monooff
›or›struct-or-union-specifier
›or›enum-specifier
›or›typedef-name
›or›type-qualifier
.*
.************************************************************************
.*
..if '&target' eq 'PC' or '&target' eq 'PC 370' ..th ..do begin
.keep break
..sk 1 c
:ITAL.type-qualifier:eITAL.
››:MONO.const | volatile:eMONO.
›or›&company.-type-qualifier
.keep break
..sk 1 c
:ITAL.&company.-type-qualifier:eITAL.
.im tqwatcom
.sk 1
..do end
..el ..do begin
.keep break
..sk 1 c
:ITAL.type-qualifier:eITAL.
.monoon
››const | volatile
.monooff
..do end
.*
.************************************************************************
.*
.keep break
..sk 1 c
:ITAL.struct-or-union-specifier:eITAL.
››struct-or-union &LANGLE.identifier&RANGLE.
.mono {
struct-declaration-list
.mono }
›or›struct-or-union identifier
.keep break
..sk 1 c
:ITAL.struct-or-union:eITAL.
››:MONO.struct | union:eMONO.
.keep break
..sk 1 c
:ITAL.struct-declaration-list:eITAL.
››{struct-declaration}&SUPER1.
.keep break
..sk 1 c
:ITAL.struct-declaration:eITAL.
››type-specifier-list struct-declarator-list:MSEMI.
.keep break
..sk 1 c
:ITAL.type-specifier-list:eITAL.
››{type-specifier}&SUPER1.
.keep break
..sk 1 c
:ITAL.struct-declarator-list:eITAL.
››struct-declarator {:MONO.,:eMONO.
struct-declarator}&SUPER0.
.keep break
..sk 1 c
:ITAL.struct-declarator:eITAL.
››declarator
›or›&LANGLE.declarator&RANGLE.
.mono :
constant-expression
.keep break
..sk 1 c
:ITAL.enum-specifier:eITAL.
››:MONO.enum:eMONO.
&LANGLE.identifier&RANGLE.
.mono {
enumerator-list
.mono }
›or›:MONO.enum:eMONO.
identifier
.keep break
..sk 1 c
:ITAL.enumerator-list:eITAL.
››enumerator {:MONO.,:eMONO.
enumerator}&SUPER0.
.keep break
..sk 1 c
:ITAL.enumerator:eITAL.
››enumeration-constant &LANGLE.:MONO.=:eMONO.
constant-expression&RANGLE.
.keep break
..sk 1 c
:ITAL.declarator:eITAL.
››&LANGLE.pointer&RANGLE. direct-declarator
.keep break
..sk 1 c
:ITAL.direct-declarator:eITAL.
››identifier
›or›
..ct .mono (
declarator
.mono )
›or›direct-declarator
.mono [
&LANGLE.constant-expression&RANGLE.
.mono ]
›or›direct-declarator
.mono (
parameter-type-list
.mono )
›or›direct-declarator
.mono (
&LANGLE.identifier-list&RANGLE.
.mono )
.keep break
..sk 1 c
:ITAL.pointer:eITAL.
››{:MONO.*:eMONO.
&LANGLE.type-specifier-list&RANGLE.}&SUPER1.
.keep break
..sk 1 c
:ITAL.parameter-type-list:eITAL.
››parameter-list &LANGLE.:MONO., ...:eMONO.
..ct &RANGLE.
.keep break
..sk 1 c
:ITAL.parameter-list:eITAL.
››parameter-declaration {:MONO.,:eMONO.
parameter-declaration}&SUPER0.
..sk 1 c
.keep break
:ITAL.parameter-declaration:eITAL.
››declaration-specifiers declarator
›or›declaration-specifiers &LANGLE.abstract-declarator&RANGLE.
.keep break
..sk 1 c
:ITAL.identifier-list:eITAL.
››identifier {:MONO.,:eMONO.
identifier}&SUPER0.
.keep break
..sk 1 c
:ITAL.type-name:eITAL.
››type-specifier-list &LANGLE.abstract-declarator&RANGLE.
.keep break
..sk 1 c
:ITAL.abstract-declarator:eITAL.
››pointer
›or›&LANGLE.pointer&RANGLE. direct-abstract-declarator
.keep break
..sk 1 c
:ITAL.direct-abstract-declarator:eITAL.
››:MONO.(:eMONO.
abstract-declarator
.mono )
›or›&LANGLE.direct-abstract-declarator&RANGLE.
.mono [
&LANGLE.constant-expression&RANGLE.
.mono ]
›or›&LANGLE.direct-abstract-declarator&RANGLE.
.mono (
&LANGLE.parameter-type-list&RANGLE.
.mono )
.keep break
..sk 1 c
:ITAL.typedef-name:eITAL.
››identifier
.keep break
..sk 1 c
:ITAL.initializer:eITAL.
››assignment-expression
›or›:MONO.{:eMONO.initializer-list &LANGLE.:MONO.,:eMONO.&RANGLE.:MONO.}:eMONO.
.keep break
..sk 1 c
:ITAL.initializer-list:eITAL.
››initializer {:MONO.,:eMONO.
initializer}&SUPER0.
.keep break
.if &e'&dohelp eq 0 .do begin
.section Statements
.do end
.el .do begin
.section Definition of a Statement
.do end
.pp
:ITAL.statement:eITAL.
››labelled-statement
›or›compound-statement
›or›expression-statement
›or›selection-statement
›or›iteration-statement
›or›jump-statement
.keep break
..sk 1 c
:ITAL.labelled-statement:eITAL.
››identifier
.mono :
statement
›or›:MONO.case:eMONO.
constant-expression
.mono :
statement
›or›:MONO.default ::eMONO.
statement
.keep break
..sk 1 c
:ITAL.compound-statement:eITAL.
››:MONO.{:eMONO.
&LANGLE.declaration-list&RANGLE. &LANGLE.statement-list&RANGLE.:MONO.}:eMONO.
.keep break
..sk 1 c
:ITAL.declaration-list:eITAL.
››{declaration}&SUPER1.
.keep break
..sk 1 c
:ITAL.statement-list:eITAL.
››{statement}&SUPER1.
.keep break
..sk 1 c
:ITAL.expression-statement:eITAL.
››&LANGLE.expression&RANGLE.:MSEMI.
.keep break
..sk 1 c
:ITAL.selection-statement:eITAL.
››:MONO.if     (:eMONO.
expression
.mono )
statement
›or›:MONO.if     (:eMONO.
expression
.mono )
statement
.mono else
statement
›or›:MONO.switch (:eMONO.
expression
.mono )
statement
.keep break
..sk 1 c
:ITAL.iteration-statement:eITAL.
››:MONO.while (:eMONO.
expression
.mono )
statement
›or›:MONO.do:eMONO.
statement
.mono while (
expression
.monoon
);
.monooff
›or›:MONO.for (:eMONO.
&LANGLE.expression&RANGLE.:MSEMI.
&LANGLE.expression&RANGLE.:MSEMI.
&LANGLE.expression&RANGLE.
.mono )
statement
.keep break
..sk 1 c
:ITAL.jump-statement:eITAL.
››:MONO.goto:eMONO.
identifier:MSEMI.
›or›:MONO.continue:eMONO.:MSEMI.
›or›:MONO.break:eMONO.:MSEMI.
›or›:MONO.return:eMONO. &LANGLE.expression&RANGLE.:MSEMI.
.keep break
.*
.section External Definitions
.*
.pp
:ITAL.file:eITAL.
››{external-definition}&SUPER1.
.keep break
..sk 1 c
:ITAL.external-definition:eITAL.
››function-definition
›or›declaration
.keep break
..sk 1 c
:ITAL.function-definition:eITAL.
››&LANGLE.declaration-specifiers&RANGLE. declarator &LANGLE.declaration-list&RANGLE.
›››compound-statement
.endlevel
.*
.************************************************************************
.*
.keep break
.section Preprocessing Directives Grammar
.beglevel
.pp
:ITAL.preprocessing-file:eITAL.
››group
.keep break
..sk 1 c
:ITAL.group:eITAL.
››{group-part}&SUPER1.
.keep break
..sk 1 c
:ITAL.group-part:eITAL.
››&LANGLE.pp-token&RANGLE. new-line
›or›if-section
›or›control-line
.keep break
..sk 1 c
:ITAL.if-section:eITAL.
››if-group {elif-group}&SUPER0. &LANGLE.else-group&RANGLE. endif-line
.keep break
..sk 1 c
:ITAL.if-group:eITAL.
.monoon
››# if
.monooff
const-expression new-line &LANGLE.group&RANGLE.
.monoon
››# ifdef
.monooff
identifier new-line &LANGLE.group&RANGLE.
.monoon
››# ifndef
.monooff
identifier new-line &LANGLE.group&RANGLE.
.keep break
..sk 1 c
:ITAL.elif-group:eITAL.
.monoon
››# elif
.monooff
constant-expression new-line &LANGLE.group&RANGLE.
.keep break
..sk 1 c
:ITAL.else-group:eITAL.
.monoon
››# else
.monooff
new-line &LANGLE.group&RANGLE.
.keep break
..sk 1 c
:ITAL.endif-line:eITAL.
.monoon
››# endif
.monooff
new-line
.keep break
..sk 1 c
:ITAL.control-line:eITAL.
.monoon
››# include
.monooff
pp-tokens new-line
.monoon
››# define
.monooff
identifier &LANGLE.pp-tokens&RANGLE. new-line
.monoon
››# define
.monooff
identifier
.mono (
&LANGLE.identifier-list&RANGLE.
.mono )
&LANGLE.pp-tokens&RANGLE. new-line
.monoon
››# undef
.monooff
identifier new-line
.monoon
››# line
.monooff
pp-tokens new-line
.monoon
››# error
.monooff
&LANGLE.pp-tokens&RANGLE. new-line
.monoon
››# pragma
.monooff
&LANGLE.pp-tokens&RANGLE. new-line
.monoon
››#
.monooff
new-line
.keep break
..sk 1 c
:ITAL.pp-tokens:eITAL.
››{preprocessing-token}&SUPER1.
.keep break
..sk 1 c
:ITAL.preprocessing-token:eITAL.
››header-name (only within a
.mono #include
directive)
›or›identifier (no keyword distinction)
›or›constant
›or›string-literal
›or›operator
›or›punctuator
›or›each non-white-space character that cannot be one of the above
.keep break
..sk 1 c
:ITAL.header-name:eITAL.
.monoon
››<
.monooff
..ct {h-char}&SUPER0.
..ct .mono >
.keep break
..sk 1 c
:ITAL.h-char:eITAL.
››any character in the source character set except :ITAL.new-line:eITAL. and
.mono >
.keep break
..sk 1 c
:ITAL.new-line:eITAL.
››the new-line character
.endlevel
..tb set
.keep end
