// Test linkage conflict diagnostics

static int fn1( void );
extern int fn1( void );     // OK, function linkage remains internal

extern int fn2( void );
static int fn2( void );     // bad external->internal linkage redeclaration

int fn3( void );            // implicitly extern
static int fn3( void );     // bad external->internal linkage redeclaration

// We do not want to warn in this case - Watcom extension. Note that
// in -za mode, a warning will be emitted (also a warning about missing
// prototype).
void foo( void )
{
    fn4();                  // implicit extern declaration
}

static int fn4( void ) {    // possibly bad extern->static redeclaration
    return( 0 );
}



extern int i1;
int i1;                     // definition OK

extern int i2;
static int i2;              // bad external->internal linkage redeclaration

int i3;
static int i3;              // bad external->internal linkage redeclaration

int i4;
extern int i4;              // OK, symbol linkage remains external

static int i5;
extern int i5;              // OK, symbol linkage remains internal

static int i6;
int i6;                     // bad internal->external linkage redeclaration

// Avoid second 'unused static' warning. The symbol table will be iterated
// through in somewhat non-deterministic fashion and two unused static
// warnings may be output in different order depending on circumstances.
// If we only have one warning, there's no problem.
int *pi6 = &i6;

// Test function redeclaration which is considered OK (as long as the
// default calling convention is in fact __watcall, ie. -ec? isn't used)
void foo( void );
void __watcall foo( void );

// The following is also considered OK. Users may still shoot themselves
// in the foot by not including prototype in the module where function is
// defined, but sloppy programmers do not deserve too much compassion. Just
// about all other compilers (incl. wpp) allow this.
void __cdecl bar( void );
void bar( void );
