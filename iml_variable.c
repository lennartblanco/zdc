#include <glib.h>

#include "iml_variable.h"
#include "utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

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


ImlVariable *
iml_variable_new(iml_data_type_t data_type)
{
    ImlVariable *obj;

    obj = g_object_new(IML_TYPE_VARIABLE, NULL);

    obj->datatype = data_type;

    return obj;
}

iml_data_type_t
iml_variable_get_data_type(ImlVariable *self)
{
    assert(IML_IS_VARIABLE(self));

    return self->datatype;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
iml_variable_do_print(ImlOperand *self, FILE *out, guint indention)
{
    assert(IML_IS_VARIABLE(self));
    assert(out);

    ImlVariable *var = IML_VARIABLE(self);

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
        default:
            /* unexpected data type */
            assert(FALSE);
    }

    fprintf_indent(out, indention, "%s [%p]", type_str, self);
}

static void
iml_variable_class_init(gpointer klass, gpointer foo)
{
    assert(IML_IS_OPERAND_CLASS(klass));

    /* install print method implementation */
    IML_OPERAND_CLASS(klass)->do_print = iml_variable_do_print;
}
