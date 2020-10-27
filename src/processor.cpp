#include "processor.h"
#include <ctime>
#include <unistd.h>
#include "linux_parser.h"

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
  long c_idle=0;
  long c_active=0;
  long p_idle=0;
  long p_active=0;
  long p_total=0;
  long c_total=0;
  long d_total=0;
  long d_idle=0;

  float cpu_utl=0;
  int micro_seconds = 100000;

  p_idle = LinuxParser::IdleJiffies();
  p_active = LinuxParser::ActiveJiffies();
  p_total = p_idle + p_active;

  // Delta delay
  // TODO: find non-blocking method (cash or multithreading)
  usleep(micro_seconds);

  c_idle = LinuxParser::IdleJiffies();
  c_active = LinuxParser::ActiveJiffies();
  c_total = c_idle + c_active;

  d_total = c_total - p_total;
  d_idle = c_idle - p_idle;

  cpu_utl = (float)(d_total - d_idle) / (float)d_total;

  return cpu_utl;

}