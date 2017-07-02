#ifndef DATACONTEXT_H
#define DATACONTEXT_H

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include "lib/list.h"
#include "lib/dict.h"
#include "ngf.h"

DataContext* create_datacontext();
//DataContext* create_dc_ex(EventHandler *pEvtHandler);
void free_datacontext(DataContext *pDc);

void DataContext_add_str(DataContext *pDc, wchar_t *szKey, wchar_t *szStr);
bool DataContext_set_str(DataContext *pDc, wchar_t *szKey, wchar_t *szStr);
size_t DataContext_get_str(DataContext *pDc, wchar_t *szKey, wchar_t **szStr);

void DataContext_add_command(DataContext *pDc, wchar_t *szKey, CommandFn fnCommand);
void DataContext_run_command(DataContext *pDc, wchar_t *szKey);

void DataContext_add_object(DataContext *pDc, wchar_t *szKey, char *pData, size_t iSize);
char* DataContext_get_object(DataContext *pDc, wchar_t *szKey);

//void DataContext_update(DataContext *pDc, wchar_t *szKey);
void DataContext_observe(DataContext *pDc, wchar_t *szKey, char *pObserver, size_t iObserverSize);

#endif
