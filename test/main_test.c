#include <assert.h>
#include <stdlib.h>
#include "reader.h"
#include "cpu.h"

int main(void)
{
    {
        cpuInfoS* cpu = cpu_newObject();

        cpu_refillCpuData(cpu);
        cpu_printCpuData(cpu, true);

        cpu_deleteObject(cpu);
    }
    return 0;
}
