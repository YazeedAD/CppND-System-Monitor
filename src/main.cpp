#include "ncurses_display.h"
#include "system.h"
#include <iostream>
#include "format.h"
#include "linux_parser.h"
#include <unistd.h>

using namespace std;
int main() {
  System system;
//  cout << system.Cpu().Utilization() ;
//  cout << LinuxParser::RunningProcesses();
  NCursesDisplay::Display(system);

//  while (1)
//  {
//    cout << Format::ElapsedTime(system.UpTime()) << "\n";
//    sleep(1);
//  }
}