#ifndef PROPERTY_H
#define PROPERTY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wchar.h>
#include "types.h"
#include "dcitem.h"

typedef struct _propertyInfo
{
    PropertyType eType;
    char *pValue;
} PropertyInfo;

void propinfo_init_int(PropertyInfo *pInfo, int iValue);
void propinfo_init_str(PropertyInfo *pInfo, wchar_t *pValue);
void propinfo_init_cmd(PropertyInfo *pInfo, CommandFn fnCommand);
void propinfo_init_ptr(PropertyInfo *pInfo, char *pDataPtr);
void propinfo_destroy(PropertyInfo *pInfo);

void propinfo_set(PropertyInfo *pInfo, char *pValue);
char* propinfo_get(PropertyInfo *pInfo);

void propinfo_bind(PropertyInfo *pInfo, DcItem *pItem);

#endif
