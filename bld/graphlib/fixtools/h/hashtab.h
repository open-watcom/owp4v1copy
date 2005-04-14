#ifndef __HASHTAB_H
#define __HASHTAB_H

typedef struct symbol {
        struct symbol   *next;
        char            *name;
        char            *value;
} symbol, **sym_table;

extern  void        AddSymbol( sym_table, char *, char * );
extern  char        *SymbolExists( sym_table, char * );
extern  sym_table   SymbolInit( void );
extern  void        SymbolFini( sym_table );

#endif
