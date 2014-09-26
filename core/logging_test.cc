#include "logging.h"

int main() {
  InitializeLogger("/opt/nesox/info.log", "/opt/nesox/warn.log", "/opt/nesox/erro.log");
  LOG(INFO)    << "An info message going into /tmp/info.log";
  LOG(WARNING) << "An warn message going into /tmp/warn.log";
  LOG(ERROR)   << "An erro message going into /tmp/erro.log";
  LOG(FATAL)   << "An fatal message going into /tmp/erro.log, "
               << "and kills current process by a segmentation fault.";
  return 0;
}
