#ifndef __RC_H__
#define __RC_H__
#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"
#ifdef __cplusplus
extern "C"
{
#endif

//ͨ�����ƻ�ȡ��Դ��С
size_t RCGetSize(const char * name);

//ͨ�����ƻ�ȡ��Դָ��
const unsigned char * RCGetHandle(const char * name);

#ifdef __cplusplus
};
#endif
#endif // __RC_H__
