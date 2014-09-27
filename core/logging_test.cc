#include "logging.h"

int main() {
  InitializeLogger("../logs/info.log", "../logs/warn.log", "../logs/erro.log");
  LOG(INFO)    << "An info message going into info.log";
  return 0;
}
