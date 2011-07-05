#include "ir_while.h"
#include "utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_while_do_print(IrNode *self, FILE *out, int indention);

static void
ir_while_class_init(gpointer klass, gpointer foo);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_while_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info =
      {
        sizeof (IrWhileClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_while_class_init, /* class_init */
        NULL, /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrWhile),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_STATMENT,
                                    "IrWhileType",
                                    &info, 0);
    }
    return type;
}

IrWhile *
ir_while_new(IrExpression *loop_condition, IrCodeBlock *body)
{
    IrWhile *obj;

    obj = g_object_new(IR_TYPE_WHILE, NULL);

    obj->loop_condition = loop_condition;
    obj->body = body;

    return obj;
}

void
ir_while_set_loop_condition(IrWhile *self, IrExpression *loop_condition)
{
    assert(self);
    assert(IR_IS_WHILE(self));
    assert(loop_condition);
    assert(IR_IS_EXPRESSION(loop_condition));

    self->loop_condition = loop_condition;
}

IrExpression *
ir_while_get_loop_condition(IrWhile *self)
{
    assert(self);
    assert(IR_IS_WHILE(self));

    return self->loop_condition;
}

IrCodeBlock *
ir_while_get_body(IrWhile *self)
{
    assert(self);
    assert(IR_IS_WHILE(self));

    return self->body;
}


/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ir_while_do_print(IrNode *self, FILE *out, int indention)
{
    assert(self);
    assert(IR_IS_WHILE(self));
    assert(out);

    IrWhile *w = IR_WHILE(self);

    fprintf_indent(out, indention, "while [%p]\nloop_condition: ", w);
    ir_node_print(IR_NODE(w->loop_condition), out, indention);
    fprintf_indent(out, indention, "\nbody:\n");
    ir_node_print(IR_NODE(w->body), out, indention + 2);
    
}

static void
ir_while_class_init(gpointer klass, gpointer foo)
{
    ((IrNodeClass *)klass)->do_print = ir_while_do_print;
}
