#ifndef CPU_H
#define CPU_H

#include <stdbool.h>
#include <stdint.h>

typedef struct cpuInfoS cpuInfoS;
typedef struct CpuAll CpuAll;

CpuAll*     cpu_init(uint8_t coreQty);
bool        cpu_deleteObject(CpuAll* cpu);
bool        cpu_isActive(CpuAll* cpu);
bool        cpu_printCpuData(cpuInfoS* cpuPtr, bool new);
bool        cpu_refillCpuData(cpuInfoS* cpuPtr);
bool        cpu_countUsage(cpuInfoS* cpuPtr);
bool        cpu_isActive(CpuAll* cpu);
#endif