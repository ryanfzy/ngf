#ifndef TYPES_H
#define TYPES_H

static const int FE_WINDOW = 1;
static const int FE_GRID = 2;
static const int FE_BUTTON = 3;
static const int FE_TEXTBLOCK = 4;
static const int FE_BORDER = 5;
static const int FE_TREEVIEW = 6;
static const int FE_TVITEM = 7;
static const int FE_STACKPANEL = 8;

static const int SIZE_AUTO = -1;
static const int SIZE_UNDEFINED = -2;

typedef enum _properyType
{
    PropertyType_NoType,
    PropertyType_Str, 
    PropertyType_Int,
    PropertyType_Cmd,
    PropertyType_Binding,
    PropertyType_Ptr,
} PropertyType;

typedef enum _horizontalAlignmentType
{
    HorizontalAlignment_NoType,
    HorizontalAlignment_Left,
    HorizontalAlignment_Center,
    HorizontalAlignment_Right,
} HorizontalAlignmentType;

typedef enum _orientationType
{
    Orientation_Vertical,
    Orientation_Horizontal,
} OrientationType;

typedef enum _eventType
{
    EventType_Click,
    EventType_PropertyChanged,
    EventType_DcItemChanged,
} EventType;

#endif
