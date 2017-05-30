#ifndef NGF_H
#define NGF_H

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include "lib/dict.h"
#include "lib/list.h"

typedef enum
{
    ITEMDATATYPE_STRING,
    ITEMDATATYPE_FUNC,
    ITEMDATATYPE_OBJECT,
} DcItemType;

typedef enum
{
    EVENTTYPE_UPDATE,
} EventType;

typedef struct _event
{
    char *pObserver;
    EventType eEvtType;
} Event;

typedef struct _dataContext
{
    struct _eventHandler *pEvtHandler;
    Dict dict;
} DataContext;

typedef void (*RaiseEvent)(Event *pEvt);

typedef struct _eventHandler
{
    RaiseEvent fnRaiseEvent;
} EventHandler;

typedef void (*CommandFn)(DataContext*);

typedef struct _dataContextItem
{
    char *pData;
    size_t iDataSize;
    CommandFn fnCommand;
    DcItemType eDataType;
    SList observers;
} DcItem;

#endif
