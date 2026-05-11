#include "x86Scheduler.h"
#include "vgaTerminal.h"
#include "util.h"

int (*processes[MAX_PROCESSES])();
PID processCount = 0;
PID currentProcess = 0;

PID scheduleProcess(int (*process)())
{
  PID pid;
  if (processCount < sizeof(processCount))
  {
    pid = processCount;
    processes[pid] = process;
    pid++;
  }

  return pid;
}

int runNextProcess()
{ 
  if (processes[currentProcess] != 0)
  {
    char str[256];
    terminalWriteStr("Running process: ");
    intToAscii(currentProcess, str);
    terminalWriteStr(str);
    terminalWriteStr("\n");
    return processes[currentProcess++]();
  }

  return -1;
}
