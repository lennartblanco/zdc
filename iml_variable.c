#include <glib.h>
#include <stdbool.h>

#include "iml_variable.h"
#include "utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static iml_data_type_t
iml_variable_do_get_data_type(ImlOperand *self);

static void
iml_variable_do_print(ImlOperand *self, FILE *out, guint indention);

static void
iml_variable_class_init(gpointer klass, gpointer foo);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
iml_variable_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (ImlVariableClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        iml_variable_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (ImlVariable),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IML_TYPE_OPERAND,
                                    "ImlVariableType",
                                    &info, 0);
    }
    return type;
}

bool
iml_is_variable(void *obj)
{
    return G_TYPE_CHECK_INSTANCE_TYPE ((obj), IML_TYPE_VARIABLE);
}

ImlVariable *
iml_variable_new(iml_data_type_t data_type, const gchar *name)
{
    ImlVariable *obj;
    static guint temp_var_names_cntr = 0;

    obj = g_object_new(IML_TYPE_VARIABLE, NULL);

    obj->datatype = data_type;
    obj->reg = NULL;
    if (name == NULL)
    {
        obj->name = g_strdup_printf("[temp%u]", temp_var_names_cntr++);
        obj->is_temp = true;
    }
    else
    {
        obj->name = g_strdup(name);
        obj->is_temp = false;
    }

    return obj;
}

ImlVariable *
iml_variable_blob_new(guint size, const gchar *name)
{
    ImlVariable *obj;

    obj = iml_variable_new(iml_blob, name);
    obj->size = size;

    return obj;
}

iml_data_type_t
iml_variable_get_data_type(ImlVariable *self)
{
    assert(iml_is_variable(self));

    return iml_operand_get_data_type(IML_OPERAND(self));
}

guint
iml_variable_get_size(ImlVariable *self)
{
    assert(iml_is_variable(self));

    switch (self->datatype)
    {
        case iml_blob:
            return self->size;
        case iml_ptr:
        case iml_32b:
        case iml_16b:
        case iml_8b:
            /* all variables are store as 32-bit values for now */
            return 4;
    }

    /* unexpected variable data type */
    assert(false);
}

bool
iml_variable_is_temp(ImlVariable *self)
{
    assert(iml_is_variable(self));

    return self->is_temp;
}

void
iml_variable_set_register(ImlVariable *self, iml_register_t *reg)
{
    assert(iml_is_variable(self));
    assert(reg);

    self->reg = reg;
}

iml_register_t *
iml_variable_get_register(ImlVariable *self)
{
    assert(iml_is_variable(self));

    return self->reg;
}

void
iml_variable_set_frame_offset(ImlVariable *self, gint frame_offset)
{
    assert(iml_is_variable(self));
    assert(self->reg == NULL);

    self->frame_offset = frame_offset;
}

gint
iml_variable_get_frame_offset(ImlVariable *self)
{
    assert(iml_is_variable(self));

    return self->frame_offset;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static iml_data_type_t
iml_variable_do_get_data_type(ImlOperand *self)
{
    assert(iml_is_variable(self));

    return IML_VARIABLE(self)->datatype;
}

static void
iml_variable_do_print(ImlOperand *self, FILE *out, guint indention)
{
    assert(iml_is_variable(self));
    assert(out);

    ImlVariable *var = IML_VARIABLE(self);
    gchar *blob_size_str = NULL;

    char *type_str;

    switch (var->datatype)
    {
        case iml_8b:
            type_str = " 8b";
            break;
        case iml_16b:
            type_str = "16b";
            break;
        case iml_32b:
            type_str = "32b";
            break;
        case iml_ptr:
            type_str = "ptr";
            break;
        case iml_blob:
            type_str = "blob";
            break;
        default:
            /* unexpected data type */
            assert(FALSE);
    }

    if (var->datatype == iml_blob)
    {
        blob_size_str = g_strdup_printf("(%u)", var->size);
    }

    fprintf_indent(out, indention, "%s%s %s",
                   type_str,
                   blob_size_str != NULL ? blob_size_str : "",
                   var->name);

    if (var->reg != NULL) {
        fprintf(out, " [");
        iml_register_print(var->reg, out, 0);
        fprintf(out, "]");
    }
    else
    {
       fprintf(out, " [%04d]", var->frame_offset);
    }

    g_free(blob_size_str);
}

static void
iml_variable_do_print_short(ImlOperand *self, FILE *out, guint indention)
{
    assert(iml_is_variable(self));
    assert(out);

    fprintf(out, "%s", IML_VARIABLE(self)->name);
}

static void
iml_variable_class_init(gpointer klass, gpointer foo)
{
    assert(IML_IS_OPERAND_CLASS(klass));

    /* install virtual method implementations */
    IML_OPERAND_CLASS(klass)->do_get_data_type = iml_variable_do_get_data_type;
    IML_OPERAND_CLASS(klass)->do_print = iml_variable_do_print;
    IML_OPERAND_CLASS(klass)->do_print_short = iml_variable_do_print_short;
}
