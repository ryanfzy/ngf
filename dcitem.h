#ifndef DCITEM_H
#define DCITEM_H

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include "lib/list.h"
#include "ngf.h"

typedef struct _dcItem
{
    char *pData;
    SList observers;
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

void dcitem_set_value(DcItem *pItem, char *pValue, size_t iSize);
char* dcitem_get_value(DcItem *pItem);

void stritem_init(StrItem *pItem);
void stritem_destroy(StrItem *pItem);
void stritem_set_value(StrItem *pItem, wchar_t *pStr);
wchar_t* strItem_get_value(StrItem *pItem);
void stritem_add_observer(StrItem *pItem, FrameworkElement *pFe);

void cmditem_init(CmdItem *pItem);
void cmditem_destroy(CmdItem *pItem);
void cmditem_set_value(CmdItem *pitem, CommandFn fnCommand);
CommandFn cmditem_get_value(CmdItem *pItem);
void cmditem_add_observer(CmdItem *pItem, FrameworkElement *pFe);

#endif
