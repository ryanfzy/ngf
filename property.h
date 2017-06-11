#ifndef PROPERTY_H
#define PROPERTY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wchar.h>
#include "types.h"
#include "datacontext.h"

typedef struct _binding
{
    wchar_t *szKey;
    DataContext *pDc;
} BindingInfo;

typedef struct _propertyInfo
{
    BindingInfo *pBinding;
    PropertyType eType;
    char *pValue;
} PropertyInfo;

PropertyInfo* propinfo_create_ex(PropertyType eType);
void propinfo_free(PropertyInfo *pInfo);

void propinfo_set(PropertyInfo *pInfo, char *pValue);
bool propinfo_get(PropertyInfo *pInfo, char *pValue);

void propinfo_set_binding(PropertyInfo *pInfo, BindingInfo *pBinding);
void propinfo_observe(PropertyInfo *pInfo, char *pObserver, size_t iObserverSize);

BindingInfo* binding_create(wchar_t *szKey, DataContext *pDc);

void binding_set(BindingInfo *pInfo, PropertyType eType, char *pValue);
bool binding_get(BindingInfo *pInfo, PropertyType eType, char *pValue);

#endif
