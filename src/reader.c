#include "reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reader_readProcStat(void)
{
    FILE* ptr;
    char buffer[100] = {0};
 
    ptr = fopen(PROC_STAT_PATH, "r");
 
    if (NULL == ptr) 
    {
        printf("file can't be opened \n");
        return;
    }

    (void)fscanf(ptr, "%*s %s", buffer);

    printf("\nReaded:%s\n", buffer);

    fclose(ptr);
}

void reader_printFile(void)
{
    FILE* file;
    char ch;

    file = fopen(PROC_STAT_PATH, "r");
    if (NULL == file) 
    {
        printf("file can't be opened \n");
        return;
    }

    while (feof(file) == 0)
    {
        ch = fgetc(file);
        printf("%c", ch);
    }

    fclose(file);

}