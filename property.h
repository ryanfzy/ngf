#ifndef PROPERTY_H
#define PROPERTY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wchar.h>
#include "datacontext.h"

typedef struct _binding
{
    wchar_t *szKey;
    DataContext *pDc;
} BindingInfo;

typedef enum _properyType
{
    PropertyType_Null,
    PropertyType_Str, 
    PropertyType_Int,
} PropertyType;

typedef struct _propertyInfo
{
    BindingInfo *pBinding;
    PropertyType eType;
    char *pValue;
} PropertyInfo;

PropertyInfo* propinfo_create_ex(PropertyType eType);
void propinfo_free(PropertyInfo *pInfo);

//PropertyInfo* propinfo_create_ex(wchar_t *szKey, PropertyType eType);
//PropertyInfo* propinfo_create_str(wchar_t *szValue);

//wchar_t* propinfo_get_str(PropertyInfo *pInfo);
//bool propinfo_get_value(PropertyInfo *pInfo, DataContext *pDc, char *pValue);
void propinfo_set(PropertyInfo *pInfo, char *pValue);
bool propinfo_get(PropertyInfo *pInfo, char *pValue);

void propinfo_set_binding(PropertyInfo *pInfo, BindingInfo *pBinding);

BindingInfo* binding_create(wchar_t *szKey, DataContext *pDc);
void binding_set(BindingInfo *pInfo, PropertyType eType, char *pValue);
bool binding_get(BindingInfo *pInfo, PropertyType eType, char *pValue);

#endif
