#ifndef ZAMLSTRUCTS_H
#define ZAMLSTRUCTS_H

#include <stdbool.h>
#include "lib/list.h"
#include "datacontext.h"
#include "property.h"
#include "feinfo.h"
#include "treeviewinfo.h"
#include "tviteminfo.h"
#include "tbinfo.h"
#include "gridinfo.h"
#include "btninfo.h"
#include "borderinfo.h"

bool zaml_add_child(FrameworkElement *pParentFe, FrameworkElement *pChildFe);
int zaml_get_children_count(FrameworkElement *pFe);
FrameworkElement* zaml_get_child(FrameworkElement *pFe, int iPos);

FeSize fe_get_size(FrameworkElement *pFe);
FePos fe_get_pos(FrameworkElement *pFe);
void fe_set_pos(FrameworkElement *pFe, FePos pos);

#endif
