#include "system.h"
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

Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() {
  vector<Process> processes_vector;
  for(unsigned long i=0; i < LinuxParser::Pids().size(); i++)
  {
    processes_vector.emplace_back(LinuxParser::Pids()[i]);

  }

  std::sort(processes_vector.begin(), processes_vector.end(), CompCPU);
  processes_ = processes_vector;
  return processes_ ; }

std::string System::Kernel() {
  return LinuxParser::Kernel();
}

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }


int System::TotalProcesses() { return LinuxParser::Pids().size(); }

long int System::UpTime() { return LinuxParser::UpTime(); }

