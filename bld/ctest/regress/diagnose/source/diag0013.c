/*
 *  Changelist 31737
 *  Should cause an error because the pragma attempts to define code for an already existing function
 */

int foo(int i) { return i; }

#pragma aux foo = \
    "mov  eax,1"  \
    parm caller [eax]

int main(void)
{
    return foo(5);
}
