#ifndef DATACONTEXT_H
#define DATACONTEXT_H

#include <stdio.h>
#include <stdlib.h>
#include "lib/list.h"
#include "lib/dict.h"
#include "ngf.h"

DataContext* create_datacontext();
DataContext* create_dc_ex(EventHandler *pEvtHandler);
void free_datacontext(DataContext *pDc);

void DataContext_add_str(DataContext *pDc, char *szKey, char *szStr, size_t iLen);
void DataContext_add_command(DataContext *pDc, char *szKey, CommandFn fnCommand);
void DataContext_add_object(DataContext *pDc, char *szKey, char *pData, size_t iSize);

bool DataContext_set_str(DataContext *pDc, char *szKey, char *szStr, size_t iLen);
void DataContext_run_command(DataContext *pDc, char *szKey);

size_t DataContext_get_str(DataContext *pDc, char *szKey, char **szStr);
char* DataContext_get_object(DataContext *pDc, char *szKey);

void DataContext_update(DataContext *pDc, char *szKey);
//void DataContext_observe(DataContext *pDc, char *szKey, HWND hWnd);
void DataContext_observe(DataContext *pDc, char *szKey, char *pObserver, size_t iObserverSize);

#endif
