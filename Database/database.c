#include "database.h"
#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"


typedef enum{
    kcfgtime,
    kcfguserName,
    kcfgacidName,
} cfg_field_type;


typedef enum{
    kcfgint32,
    kcfgfloat,
    kcfgstr,
} cfg_data_type;

//字段属性 字段名，数据类型，信息
typedef struct{
    cfg_field_type field_type;
    cfg_data_type type;
    const char* info;
} cfg_field_info;

//记录属性：id和4个字段
typedef struct{
    uint32_t index;
    cfg_field_info record[4];
} field_record_t;


//表属性，表大小，指向第一条记录
typedef struct{
    uint32_t size;
    field_record_t* head; 
}cfg_surface_t;



typedef struct{
    int32_t head;
    int32_t checksum;
    field_record_t cfg_info[10];
} cfg_cache_t;


//记录数组
static field_record_t database_info[] = {
    {0, {{kcfgtime, kcfgint32, "0"},{kcfgtime, kcfgstr, "1"}, {kcfgtime, kcfgstr, "2"}, {kcfgtime, kcfgint32, "3"}}},
    {1, {{kcfgtime, kcfgint32, ""},{kcfgtime, kcfgstr, ""}, {kcfgtime, kcfgstr, ""}, {kcfgtime, kcfgint32, ""}}},
    {2, {{kcfgtime, kcfgint32, ""},{kcfgtime, kcfgstr, ""}, {kcfgtime, kcfgstr, ""}, {kcfgtime, kcfgint32, ""}}},
    {3, {{kcfgtime, kcfgint32, ""},{kcfgtime, kcfgstr, ""}, {kcfgtime, kcfgstr, ""}, {kcfgtime, kcfgint32, ""}}},
    {4, {{kcfgtime, kcfgint32, ""},{kcfgtime, kcfgstr, ""}, {kcfgtime, kcfgstr, ""}, {kcfgtime, kcfgint32, ""}}},
    {5, {{kcfgtime, kcfgint32, ""},{kcfgtime, kcfgstr, ""}, {kcfgtime, kcfgstr, ""}, {kcfgtime, kcfgint32, ""}}},
    {6, {{kcfgtime, kcfgint32, "3"},{kcfgtime, kcfgstr, ""}, {kcfgtime, kcfgstr, ""}, {kcfgtime, kcfgint32, ""}}},
    {7, {{kcfgtime, kcfgint32, ""},{kcfgtime, kcfgstr, ""}, {kcfgtime, kcfgstr, ""}, {kcfgtime, kcfgint32, ""}}},
    {8, {{kcfgtime, kcfgint32, ""},{kcfgtime, kcfgstr, ""}, {kcfgtime, kcfgstr, ""}, {kcfgtime, kcfgint32, ""}}},
    {9, {{kcfgtime, kcfgint32, ""},{kcfgtime, kcfgstr, ""}, {kcfgtime, kcfgstr, ""}, {kcfgtime, kcfgint32, ""}}},
};


static cfg_cache_t cfgCaChe;
static  cfg_surface_t audit;
int main(void){
    audit.size = sizeof(database_info)/sizeof(field_record_t);
    audit.head = database_info;

    for(int i = 0; i < 10; i++){
        audit.head[i].index =  i;
        memcpy(&(cfgCaChe.cfg_info[i]), &audit.head[i], sizeof(field_record_t));
    }

    for(int j = 0; j < 4; j++)
    printf("%s",cfgCaChe.cfg_info[6].record[j].info);
}