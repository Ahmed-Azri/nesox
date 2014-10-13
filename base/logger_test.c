#include "logger.h"

int main(){

logopen("loggertest.log");
logprintf("Hello, Nesox Logger!\n");
logsend();
LOG("Hello, LOG!");
logclose();

return 0; }
