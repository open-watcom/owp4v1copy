/* Bit field errors */
struct s1 {
    char    c1  : 8;
    char    c2  : 16;
};

struct s2 {
    short   sh1 : 16;
    short   sh2 : 32;
};

struct s3 {
    __int64 i64 : 16;
};

struct s4 {
    int *p  : 16;
};

struct s5_inner {
    int i;
};

struct s5 {
    struct s5_inner s5i : 16;
};

