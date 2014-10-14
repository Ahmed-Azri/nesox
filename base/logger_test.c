#include "logger.h"

int main(){

logopen("loggertest.log");

LOG("Hello, LOG!");
logprintf("Hello, Nesox Logger!\n");
logprintf("Hello, Nesox Logger!\n");
logprintf("Hello, Nesox Logger!\n");
logprintf("Hello, Nesox Logger!\n");
logprintf("Hello, Nesox Logger!\n");
logprintf("Hello, Nesox Logger!\n");

sleep(2);

logprintf("Hello, Nesox Logger!\n");
logprintf("Hello, Nesox Logger!\n");
logprintf("Hello, Nesox Logger!\n");
logprintf("Hello, Nesox Logger!\n");
logprintf("Hello, Nesox Logger!\n");
logprintf("%s\n", "OK???");
logprintf("%d\n", 1000);
printf("%s:%s:%d\n", __FILE__, __FUNCTION__, __LINE__);
logsend();

LOG("Hello, LOG!\n");


logclose();

return 0; }
