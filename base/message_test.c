#include "message.h"


int main(){

char data[0xff] = "Hello, message module!!";

message m;
messageinit(ECHO, sizeof(data), &m);

char messagestring[numdigitsmssg];
int num = snprintf(messagestring, sizeof(messagestring), "%s", encode(&m));
fprintf(stderr, "encoded message[%2lu]: %s\n", strlen(messagestring), messagestring);

message x = decode(messagestring);

return 0; }
