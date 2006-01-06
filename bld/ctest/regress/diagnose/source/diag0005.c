// Test linkage conflict diagnostics

static int fn1( void );
extern int fn1( void );     // OK, function linkage remains internal

extern int fn2( void );
static int fn2( void );     // bad external->internal linkage redeclaration

int fn3( void );            // implicitly extern
static int fn3( void );     // bad external->internal linkage redeclaration

#if 0

// Pending resolution - we may or may not want to warn
void foo( void )
{
    fn4();                  // implicit extern declaration
}

static int fn4( void ) {    // bad extern->static redeclaration
    return( 0 );
}

#endif

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
