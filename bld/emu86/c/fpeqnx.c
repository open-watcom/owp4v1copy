#include <signal.h>
#include <sys/magic.h>

void __FPE_handler( void )
{
    kill( __MAGIC.my_pid, SIGFPE );
}
