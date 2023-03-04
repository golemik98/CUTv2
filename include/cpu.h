#ifndef CPU_H
#define CPU_H

#include <stdbool.h>

typedef struct cpuInfoS cpuInfoS;

cpuInfoS*   cpu_newObject();
void        cpu_deleteObject(cpuInfoS* cpuPtr);
void        cpu_printCpuData(cpuInfoS* cpuPtr, bool new);
void        cpu_refillCpuData(cpuInfoS* cpuPtr);
void        cpu_countUsage(cpuInfoS* cpuPtr);
#endif