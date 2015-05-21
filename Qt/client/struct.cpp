#include "struct.h"

#include "cJSON.h"

#include <string>

InitInfo::InitInfo()
{
    valid = false;
}

void InitInfo::parse(void *src)
{
    printf("json:%s\n", (char*)src);
    cJSON* json = cJSON_Parse((char*)src);
    if(!json)
    {
        valid = false;
        return;
    }

    cJSON* jw = cJSON_GetObjectItem(json, "width");
    if(!jw)
    {
        valid = false;
        return;
    }
    video_width = jw->valueint;

    cJSON* jh = cJSON_GetObjectItem(json, "height");
    if(!jh)
    {
        valid = false;
        return;
    }
    video_height = jh->valueint;

    valid = true;
    cJSON_Delete(json);
}

void InitInfo::serialize(void *dst)
{
    cJSON* json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "width", video_width);
    cJSON_AddNumberToObject(json, "height", video_height);
    char* jstr = cJSON_PrintUnformatted(json);
    strcpy((char*)dst, jstr);
    free(jstr);
    cJSON_Delete(json);
}
