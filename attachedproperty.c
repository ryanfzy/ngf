#include <string.h>
#include "attachedproperty.h"

static void attachedpropinfo_init(AttachedPropertyInfo *pInfo, PropertyType eType, char *pValue, size_t iSize)
{
    if (pInfo != NULL && pValue != NULL && iSize > 0)
    {
        pInfo->eType = eType;
        pInfo->pDefaultValue = malloc(iSize);
        memset(pInfo->pDefaultValue, 0, iSize);
        memcpy(pInfo->pDefaultValue, pValue, iSize);
    }
}

void attachedpropinfo_init_int(AttachedPropertyInfo *pInfo, int iDefaultValue)
{
    if (pInfo != NULL)
        attachedpropinfo_init(pInfo, PropertyType_Int, (char*)(&iDefaultValue), sizeof(int));
}

void attachedpropinfo_init_str(AttachedPropertyInfo *pInfo, wchar_t *szDefaultValue)
{
    // todo
}

void attachedpropinfo_destroy(AttachedPropertyInfo *pInfo)
{
    free(pInfo->pDefaultValue);
}
