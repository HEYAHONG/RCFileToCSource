#include <iostream>
#include "RC.h"
#include "string.h"
using namespace std;

int main()
{
    uint8_t * data=new uint8_t[RCGetSize("test.txt")+1];
    memset(data,0,RCGetSize("test.txt")+1);
    memcpy(data,RCGetHandle("test.txt"),RCGetSize("test.txt"));
    cout << data;

    return 0;
}
