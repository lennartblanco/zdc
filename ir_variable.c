#include <strings.h>

#include "ir_variable.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_variable_class_init(gpointer klass, gpointer foo);

static void
ir_variable_do_print(IrSymbol *self, FILE *out, int indention);

static AstDataType *
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
ir_variable_new(AstDataType *type, 
                char *name,
                IrExpression *initializer)
{
    assert(type);
    assert(name);

    IrVariable *obj;

    obj = g_object_new(IR_TYPE_VARIABLE,
                       "ir-symbol-name", name,
                       NULL);
    obj->type = type;
    obj->initializer = initializer;
    return obj;
}

void
ir_variable_set_location(IrVariable *self, GObject *location)
{
    assert(self);
    assert(IR_IS_VARIABLE(self));
    assert(location);

    self->location = location;
}

GObject *
ir_variable_get_location(IrVariable *self)
{
    assert(self);
    assert(IR_IS_VARIABLE(self));

    return self->location;
}

AstDataType *
ir_variable_get_data_type(IrVariable *self)
{
    assert(self);
    assert(IR_IS_VARIABLE(self));

    return ir_variable_do_get_data_type(IR_EXPRESSION(self));
}

char *
ir_variable_get_name(IrVariable *self)
{
    assert(self);
    assert(IR_IS_VARIABLE(self));

    return ir_symbol_get_name(IR_SYMBOL(self));
}

IrExpression *
ir_variable_get_initializer(IrVariable *self)
{
    assert(self);
    assert(IR_IS_VARIABLE(self));

    return self->initializer;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ir_variable_class_init(gpointer klass, gpointer foo)
{
    ((IrSymbolClass *)klass)->do_print = ir_variable_do_print;
    ((IrExpressionClass *)klass)->do_get_data_type =
        ir_variable_do_get_data_type;
}

static void
ir_variable_do_print(IrSymbol *self, FILE *out, int indention)
{
    assert(self);
    assert(IR_IS_VARIABLE(self));

    IrVariable *var = IR_VARIABLE(self);
    ast_node_print(XDP_AST_NODE(var->type), out);
    fprintf(out, " %s", ir_symbol_get_name(self));

    if (var->initializer != NULL)
    {
        fprintf(out, " = ");
        ast_node_print(XDP_AST_NODE(var->initializer), out);
    }
}

static AstDataType *
ir_variable_do_get_data_type(IrExpression *self)
{
    assert(self);
    assert(IR_IS_VARIABLE(self));

    return IR_VARIABLE(self)->type;
}
