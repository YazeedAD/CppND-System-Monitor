#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

bool CompCPU(Process p1, Process p2)
{
  return p1.CpuUtilization() > p2.CpuUtilization();
}

// DONE: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  vector<Process> processes_vector;
  for(unsigned long i=0; i < LinuxParser::Pids().size(); i++)
  {
    processes_vector[i] = Process(LinuxParser::Pids()[i]);
  }

  std::sort(processes_vector.begin(), processes_vector.end(), CompCPU);


  return processes_ ; }

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() {
  return LinuxParser::Kernel();
}

// DONE: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// DONE: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// DONE: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// DONE: Return the total number of processes on the system
// Doc
int System::TotalProcesses() { return LinuxParser::Pids().size(); }

// DONE: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }

