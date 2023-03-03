#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include "cpu.h"

// example template
static void cpu_create_test(void);
// static void cpu_empty_full_on_empty_test(void);
// static void cpu_single_enqueue_test(void);
// static void cpu_enqueue_to_full_test(void);
// static void cpu_single_dequeue_test(void);
// static void cpu_dequeue_to_empty_test(void);
// static void cpu_is_active_test(void);

static void cpu_create_test(void)
{
    {
        register const uint8_t coreNumber = 8;
        CpuAll* cpus = cpu_init(coreNumber);
        
        assert(cpus != NULL);
        assert(cpu_isActive(cpus) == true);
     
        assert(cpu_deleteObject(cpus) == true);
    }

    // {
    //     register const uint8_t coreNumber = 1;
    //     CpuAll* cpus = cpu_init(coreNumber);
        
    //     assert(cpus != NULL);
    //     assert(cpu_isActive(cpus) == true);
     
    //     assert(cpu_deleteObject(cpus) == true);
    // }
}

int main(void) //how to change this name to sth not main explicite
{
    cpu_create_test();
    return 0;
}
