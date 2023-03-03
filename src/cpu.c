#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "cpu.h"
#include "logger.h"

#define USER_IDX    0
#define NICE_IDX    1
#define SYSTEM_IDX  2
#define IDLE_IDX    3
#define IOWAIT_IDX  4
#define IRQ_IDX     5
#define SOFTIRQ_IDX 6
#define STEAL_IDX   7
#define GUEST_IDX   8
#define GUEST_N_IDX 9
#define DATA_QTY    10

#define MAGIC_NUMBER 0xDEADBEEF

typedef struct cpuInfoS
{
    uint32_t magicNumber;
    uint8_t index;
    uint32_t data[10];
    uint32_t prevData[10];
}cpuInfoS;

typedef struct CpuAll
{
    uint32_t magicNumber;
    uint8_t coreQty;

    cpuInfoS* singleCore[];
}CpuAll;

CpuAll* cpu_init(uint8_t coreQty)
{
    if (coreQty == 0)
    {
        return NULL;
    }

    CpuAll* const cpus = calloc(1, sizeof(*cpus) + sizeof(cpuInfoS) * coreQty);
    if (cpus == NULL)
    {
        return NULL;
    }
    
    cpus->coreQty = coreQty;
    cpus->magicNumber = MAGIC_NUMBER;

    return cpus;
}

bool cpu_deleteObject(CpuAll* cpus)
{
    if (cpus == NULL || !cpu_isActive(cpus))
    {
        logger_debug("Wrong obj to free\n");
        return false;
    }

    free(cpus);
    return true;
}

bool cpu_isActive(CpuAll* cpu)
{
    if (cpu == NULL)
        return false;

    return cpu->magicNumber == MAGIC_NUMBER;    
}

bool cpu_printCpuData(cpuInfoS* cpuPtr, bool new)
{
    if (cpuPtr == NULL)
    {
        return false;
    }
    
    printf("cpu[%d]: ", cpuPtr->index);
    for (size_t i = 0; i < DATA_QTY; i++)
    {
        (new) ? printf("%u ", cpuPtr->data[i]) : printf("%u ", cpuPtr->prevData[i]);
    }
    printf("\n");
    
    return true;
}

bool cpu_refillCpuData(cpuInfoS* cpuPtr)
{
    FILE* file;
    char strings[10][12] = {0};
    file = fopen("/proc/stat", "r");

    if (file == NULL)
    {
        return false;
    }

    int result = fscanf(file, "%*s %10s %10s %10s %10s %10s %10s %10s %10s %10s %10s", 
    strings[USER_IDX],
    strings[NICE_IDX],
    strings[SYSTEM_IDX],
    strings[IDLE_IDX],
    strings[IOWAIT_IDX],
    strings[IRQ_IDX],
    strings[SOFTIRQ_IDX],
    strings[STEAL_IDX],
    strings[GUEST_IDX],
    strings[GUEST_N_IDX]);

    if (result == 0)
    {
        fclose(file);
        return false;
    }
    fclose(file);

    memcpy(&cpuPtr->prevData, &cpuPtr->data, sizeof(cpuPtr->data));
    for (size_t i = 0; i < DATA_QTY; i++)
    {
        cpuPtr->data[i] = atoi(strings[i]);
    }

    return true;
}

bool cpu_countUsage(cpuInfoS* cpuPtr)
{
    uint32_t prevIdle = cpuPtr->prevData[IDLE_IDX] + cpuPtr->prevData[IOWAIT_IDX];
    uint32_t idle = cpuPtr->data[IDLE_IDX] + cpuPtr->data[IOWAIT_IDX];

    uint32_t prevNonIdle = cpuPtr->prevData[USER_IDX] + 
        cpuPtr->prevData[NICE_IDX] + 
        cpuPtr->prevData[SYSTEM_IDX] + 
        cpuPtr->prevData[IRQ_IDX] + 
        cpuPtr->prevData[SOFTIRQ_IDX] + 
        cpuPtr->prevData[STEAL_IDX];
    uint32_t nonIdle = cpuPtr->data[USER_IDX] + 
        cpuPtr->data[NICE_IDX] + 
        cpuPtr->data[SYSTEM_IDX] + 
        cpuPtr->data[IRQ_IDX] + 
        cpuPtr->data[SOFTIRQ_IDX] + 
        cpuPtr->data[STEAL_IDX];

    uint32_t prevTotal = prevIdle + prevNonIdle;    
    uint32_t total = idle + nonIdle;

    uint32_t totald = total - prevTotal;
    uint32_t idled = idle - prevIdle;

    if (totald == 0)
    {
        return false;
    }
    
    uint8_t cpuPercentageUsage = (totald - idled) / totald;

    printf("Usage in %d%%\n", cpuPercentageUsage);

    return true;
}