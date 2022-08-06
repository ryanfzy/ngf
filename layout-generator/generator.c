#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "xml\xmlparser.h"
#include "tree.h"
#include "list.h"

#define BINDING_PREFIX "{binding "

#define WINDOW "window"
#define GRID "grid"
#define GRID_ROWDEFINITION "grid.rowdefinition"
#define ROWDEFINITION "rowdefinition"
#define BORDER "border"
#define TEXTBLOCK "textblock"
#define BUTTON "button"
#define GRID_COLUMNDEFINITION "grid.columndefinition"
#define COLUMNDEFINITION "columndefinition"

#define NAME "name"
#define HEIGHT "height"
#define WIDTH "width"
#define HORIZONTALALIGNMENT "horizontalalignment"
#define CONTENT "content"
#define TEXT "text"
#define COMMAND "command"
#define AUTO "auto"
#define LEFT "left"
#define RIGHT "right"

#define GRID_ROW "grid.row"
#define GRID_COLUMN "grid.column"
#define GRID_COLSPAN "grid.colspan"
#define GRID_ROWSPAN "grid.rowspan"

#define VAL_AUTO -1

#define NAME_SIZE 256

static FILE *fp;
static Tree *ui_tree;
static Tree *ui_current;

int grid_n = 0;
int rowdef_n = 0;
int coldef_n = 0;
int border_n = 0;
int textblock_n = 0;
int button_n = 0;

typedef enum _UiType
{
    UiType_Error,
    UiType_Window,
    UiType_Grid,
    UiType_Grid_Rowdefinition,
    UiType_Rowdefinition,
    UiType_Border,
    UiType_Textblock,
    UiType_Button,
    UiType_Grid_Columndefinition,
    UiType_Columndefinition,
} UiType;

typedef enum _UiPropType
{
    UiPropType_GridRow,
    UiPropType_GridCol,
    UiPropType_GridColSpan,
    UiPropType_GridRowSpan,
} UiPropType;

typedef enum _HoriAlignType
{
    HoriAlignType_Left,
    HoriAlignType_Right,
} HoriAlignType;

typedef struct _UiProp
{
    UiPropType type;
    union
    {
        int ival;
    };
} UiProp;

typedef enum _AttrValueType
{
    AttrValueType_None,
    AttrValueType_Const,
    AttrValueType_Binding,
} AttrValueType;

typedef struct _BindingType
{
    char name[NAME_SIZE];
} BindingType;

typedef struct _AttrValue
{
    AttrValueType type;
    union
    {
        BindingType binding;
    };
} AttrValue;

static void attr_value_init(AttrValue *attr)
{
    memset(attr, 0, sizeof(AttrValue));
    attr->type = AttrValueType_None;
}

typedef struct _UiComponent
{
    UiType type;
    size_t size;
    int height;
    int width;
    char name[NAME_SIZE];
    List props;
} UiComponent;

typedef struct _UiWindow
{
    UiComponent base;
} UiWindow;

typedef struct _UiGrid
{
    UiComponent base;
} UiGrid;

typedef struct _UiGridRowdefinition
{
    UiComponent base;
} UiGridRowdefinition;

typedef struct _UiRowdefinition
{
    UiComponent base;
} UiRowdefinition;

typedef struct _UiBorder
{
    UiComponent base;
} UiBorder;

typedef struct _UiTextblock
{
    UiComponent base;
    HoriAlignType halign;
    AttrValue text;
} UiTextblock;

typedef struct _UiButton
{
    UiComponent base;
    char content[NAME_SIZE];
    AttrValue command;
} UiButton;

typedef struct _UiGridColumndefinition
{
    UiComponent base;
} UiGridColumndefinition;

typedef struct _UiColumndefinition
{
    UiComponent base;
    int width;
} UiColumndefinition;

static UiType _get_type_by_name(char *type_name)
{
    if (strncmp(type_name, WINDOW, sizeof(WINDOW)) == 0)
        return UiType_Window;
    else if (strncmp(type_name, GRID, sizeof(GRID)) == 0)
        return UiType_Grid;
    else if (strncmp(type_name, GRID_ROWDEFINITION, sizeof(GRID_ROWDEFINITION)) == 0)
        return UiType_Grid_Rowdefinition;
    else if (strncmp(type_name, ROWDEFINITION, sizeof(ROWDEFINITION)) == 0)
        return UiType_Rowdefinition;
    else if (strncmp(type_name, BORDER, sizeof(BORDER)) == 0)
        return UiType_Border;
    else if (strncmp(type_name, TEXTBLOCK, sizeof(TEXTBLOCK)) == 0)
        return UiType_Textblock;
    else if (strncmp(type_name, BUTTON, sizeof(BUTTON)) == 0)
        return UiType_Button;
    else if (strncmp(type_name, GRID_COLUMNDEFINITION, sizeof(GRID_COLUMNDEFINITION)) == 0)
        return UiType_Grid_Columndefinition;
    else if (strncmp(type_name, COLUMNDEFINITION, sizeof(COLUMNDEFINITION)) == 0)
        return UiType_Columndefinition;
    return UiType_Error;
}

static void ui_base_init(UiComponent *comp, UiType type, size_t size, char *name, int postfix)
{
    comp->type = type;
    comp->size = size;
    comp->width = VAL_AUTO;
    comp->height = VAL_AUTO;
    memset(comp->name, 0, NAME_SIZE);
    memcpy(comp->name, name, strlen(name));
    sprintf(comp->name+strlen(name), "%d", postfix);
    list_init(&comp->props);
}

static UiComponent* create_ui_window()
{
    UiComponent *comp = (UiComponent*)malloc(sizeof(UiWindow));
    ui_base_init(comp, UiType_Window, sizeof(UiWindow), "noname", 0);
    return comp;
}

static UiComponent* create_ui_grid()
{
    UiComponent *comp = (UiComponent*)malloc(sizeof(UiGrid));
    ui_base_init(comp, UiType_Grid, sizeof(UiGrid), GRID, grid_n++);
    return comp;
}

static UiComponent* create_ui_grid_rowdef()
{
    UiComponent *comp = (UiComponent*)malloc(sizeof(UiGridRowdefinition));
    ui_base_init(comp, UiType_Grid_Rowdefinition, sizeof(UiGridRowdefinition), "noname", 0);
    return comp;
}

static UiComponent* create_ui_rowdef()
{
    UiRowdefinition *comp = malloc(sizeof(UiRowdefinition));
    ui_base_init((UiComponent*)comp, UiType_Rowdefinition, sizeof(UiRowdefinition), "rowdef", rowdef_n++);
    comp->base.height = 0;
    return (UiComponent*)comp;
}

static UiComponent* create_ui_border()
{
    UiComponent *comp = (UiComponent*)malloc(sizeof(UiBorder));
    ui_base_init(comp, UiType_Border, sizeof(UiBorder), BORDER, border_n++);
    return comp;
}

static UiComponent* create_ui_textblock()
{
    UiTextblock *comp = (UiTextblock*)malloc(sizeof(UiTextblock));
    ui_base_init((UiComponent*)comp, UiType_Textblock, sizeof(UiTextblock), TEXTBLOCK, textblock_n++);
    comp->halign = HoriAlignType_Left;
    attr_value_init(&comp->text);
    return (UiComponent*)comp;
}

static UiComponent* create_ui_button()
{
    UiButton *comp = (UiButton*)malloc(sizeof(UiButton));
    ui_base_init((UiComponent*)comp, UiType_Button, sizeof(UiButton), BUTTON, button_n++);
    comp->content[0] = '\0';
    attr_value_init(&comp->command);
    return (UiComponent*)comp;
}

static UiComponent* create_ui_grid_coldef()
{
    UiComponent *comp = (UiComponent*)malloc(sizeof(UiGridColumndefinition));
    ui_base_init(comp, UiType_Grid_Columndefinition, sizeof(UiGridColumndefinition), "noname", 0);
    return comp;
}

static UiComponent* create_ui_coldef()
{
    UiColumndefinition *comp = (UiColumndefinition*)malloc(sizeof(UiColumndefinition));
    ui_base_init((UiComponent*)comp, UiType_Columndefinition, sizeof(UiColumndefinition), "coldef", coldef_n++);
    comp->width = 0;
    return (UiComponent*)comp;
}

static UiComponent* create_ui_comp_by_name(char *type_name)
{
    switch (_get_type_by_name(type_name))
    {
        case UiType_Window:
            return create_ui_window();
        case UiType_Grid:
            return create_ui_grid();
        case UiType_Grid_Rowdefinition:
            return create_ui_grid_rowdef();
        case UiType_Rowdefinition:
            return create_ui_rowdef();
        case UiType_Border:
            return create_ui_border();
        case UiType_Textblock:
            return create_ui_textblock();
        case UiType_Button:
            return create_ui_button();
        case UiType_Grid_Columndefinition:
            return create_ui_grid_coldef();
        case UiType_Columndefinition:
            return create_ui_coldef();
        default:
            printf("No component for type: %s\n", type_name);
    }
    return NULL;
}

static void ui_window_set_attr(UiWindow *window, char *key, char *val)
{
    if (strncmp(key, NAME, sizeof(NAME)) == 0)
    {
        memcpy(window->base.name, val, strlen(val));
        window->base.name[strlen(val)] = '\0';
    }
    else
        printf("Unrecognised window attribute: %s\n", key);
}

static void ui_rowdef_set_attr(UiRowdefinition *rowdef, char *key, char *val)
{
    printf("Unrecognised rowdef attribute: %s\n", key);
}

static bool _is_binding_attr(char *expr)
{
    return (strlen(expr) >= sizeof(BINDING_PREFIX))
        && (strncmp(expr, BINDING_PREFIX, sizeof(BINDING_PREFIX)-1) == 0);
}

static void _set_binding_attr(AttrValue *attr, char *expr)
{
    attr->type = AttrValueType_Binding;
    char *start = expr + sizeof(BINDING_PREFIX)-1;
    int len = strlen(start)-1;
    memcpy(attr->binding.name, start, len);
}

static void ui_textblock_set_attr(UiTextblock *textblock, char *key, char *val)
{
    if (strncmp(key, HORIZONTALALIGNMENT, sizeof(HORIZONTALALIGNMENT)) == 0)
    {
        if (strncmp(val, RIGHT, sizeof(RIGHT)) == 0)
            textblock->halign = HoriAlignType_Right;
    }
    else if (strncmp(key, TEXT, sizeof(TEXT)) == 0)
    {
        if (_is_binding_attr(val))
            _set_binding_attr(&textblock->text, val);
        else
            printf("textblock set const text attribute not implemented.\n");
    }
    else
        printf("Unrecognised textblock attribute: %s\n", key);
}

static void ui_button_set_attr(UiButton *button, char *key, char *val)
{
    if (strncmp(key, CONTENT, sizeof(CONTENT)) == 0)
    {
        memcpy(button->content, val, strlen(val));
    }
    else if (strncmp(key, COMMAND, sizeof(COMMAND)) == 0)
    {
        if (_is_binding_attr(val))
            _set_binding_attr(&button->command, val);
        else
            printf("button set const command attribute not implemented.\n");
    }
    else
        printf("Unrecognised button attribute: %s\n", key);
}

static void ui_coldef_set_attr(UiColumndefinition *coldef, char *key, char *val)
{
    printf("Unrecognised coldef attribute: %s\n", key);
}

static void _get_token_name(FILE *fp, char *buf, size_t start, size_t length)
{
    int cur = ftell(fp);
    fseek(fp, start, SEEK_SET);
    fgets(buf, length+1, fp);
    fseek(fp, cur, SEEK_SET);
}

static bool _add_ui_prop(UiComponent *comp, char *key, char *val)
{
    if (strncmp(key, GRID_ROW, sizeof(GRID_ROW)) == 0)
    {
        UiProp prop;
        prop.type = UiPropType_GridRow;
        prop.ival = atoi(val);
        list_add(&comp->props, (char*)&prop, sizeof(UiProp));
    }
    else if (strncmp(key, GRID_COLUMN, sizeof(GRID_COLUMN)) == 0)
    {
        UiProp prop;
        prop.type = UiPropType_GridCol;
        prop.ival = atoi(val);
        list_add(&comp->props, (char*)&prop, sizeof(UiProp));
    }
    else if (strncmp(key, GRID_COLSPAN, sizeof(GRID_COLSPAN)) == 0)
    {
        UiProp prop;
        prop.type = UiPropType_GridColSpan;
        prop.ival = atoi(val);
        list_add(&comp->props, (char*)&prop, sizeof(UiProp));
    }
    else if (strncmp(key, GRID_ROWSPAN, sizeof(GRID_ROWSPAN)) == 0)
    {
        UiProp prop;
        prop.type = UiPropType_GridRowSpan;
        prop.ival = atoi(val);
        list_add(&comp->props, (char*)&prop, sizeof(UiProp));
    }
    else
        return false;
    return true;
}

static bool _set_ui_base_attr(UiComponent *comp, char *key, char *val)
{
    if (strncmp(key, HEIGHT, sizeof(HEIGHT)) == 0)
    {
        if (strncmp(val, AUTO, sizeof(AUTO)) == 0)
            comp->height = VAL_AUTO;
        else if (val != NULL)
            comp->height = atoi(val);
    }
    else if (strncmp(key, WIDTH, sizeof(WIDTH)) == 0)
    {
        if (strncmp(val, AUTO, sizeof(AUTO)) == 0)
            comp->width = VAL_AUTO;
        else if (val != NULL)
            comp->width = atoi(val);
    }
    else
        return false;
    return true;
}

static void _set_ui_attr(UiComponent *comp, char *key, char *val)
{
    if (comp != NULL && key != NULL)
    {
        if (_add_ui_prop(comp, key, val))
            return;
        if (_set_ui_base_attr(comp, key, val))
            return;
        switch (comp->type)
        {
            case UiType_Window:
                ui_window_set_attr((UiWindow*)comp, key, val);
                break;
            case UiType_Rowdefinition:
                ui_rowdef_set_attr((UiRowdefinition*)comp, key, val);
                break;
            case UiType_Textblock:
                ui_textblock_set_attr((UiTextblock*)comp, key, val);
                break;
            case UiType_Button:
                ui_button_set_attr((UiButton*)comp, key, val);
                break;
            case UiType_Columndefinition:
                ui_coldef_set_attr((UiColumndefinition*)comp, key, val);
                break;
            default:
                printf("No component to set attribute: %s\n", key);
                break;
        }
    }
}

static void ui_component_free(UiComponent *comp)
{
    free(comp);
}

static void on_start_found(XmlToken *t)
{
    char buf[256];
    _get_token_name(fp, buf, t->start, t->length);
    UiComponent *comp = create_ui_comp_by_name(buf);
    if (ui_tree == NULL)
    {
        ui_tree = tree_create((char*)comp, comp->size);
        ui_current = ui_tree;
    }
    else
    {
        ui_current = tree_add_child(ui_current, (char*)comp, comp->size);
    }
    ui_component_free(comp);
}

static void on_close_found(XmlToken *t)
{
    char buf[256];
    _get_token_name(fp, buf, t->start, t->length);
    UiComponent *comp = (UiComponent*)tree_get_data(ui_current);
    if (comp->type == _get_type_by_name(buf))
        ui_current = tree_get_parent(ui_current);
    else
        printf("Found unmatched closed tag: %s, should be %d\n", buf, comp->type);
}

static void on_attr_found(XmlToken *k, XmlToken *v)
{
    char key[256];
    char val[256];
    _get_token_name(fp, key, k->start, k->length);
    UiComponent *comp = (UiComponent*)tree_get_data(ui_current);
    if (v != NULL)
    {
        _get_token_name(fp, val, v->start+1, v->length-2);
        _set_ui_attr(comp, key, val);
    }
    else
        _set_ui_attr(comp, key, NULL);
    
}

static void on_text_found(XmlToken *t)
{
    char buf[256];
    int cur = ftell(fp);
    fseek(fp, t->start, SEEK_SET);
    fgets(buf, t->length+1, fp);
    fseek(fp, cur, SEEK_SET);
}

static void _print_component_by_type(UiComponent *comp)
{
    switch (comp->type)
    {
        case UiType_Window:
            printf("%s\n", WINDOW);
            break;
        case UiType_Grid:
            printf("%s:name=%s\n", GRID, comp->name);
            break;
        case UiType_Grid_Rowdefinition:
        {
            printf("%s\n", GRID_ROWDEFINITION);
            break;
        }
        case UiType_Rowdefinition:
        {
            UiRowdefinition *row = (UiRowdefinition*)comp;
            printf("%s height=%d\n", ROWDEFINITION, row->base.height);
            break;
        }
        case UiType_Border:
            printf("%s\n", BORDER);
            break;
        case UiType_Textblock:
            printf("%s\n", TEXTBLOCK);
            break;
        case UiType_Button:
            printf("%s\n", BUTTON);
            break;
        case UiType_Grid_Columndefinition:
            printf("%s\n", GRID_COLUMNDEFINITION);
            break;
        case UiType_Columndefinition:
            printf("%s\n", COLUMNDEFINITION);
            break;
        default:
            printf("No component for type: %d\n", comp->type);
    }
}

static void _print_tree(Tree *tree, int leading)
{
    if (tree != NULL)
    {
        for (int i = 0; i < leading; i++)
            printf(" ");
        UiComponent *comp = (UiComponent*)tree_get_data(tree);
        _print_component_by_type(comp);
        for (int i = 0; i < tree_children_count(tree); i++)
            _print_tree(tree_get_child(tree, i), leading+2);
    }
}

static void _write_rowdef_to_file(FILE *f, Tree *tree)
{
    UiRowdefinition *rowdef = (UiRowdefinition*)tree_get_data(tree);
    UiGrid *grid = (UiGrid*)tree_get_data(tree_get_parent(tree_get_parent(tree)));
    char *line1 = "\nRowDefinition %s;\n";
    char *line2_a = "%s.height = %s;\n";
    char *line2_b = "%s.height = %d;\n";
    char *line3 = "grid_add_rowdef(%s, &%s);\n";
    fprintf(f, line1, rowdef->base.name);
    if (rowdef->base.height == VAL_AUTO)
        fprintf(f, line2_a, rowdef->base.name, "SIZE_AUTO");
    else
        fprintf(f, line2_b, rowdef->base.name, rowdef->base.height);
    fprintf(f, line3, grid->base.name, rowdef->base.name);
}

static void _write_coldef_to_file(FILE *f, Tree *tree)
{
    UiColumndefinition *coldef = (UiColumndefinition*)tree_get_data(tree);
    UiGrid *grid = (UiGrid*)tree_get_data(tree_get_parent(tree_get_parent(tree)));
    char *line1 = "\nColumnDefinition %s;\n";
    char *line2_a = "%s.width = %s;\n";
    char *line2_b = "%s.width = %d;\n";
    char *line3 = "grid_add_coldef(%s, &%s);\n";
    fprintf(f, line1, coldef->base.name);
    if (coldef->base.width == VAL_AUTO)
        fprintf(f, line2_a, coldef->base.name, "SIZE_AUTO");
    else
        fprintf(f, line2_b, coldef->base.name, coldef->base.width);
    fprintf(f, line3, grid->base.name, coldef->base.name);
}

static void _write_add_child_to_file(FILE *f, Tree *parent, Tree *child)
{
    if (parent != NULL)
    {
        UiComponent *parent_comp = (UiComponent*)tree_get_data(parent);
        if (parent_comp->type != UiType_Window)
        {
            UiComponent *comp = (UiComponent*)tree_get_data(child);
            char *line = "fe_add_child(%s, %s);\n";
            fprintf(f, line, parent_comp->name, comp->name);
        }
    }
}

static UiComponent* _get_parent(Tree *tree, UiType parent_type)
{
    UiComponent *comp = (UiComponent*)tree_get_data(tree);
    while (comp != NULL)
    {
        if (comp->type == parent_type)
            return comp;
        tree = tree_get_parent(tree);
        comp = (UiComponent*)tree_get_data(tree);
    }
    return NULL;
}

static void _write_props_to_file(FILE *f, Tree *tree)
{
    UiComponent *comp = (UiComponent*)tree_get_data(tree);
    for (int i = 0; i < list_count(&comp->props); i++)
    {
        UiProp *prop = (UiProp*)list_get(&comp->props, i);
        switch (prop->type)
        {
            case UiPropType_GridRow:
            {
                UiComponent *grid = (UiComponent*)_get_parent(tree, UiType_Grid);
                char *line = "grid_set_row(%s, %s, %d);\n";
                fprintf(f, line, grid->name, comp->name, prop->ival);
                break;
            }
            case UiPropType_GridCol:
            {
                UiComponent *grid = (UiComponent*)_get_parent(tree, UiType_Grid);
                char *line = "grid_set_col(%s, %s, %d);\n";
                fprintf(f, line, grid->name, comp->name, prop->ival);
                break;
            }
            case UiPropType_GridRowSpan:
            {
                UiComponent *grid = (UiComponent*)_get_parent(tree, UiType_Grid);
                char *line = "grid_set_rowspan(%s, %s, %d);\n";
                fprintf(f, line, grid->name, comp->name, prop->ival);
                break;
            }
            case UiPropType_GridColSpan:
            {
                UiComponent *grid = (UiComponent*)_get_parent(tree, UiType_Grid);
                char *line = "grid_set_colspan(%s, %s, %d);\n";
                fprintf(f, line, grid->name, comp->name, prop->ival);
                break;
            }
            default:
                printf("No function to write property: %d\n", prop->type);
        }
    }
}

static void _write_create_comp_to_file(FILE *f, Tree *tree, char *type, char *content)
{
    UiComponent *comp = (UiComponent*)tree_get_data(tree);
    char *line = "\nFrameworkElement *%s = %s_create_ex(0, 0";
    char *line_s = ", %s";
    char *line_d = ", %d";
    char *line_c = ", L\"%s\"";
    char *line_e = ");\n";
    fprintf(f, line, comp->name, type);
    if (comp->width == VAL_AUTO)
        fprintf(f, line_s, "SIZE_AUTO");
    else
        fprintf(f, line_d, comp->width);
    if (comp->height == VAL_AUTO)
        fprintf(f, line_s, "SIZE_AUTO");
    else
        fprintf(f, line_d, comp->height);
    if (content != NULL)
        fprintf(f, line_c, content);
    fprintf(f, line_e);
    _write_add_child_to_file(f, tree_get_parent(tree), tree);
    _write_props_to_file(f, tree);
}

static void _write_grid_to_file(FILE *f, Tree *tree)
{
    _write_create_comp_to_file(f, tree, "grid", NULL);
}

static void _write_border_to_file(FILE *f, Tree *tree)
{
    _write_create_comp_to_file(f, tree, "border", NULL);
}

static void _write_textblock_to_file(FILE *f, Tree *tree)
{
    UiTextblock *tb = (UiTextblock*)tree_get_data(tree);
    _write_create_comp_to_file(f, tree, "textblock", NULL);
    if (tb->text.type == AttrValueType_Binding)
    {
        char *line = "textblock_bind_text(%s, &(pDc->%s));\n";
        fprintf(f, line, tb->base.name, tb->text.binding.name);
    }
}

static void _write_button_to_file(FILE *f, Tree *tree)
{
    UiButton *btn = (UiButton*)tree_get_data(tree);
    _write_create_comp_to_file(f, tree, "button", btn->content);
    if (btn->command.type == AttrValueType_Binding)
    {
        char *line1 = "button_bind_command(%s, &(pDc->%s));\n";
        char *line2 = "button_set_cmdparameter(%s, (char*)pDc);\n";
        fprintf(f, line1, btn->base.name, btn->command.binding.name);
        fprintf(f, line2, btn->base.name);
    }
}

static void __write_to_file(FILE *f, Tree *tree)
{
    UiComponent *comp = (UiComponent*)tree_get_data(tree);
    switch (comp->type)
    {
        case UiType_Grid:
            _write_grid_to_file(f, tree);
            break;
        case UiType_Rowdefinition:
            _write_rowdef_to_file(f, tree);
            break;
        case UiType_Columndefinition:
            _write_coldef_to_file(f, tree);
            break;
        case UiType_Border:
            _write_border_to_file(f, tree);
            break;
        case UiType_Textblock:
            _write_textblock_to_file(f, tree);
            break;
        case UiType_Button:
            _write_button_to_file(f, tree);
            break;
        case UiType_Window:
        case UiType_Grid_Rowdefinition:
        case UiType_Grid_Columndefinition:
            break;
        default:
            printf("No function to write type: %d\n", comp->type);
    }
    for (int i = 0; i < tree_children_count(tree); i++)
    {
        __write_to_file(f, tree_get_child(tree, i));
    }
}

static void _write_impl_file(Tree *tree, char *folder_path)
{
    char file_path[256] = {'\0'};
    memcpy(file_path, folder_path, strlen(folder_path));
    UiWindow *window = (UiWindow*)tree_get_data(tree);
    char *name = window->base.name;
    strcat(file_path, "\\");
    strcat(file_path, name);
    strcat(file_path, "_ui.g.c");
    FILE *f = fopen(file_path, "w");
    Tree *child = tree_get_child(tree, 0);
    UiComponent *comp = (UiComponent*)tree_get_data(child);
    char *start_line = "#include \"demo_ui.h\";\nFrameworkElement* get_%s_ui(AppDataContext *pDc)\n{\n";
    char *end_line = "\nreturn %s;\n}\n";
    fprintf(f, start_line, window->base.name);
    __write_to_file(f, child);
    fprintf(f, end_line, comp->name);
    fclose(f);
}

static void _write_header_file(Tree *tree, char *folder_path)
{
    char file_path[256] = {'\0'};
    memcpy(file_path, folder_path, strlen(folder_path));
    UiWindow *window = (UiWindow*)tree_get_data(tree);
    char *name = window->base.name;
    strcat(file_path, "\\");
    strcat(file_path, name);
    strcat(file_path, "_ui.g.h");
    FILE *f = fopen(file_path, "w");
    fprintf(f, "#ifndef %sUI_H\n", name);
    fprintf(f, "#define %sUI_H\n", name);
    fprintf(f, "#include <stdio.h>\n");
    fprintf(f, "#include <stdlib.h>\n");
    fprintf(f, "#include <wchar.h>\n");
    fprintf(f, "#include \"ngf/zaml_structs.h\"\n");
    fprintf(f, "#include <wchar.h>\n");
    fprintf(f, "#include \"%s_datacontext.h\"\n", name);
    fprintf(f, "FrameworkElement* get_%s_ui(AppDataContext *pDc);\n", name);
    fprintf(f, "#endif\n");
    fclose(f);
}

int main(int argc, char *argv[])
{
    //char *test_file_path = "..\\test files\\test.layout";
    //char *output_file_path = "..\\test files\\test.layout.g.c";
    char *input_path = argv[1];
    char *output_path = argv[2];
    fp = fopen(input_path, "rb");
    XmlToken token = { 0 };
    XmlParser parser = xmlparser_create();
    xmlparser_set_listners(parser, on_start_found, on_attr_found, on_text_found, on_close_found);
    int ch = 0;
    int i = 0;
    int j = 0;
    while (EOF != (ch = fgetc(fp)))
    {
        j = ftell(fp);
        if (!xmlparser_feed(parser, ch, &i, &token))
            break;
        if (i < j)
        {
            fseek(fp, i, SEEK_SET);
        }
    }
    xmlparser_feed(parser, ' ', &i, &token);
    fclose(fp);
    xmlparser_free(parser);
    //_print_tree(ui_tree, 0);
    _write_header_file(ui_tree, output_path);
    _write_impl_file(ui_tree, output_path);
    return 0;
}