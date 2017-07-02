#ifndef NGF_H
#define NGF_H

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include "lib/dict.h"
#include "lib/list.h"

typedef struct _frameworkElement
{
    int iType;
    char *pElement;
    Dict attachedProps;
} FrameworkElement;

typedef enum
{
    ITEMDATATYPE_STRING,
    ITEMDATATYPE_FUNC,
    ITEMDATATYPE_OBJECT,
} DcItemType;

typedef struct _dataContext
{
    //struct _eventHandler *pEvtHandler;
    Dict dict;
} DataContext;

/*
typedef void (*RaiseEvent)(Event *pEvt);

typedef struct _eventHandler
{
    RaiseEvent fnRaiseEvent;
} EventHandler;
*/

typedef void (*CommandFn)(DataContext*);

typedef struct _dataContextItem
{
    char *pData;
    size_t iDataSize;
    CommandFn fnCommand;
    DcItemType eDataType;
    SList observers;
} DcItem;

typedef struct _command
{
    char *pCmdParemter;
    CommandFn fnCommand;
} Command;

#endif
