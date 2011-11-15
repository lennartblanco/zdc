#include <strings.h>

#include "ir_variable.h"
#include "dt_basic.h"
#include "utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_variable_class_init(gpointer klass, gpointer foo);

static DtDataType *
ir_variable_do_get_data_type(IrExpression *self);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_variable_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info =
      {
        sizeof (IrVariableClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_variable_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrVariable),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_SYMBOL,
                                    "IrVariableType",
                                    &info, 0);
    }
    return type;
}

IrVariable *
ir_variable_new(bool is_ref,
                DtDataType *type,
                char *name,
                IrExpression *initializer,
                guint line_number)
{
    assert(type);

    IrVariable *obj;

    obj = g_object_new(IR_TYPE_VARIABLE,
                       "ir-node-line-number", line_number,
                       "ir-symbol-name", name,
                       NULL);

    obj->is_ref = is_ref;
    obj->type = type;
    obj->initializer = initializer;
    return obj;
}

void
ir_variable_set_location(IrVariable *self, ImlVariable *location)
{
    assert(IR_IS_VARIABLE(self));
    assert(location);

    self->location = location;
}

ImlVariable *
ir_variable_get_location(IrVariable *self)
{
    assert(IR_IS_VARIABLE(self));

    return self->location;
}

DtDataType *
ir_variable_get_data_type(IrVariable *self)
{
    assert(IR_IS_VARIABLE(self));

    return ir_variable_do_get_data_type(IR_EXPRESSION(self));
}

void
ir_variable_set_data_type(IrVariable *self, DtDataType *type)
{
    assert(IR_IS_VARIABLE(self));
    assert(DT_IS_DATA_TYPE(type));

    self->type = type;
}

char *
ir_variable_get_name(IrVariable *self)
{
    assert(IR_IS_VARIABLE(self));

    return ir_symbol_get_name(IR_SYMBOL(self));
}

IrExpression *
ir_variable_get_initializer(IrVariable *self)
{
    assert(IR_IS_VARIABLE(self));

    return self->initializer;
}

void
ir_variable_set_initializer(IrVariable *self, IrExpression *initializer)
{
    assert(IR_IS_VARIABLE(self));
    assert(IR_IS_EXPRESSION(initializer));

    self->initializer = initializer;
}

bool
ir_variable_is_ref(IrVariable *self)
{
    assert(IR_IS_VARIABLE(self));

    return self->is_ref;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ir_variable_do_print(IrNode *self, FILE *out, int indention)
{
    assert(IR_IS_VARIABLE(self));

    IrVariable *var = IR_VARIABLE(self);
    fprintf_indent(out, indention, "%s %s",
                   dt_data_type_get_string(var->type),
                   ir_symbol_get_name(IR_SYMBOL(var)));

    if (var->initializer != NULL)
    {
        fprintf(out, " = ");
        ir_node_print(IR_NODE(var->initializer), out, indention + 2);
    }
}

static DtDataType *
ir_variable_do_get_data_type(IrExpression *self)
{
    assert(IR_IS_VARIABLE(self));

    return IR_VARIABLE(self)->type;
}

static UtRange *
ir_variable_get_value_range(IrExpression *self)
{
    assert(IR_IS_VARIABLE(self));

    DtDataType *type = ir_expression_get_data_type(self);
    assert(dt_is_basic(type));

    return dt_basic_get_value_range(DT_BASIC(type));
}

static void
ir_variable_class_init(gpointer klass, gpointer foo)
{
    IR_NODE_CLASS(klass)->do_print = ir_variable_do_print;
    IR_EXPRESSION_CLASS(klass)->do_get_data_type =
        ir_variable_do_get_data_type;
    IR_EXPRESSION_CLASS(klass)->get_value_range =
        ir_variable_get_value_range;
}
