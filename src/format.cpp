#include "format.h"

#include <string>
#include <iomanip>

using std::string;

string Format::ElapsedTime(long seconds [[maybe_unused]]) {
  // Doc
  long h=0;
  long m=0;
  long s=0;
  char uptime[60];
  h = seconds / 3600;
  m = (seconds % 3600) / 60;
  s = (seconds % 3600) % 60;
  std::sprintf(uptime,"%02ld:%02ld:%02ld",h, m ,s);
  return uptime;
}