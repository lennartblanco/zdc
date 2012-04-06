#include "ir_struct.h"
#include "ir_variable.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_struct_class_init(gpointer klass, gpointer foo);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_struct_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (IrStructClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_struct_class_init, /* class_init */
        NULL, /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrStruct),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_SYMBOL,
                                    "IrStructType",
                                    &info, 0);
    }
    return type;
}

IrStruct *
ir_struct_new(gchar *name,
              GSList *members,
              GSList *methods,
              bool opaque,
              IrModule *parent_module,
              sym_table_t *symbols)
{
    IrStruct *obj;

    obj = g_object_new(IR_TYPE_STRUCT,
                       "ir-symbol-name", name,
                       NULL);

    obj->data_type = dt_struct_new(name, opaque, parent_module);
    obj->members = members;
    obj->methods = methods;
    obj->symbols = symbols;

    return obj;
}

GSList *
ir_struct_get_members(IrStruct *self)
{
    assert(IR_IS_STRUCT(self));

    return self->members;
}

GSList *
ir_struct_get_methods(IrStruct *self)
{
    assert(IR_IS_STRUCT(self));

    return self->methods;
}

gchar *
ir_struct_get_name(IrStruct *self)
{
    assert(IR_IS_STRUCT(self));

    return ir_symbol_get_name(ir_symbol(self));
}

DtStruct *
ir_struct_get_data_type(IrStruct *self)
{
    assert(IR_IS_STRUCT(self));

    return DT_STRUCT(ir_expression_get_data_type(ir_expression(self)));
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static DtDataType *
ir_struct_do_get_data_type(IrExpression *self)
{
    assert(IR_IS_STRUCT(self));

    return DT_DATA_TYPE(IR_STRUCT(self)->data_type);
}

static void
ir_struct_class_init(gpointer klass, gpointer foo)
{
    IR_EXPRESSION_CLASS(klass)->do_get_data_type =
        ir_struct_do_get_data_type;
}
