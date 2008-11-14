enum foo_enum { FOO, BAR, BAZ };
typedef enum foo_enum foo_enum_t;

extern int foo( char e );

int foo( foo_enum_t e ) /* Does not match declaration. */
{
    return( e );
}

int bar( char *pc, foo_enum_t *pe )
{
    int     d = pc - pe;        /* Various pointer type mismatches. */
    pc = pe;
    ++d;
    return( pc == pe );
}
