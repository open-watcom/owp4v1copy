//
// Class definitions provided by a library vendor.
//

class employee {
    char    *Name;
protected:
    int     hourRate;
public:
    employee( char *n, int hr );
    ~employee();

    char *name() { return( Name ); }
    virtual int salary() = 0;
};

class manager : public employee {
    int     Experience;
public:
    manager( char *n, int hr, int ex ) : employee( n, hr ), Experience( ex ) {}
    int salary();
};

//
// Following is a 'programmer' class provided by library vendor and
// subsequently extended by users. Data members Overtime and OvertimeRate,
// and the member functions setOvertimeRate, addOvertime, resetOvertime
// and overtimePay were added.
//
// In a situation where library vendor didn't provide the source
// code for impl.cpp, users cannot recompile that module and hence
// cannot add any virtual member functions to the 'employee' base class.
//

class programmer : public employee {
    int     OvertimeRate;
    int     Overtime;
public:
    programmer( char *n, int hr ) : employee( n, hr ) {}
    void setOvertimeRate( int otr ) { Overtime = 0; OvertimeRate = otr; }

    void addOvertime( int ov ) { Overtime += ov; }
    void resetOvertime() { Overtime = 0; }
    int  overtimePay() { return( Overtime * OvertimeRate ); }

    int  salary() ;
};
