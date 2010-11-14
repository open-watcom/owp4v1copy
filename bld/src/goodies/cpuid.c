/*
 * cpuid.c - A sample program showing a method to identify CPU features
 * using the processor's CPUID instruction.
 *
 * For 32-bit environments only.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

void cpu_id( uint32_t cpuinfo[4], uint32_t infotype );
#pragma aux cpu_id =      \
    ".586"                \
    "cpuid"               \
    "mov [esi+0],eax"     \
    "mov [esi+4],ebx"     \
    "mov [esi+8],ecx"     \
    "mov [esi+12],edx"    \
    parm [esi] [eax] modify [ebx ecx edx];

static const char   *feature_desc[][2] =
{
    { "On-chip x87", "FPU" },
    { "Virtual 8086 Mode Enhancements", "VME" },
    { "Debugging Extensions", "DE" },
    { "Page Size Extensions", "PSE" },
    { "Time Stamp Counter", "TSC" },
    { "RDMSR/WRMSR Instructions", "MSR" },
    { "Physical Address Extensions", "PAE" },
    { "Machine Check Exception", "MCE" },
    { "CMPXCHG8B Supported", "CX8" },
    { "On-chip APIC", "APIC" },
    { "Reserved 1", "R1!!" },                   /* Bit 10 */
    { "SYSENTER/SYSEXIT Instructions", "SEP" },
    { "Memory Type Range Registers", "MTRR" },
    { "PTE Global Bit", "PGE" },
    { "Machine Check Architecture", "MCA" },
    { "Conditional Move Instructions", "CMOV" },
    { "Page Attribute Table", "PAT" },
    { "36-bit Page Size Extension", "PSE-36" },
    { "Processor Serial Number", "PSN" },
    { "CFLUSH Instruction", "CLFSH" },
    { "Reserved 2", "R2!!" },                   /* Bit 20 */
    { "Debug Store", "DS" },
    { "Thermal Monitor and Clock Control", "ACPI" },
    { "MMX Technology", "MMX" },
    { "FXSAVE/FXRSTOR Instructions", "FXSR" },
    { "SSE Extensions", "SSE" },
    { "SSE2 Extensions", "SSE2" },
    { "Self Snoop", "SS" },
    { "Hyper-threading Technology", "HTT" },
    { "Thermal Monitor", "TM" },
    { "Reserved 3", "R3!!" },                   /* Bit 30 */
    { "Pending Break Enable", "PBE" }
};

void dump_raw_cpuid( void )
{
    uint32_t    cpu_info[4];
    uint32_t    max_id;
    uint32_t    i;

    /* Obtain the number of standard CPUID leaves. */
    cpu_id( cpu_info, 0 );
    max_id = cpu_info[0];

    printf( "CPUID Information (%d leaves):\n", max_id + 1 );
    for( i = 0; i <= max_id; ++i ) {
        cpu_id( cpu_info, i );
        printf( "Leaf %02d: %08x %08x %08x %08x\n", i,
                cpu_info[0], cpu_info[1], cpu_info[2], cpu_info[3] );
    }

    /* Obtain the number of Extended CPUID leaves. */
    cpu_id( cpu_info, 0x80000000 );
    max_id = cpu_info[0] ? cpu_info[0] - 0x80000000 : 0;

    if( max_id ) {
        printf( "Extended CPUID Information (%d leaves):\n", max_id + 1 );
        for( i = 0; i <= max_id; ++i ) {
            cpu_id( cpu_info, 0x80000000 + i );
            printf( "Ext Leaf %02d: %08x %08x %08x %08x\n", i,
                    cpu_info[0], cpu_info[1], cpu_info[2], cpu_info[3] );
        }
    } else {
        printf( "No Extended CPUID Information.\n" );
    }
}

void print_cpuid( int verbose )
{
    uint32_t    cpu_info[4];
    uint32_t    max_id;
    uint32_t    max_ext_id;
    uint32_t    i;
    uint32_t    features;
    char        cpu_vendor[32];
    char        brand_string[12 * 4];

    /* Obtain the number of CPUID leaves and the vendor string. */
    cpu_id( cpu_info, 0 );
    max_id = cpu_info[0];
    memset( cpu_vendor, 0, sizeof( cpu_vendor ) );
    *(uint32_t *)(cpu_vendor + 0) = cpu_info[1];
    *(uint32_t *)(cpu_vendor + 4) = cpu_info[3];
    *(uint32_t *)(cpu_vendor + 8) = cpu_info[2];
    printf( "CPU vendor string: %s\n", cpu_vendor );

    /* Obtain the model and features. */
    cpu_id( cpu_info, 1 );
    features = cpu_info[3];

    /* Interpret the feature bits. */

    printf( "Processor features:\n" );
    for( i = 0; i < 32; ++i ) {
        if( features & (1 << i) ) {
            if( verbose )
                printf( "%s (%s)\n", feature_desc[i][0],
                        feature_desc[i][1] );
            else
                printf( "%s ", feature_desc[i][1] );
        }
    }
    if (!verbose )
        printf( "\n" );

    /* Obtain the brand string, if available. */
    cpu_id( cpu_info, 0x80000000 );
    max_ext_id = cpu_info[0] ? cpu_info[0] - 0x80000000 : 0;
    if( max_ext_id > 4 ) {
        for( i = 0; i < 3; ++i ) {
            cpu_id( cpu_info, 0x80000002 + i );
            memcpy( brand_string + i * sizeof( cpu_info ), 
                    cpu_info, sizeof( cpu_info ) );
        }
        /* Note that the brand string is null-terminated. */
        printf( "CPU brand string: %s\n", brand_string );
    }
}

int main( int argc, char **argv )
{
    int         c;
    int         verbose;
    int         raw_dump;

    verbose = raw_dump = 0;

    while( (c = getopt( argc, argv, ":rv" )) != -1 ) {
        switch( c ) {
        case 'r':
            raw_dump = 1;
            break;
        case 'v':
            verbose = 1;
            break;
        case '?':
            printf( "usage: %s -r -v\n", argv[0] );
            return( EXIT_FAILURE );
        }
    }

    if( raw_dump ) {
        dump_raw_cpuid();
    } else {
        print_cpuid( verbose );
    }
    return( EXIT_SUCCESS );
}
