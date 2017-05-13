#ifndef DATACONTEXT_H
#define DATACONTEXT_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "lib/list.h"
#include "lib/dict.h"

typedef enum
{
    ITEMDATATYPE_STRING,
    ITEMDATATYPE_FUNC,
    ITEMDATATYPE_OBJECT,
} DcItemType;

typedef struct _dataContext
{
    Dict dict;
} DataContext;

typedef void (*CommandFn)(DataContext*);

typedef struct _dataContextItem
{
    char *szKey;
    char *pData;
    size_t iDataSize;
    //void (*fnCommand)(DataContext*);
    CommandFn fnCommand;
    DcItemType eDataType;
    SList observers;
} DcItem;

DataContext* create_datacontext();
void free_datacontext(DataContext *pDc);

void DataContext_add_str(DataContext *pDc, char *szKey, char *szStr, size_t iLen);
void DataContext_add_command(DataContext *pDc, char *szKey, CommandFn fnCommand);
void DataContext_add_object(DataContext *pDc, char *szKey, char *pData, size_t iSize);

bool DataContext_set_str(DataContext *pDc, char *szKey, char *szStr, size_t iLen);
void DataContext_run_command(DataContext *pDc, char *szKey);

size_t DataContext_get_str(DataContext *pDc, char *szKey, char **szStr);
char* DataContext_get_object(DataContext *pDc, char *szKey);

void DataContext_update(DataContext *pDc, char *szKey);
void DataContext_observe(DataContext *pDc, char *szKey, HWND hWnd);

#endif
