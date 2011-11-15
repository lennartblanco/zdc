#include "ir_var_ref.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_var_ref_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_var_ref_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (IrVarRefClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_var_ref_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrVarRef),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_EXPRESSION,
                                    "IrVarRefType",
                                    &info, 0);
    }
    return type;
}

IrVarRef *
ir_var_ref_new(IrVariable *var, guint line_number)
{
    assert(IR_IS_VARIABLE(var));
    assert(ir_variable_is_ref(var));

    IrVarRef *obj;

    obj = g_object_new(IR_TYPE_VAR_REF,
                       "ir-node-line-number", line_number,
                       NULL);
    obj->var = var;

    return obj;
}

IrVariable *
ir_var_ref_get_var(IrVarRef *self)
{
    assert(IR_IS_VAR_REF(self));

    return self->var;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static DtDataType *
ir_var_ref_do_get_data_type(IrExpression *self)
{
    assert(IR_IS_VAR_REF(self));

    return ir_variable_get_data_type(IR_VAR_REF(self)->var);
}

static bool
ir_var_ref_do_is_lvalue(IrExpression *self)
{
    assert(IR_IS_VAR_REF(self));

    return true;
}

static UtRange *
ir_var_ref_get_value_range(IrExpression *self)
{
    assert(IR_IS_VAR_REF(self));

    return
        ir_expression_get_value_range(IR_EXPRESSION(IR_VAR_REF(self)->var));
}

static void
ir_var_ref_class_init(gpointer klass, gpointer dummy)
{
    IR_EXPRESSION_CLASS(klass)->do_get_data_type =
        ir_var_ref_do_get_data_type;
    IR_EXPRESSION_CLASS(klass)->do_is_lvalue =
        ir_var_ref_do_is_lvalue;
    IR_EXPRESSION_CLASS(klass)->get_value_range =
        ir_var_ref_get_value_range;
}
