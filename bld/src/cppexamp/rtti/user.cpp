#include <iostream.h>
#include <new.h>
#include <typeinfo.h>
#include "impl.h"
#include "user.h"

employeeList    *employeeList::first = NULL;
employeeList    *employeeList::last = NULL;

employeeList::~employeeList()
{
    delete Empl;
}

void employeeList::addEmployee( employee *e )
{
    if( first == NULL ) {
        first = last = new employeeList;
    } else {
        last->Next = new employeeList;
        last = last->Next;
    }
    last->Empl = e;
}

company::company()
{
    EmployeeList = new employeeList;
}

company::~company()
{
    employeeList    *el = EmployeeList;
    employeeList    *next = (el != NULL) ? el->next() : NULL;
    while( el != NULL ) {
        delete el->empl();
        delete el;
        el = next;
        next = (el != NULL) ? el->next() : NULL;
    }
}

void company::addManager( char *name, int hourRate, int experience )
{
    manager *m = new manager( name, hourRate, experience );
    EmployeeList->addEmployee( m );
}

//
// Since member functions aren't virtual, user must guarantee that
// that the newly added member functions (setOvertimeRate, addOvertime,
// resetOvertime and overtimePay) are only ever used if the 'employee'
// referred to is a 'programmer'. A dynamic_cast or a typeid can be used
// for this purpose.
//

void company::addProgrammer( char *name, int hourRate, int ovRate )
{
    programmer  *p = new programmer( name, hourRate );
    p->setOvertimeRate( ovRate );
    EmployeeList->addEmployee( p );
}

void company::addOvertime( char *name, int overtime )
{
    for( employeeList *el = EmployeeList->first; el != NULL; el = el->next() ) {
        if( !strcmp( name, el->empl()->name() ) ) {
            programmer  *pe = dynamic_cast<programmer*>( el->empl() );
            if( pe != NULL )
                pe->addOvertime( overtime );

            return;
        }
    }
}

int company::payRoll()
{
    int     expense = 0;

    for( employeeList *el = EmployeeList->first; el != NULL; el = el->next() ) {
        int pay = el->empl()->salary();
        programmer* pe = dynamic_cast<programmer*>(el->empl());
        if( pe != NULL ) {
            pay += pe->overtimePay();
            pe->resetOvertime();
        }
        cout << el->empl()->name() << " is a " << typeid( *(el->empl()) ).name()
            << " who was paid $" << pay << " this week." << endl;
        expense += pay;
    }
    cout << "The total expense in salary this week is " << expense << "." << endl;

    return( expense );
}
