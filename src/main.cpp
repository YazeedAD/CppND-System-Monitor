#include "ncurses_display.h"
#include "system.h"


using namespace std;
int main() {
  System system;
  NCursesDisplay::Display(system);

}