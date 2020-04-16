#include "RC.h"

//��Դ��Ϣ�ṹ��
typedef struct
{
size_t data_offset;
size_t data_size;
size_t name_offset;
size_t name_size;
} RC_Info_t;

extern const unsigned char RC_Data[];
extern const unsigned char RC_Name[];
extern const RC_Info_t RC_Info[];
extern const size_t    RC_Info_Size;

//�����Ƚϵ�������ͬʱ����1
static unsigned char NameCmp(const char * Name1,const char *Name2)
{
    unsigned char ret=1;
    size_t i=0;
    while(1)
    {
        if(Name1[i]!=Name2[i])
            ret=0;
        if(Name1[i]=='\0' || Name2[i]=='\0')
            break;
        i++;
    }
    return ret;
}

//��ȡָ�����Ƶ���Դ��RC_Info,ʧ�ܷ���NULL
static const RC_Info_t * FindRCInfoByName(const char * Name)
{
    const RC_Info_t *ret=NULL;
    size_t i=0;
    for(i=0;i<RC_Info_Size;i++)
    {
        if(NameCmp(Name,(const char *)&RC_Name[RC_Info[i].name_offset]))
        {
            ret=&RC_Info[i];
            break;
        }
    }

    return ret;
}


//ͨ�����ƻ�ȡ��Դ��С
size_t RCGetSize(const char * name)
{
    size_t ret=0;
    const RC_Info_t * info=FindRCInfoByName(name);
    if(info!=NULL)
    {
        ret=info->data_size;
    }
    return ret;
}

//ͨ�����ƻ�ȡ��Դָ��
const unsigned char * RCGetHandle(const char * name)
{
    const unsigned char * ret=NULL;
    const RC_Info_t * info=FindRCInfoByName(name);
    if(info!=NULL)
    {
        ret=&RC_Data[info->data_offset];
    }
    return ret;
}


