#ifndef ATTACHEDPROPERTY_H
#define ATTACHEDPROPERTY_H

#include <stdio.h>
#include <stdlib.h>
#include "types.h"

typedef struct _attachedPropertyInfo
{
    PropertyType eType;
    char *pDefaultValue;
} AttachedPropertyInfo;

void attachedpropinfo_init_int(AttachedPropertyInfo *pInfo, int iDefaultValue);
void attachedpropinfo_init_str(AttachedPropertyInfo *pInfo, wchar_t *szDefaultValue);

void attachedpropinfo_destroy(AttachedPropertyInfo *pInfo);
#endif
