#ifndef DCITEM_H
#define DCITEM_H

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include "lib/list.h"
#include "ngf.h"
#include "evtinfo.h"

typedef enum _dcItemType
{
    DcItemType_NoType,
    DcItemType_Str,
    DcItemType_Cmd,
} DcItemType;

typedef struct _dcItem
{
    DcItemType eType;
    char *pData;
    EventInfo ValueChangedEvent;
} DcItem;

typedef struct _strDcItem
{
    DcItem item;
} StrItem;

typedef struct _cmdItem
{
    DcItem item;
} CmdItem;

typedef void (*CommandFn)(char *pCmdParameter);

typedef struct _command
{
    CommandFn fnCommand;
} Command;

void dcitem_set_value(DcItem *pItem, char *pValue);
char* dcitem_get_value(DcItem *pItem);
void dcitem_sub_changed_evt(DcItem *pItem, EventCallback fnCallback, char *pEvtArg, size_t iArgSize);
void dcitem_raise_changed_evt(DcItem *pItem);

void stritem_init(StrItem *pItem);
void stritem_destroy(StrItem *pItem);
void stritem_destroy(StrItem *pItem);
void stritem_set(StrItem *pItem, wchar_t *pStr);
wchar_t* stritem_get(StrItem *pItem);

void cmditem_init(CmdItem *pItem);
void cmditem_destroy(CmdItem *pItem);
void cmditem_destroy(CmdItem *pItem);
void cmditem_set(CmdItem *pitem, CommandFn fnCommand);
CommandFn cmditem_get(CmdItem *pItem);

#endif
