class employeeList {
    employee        *Empl;
    employeeList    *Next;
public:
    employeeList() : Empl( 0 ), Next( 0 ) { }
    ~employeeList();

    employeeList *next() { return( Next ); }
    employee *empl() { return( Empl ); }

    static employeeList *first;
    static employeeList *last;
    static void addEmployee( employee * );
};

class company {
    employeeList    *EmployeeList;
public:
    company();
    ~company();
    void addManager( char *name, int hourRate, int experience );
    void addProgrammer( char *name, int hourRate, int overtimeRate );
    void addOvertime( char *name, int overtime );
    int payRoll();
};
