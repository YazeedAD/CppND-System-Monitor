#include "ncurses_display.h"
#include "system.h"
#include <iostream>
#include "format.h"
#include "linux_parser.h"
#include <unistd.h>

using namespace std;
int main() {
  System system;
  NCursesDisplay::Display(system);

  // system
//  cout << system.MemoryUtilization() << endl;
//  cout << system.UpTime() << endl;
//  cout << system.TotalProcesses() << endl;
//  cout << system.RunningProcesses() << endl;
//  cout << system.Kernel() << endl;
//  cout << system.OperatingSystem() << endl;

////
//
//  // process
//    cout << Process(2310).UpTime() << endl;
    cout << Process(28090).CpuUtilization() << endl;
//    cout << LinuxParser::CpuUtilization(2764);
//    cout << Process(2310).Pid() << endl;
//    cout << Process(2310).User() << endl;
//    cout << Process(2310).Ram() << endl;
//    cout << Process(2310).Command() << endl;

//  for (unsigned int i =0 ; i< system.Processes().size(); i++)
//  {
//    cout << system.Processes()[i].Pid() << endl;
//
//  }
}