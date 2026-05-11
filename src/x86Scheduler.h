#ifndef X86SCHEDULER_H
#define X86SCHEDULER_H

#include <stdint.h>

#define MAX_PROCESSES 256
typedef uint8_t PID;

PID scheduleProcess(int (*process)());
int runNextProcess();

#endif
