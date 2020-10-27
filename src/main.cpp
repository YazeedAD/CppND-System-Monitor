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
//  NCursesDisplay::Display(system);
//    cout << Process(2310).Command();
//    cout << LinuxParser::Uid(2310);
//    cout << Process(2310).UpTime();
      cout << LinuxParser::User(2310);
//  while (1)
//  {
//    cout << Format::ElapsedTime(system.UpTime()) << "\n";
//    sleep(1);
//  }
}