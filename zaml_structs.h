#ifndef ZAMLSTRUCTS_H
#define ZAMLSTRUCTS_H

#include <stdbool.h>
#include "lib/list.h"
#include "property.h"
#include "feinfo.h"
#include "treeviewinfo.h"
#include "tviteminfo.h"
#include "tbinfo.h"
#include "gridinfo.h"
#include "btninfo.h"
#include "borderinfo.h"
#include "types.h"

bool fe_add_child(FrameworkElement *pFe, FrameworkElement *pChildFe);
int zaml_get_children_count(FrameworkElement *pFe);
FrameworkElement* zaml_get_child(FrameworkElement *pFe, int iPos);

FeSize fe_get_size(FrameworkElement *pFe);
FePos fe_get_pos(FrameworkElement *pFe);
void fe_set_pos(FrameworkElement *pFe, FePos pos);

void fe_raise_event(FrameworkElement *pFe, EventType eType);

#endif
