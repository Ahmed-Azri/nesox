#include "logger.h"

int main(){

logopen("loggertest.log");

sleep(2);

logtrace("%s", "Hello, LOG!!!!Hello, LOG!!!!Hello, LOG!!!!Hello, LOG!!!!Hello, LOG!!!!Hello, LOG!!!!Hello, LOG!!!!Hello, LOG!!!!Hello, LOG!!!!");
logerror("%s", "Hello, LOG!!!!");
logstats("%s", "Hello, LOG!!!!");

LOG("%s", "How about this macro?");

logclose();

return 0; }
