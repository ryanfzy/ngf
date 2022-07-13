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

#endif
