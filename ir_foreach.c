#include "ir_foreach.h"
#include "utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_foreach_do_print(IrNode *self, FILE *out, int indention);

static void
ir_foreach_class_init(gpointer klass, gpointer foo);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_foreach_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (IrForeachClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_foreach_class_init, /* class_init */
        NULL, /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrForeach),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_STATMENT,
                                    "IrForeachType",
                                    &info, 0);
    }
    return type;
}

IrForeach *
ir_foreach_new(IrVariable *index,
               IrVariable *value,
               IrArraySlice *aggregate,
               IrCodeBlock *body)
{
    IrForeach *obj;

    obj = g_object_new(IR_TYPE_FOREACH, NULL);

    obj->index = index;
    obj->value = value;
    obj->aggregate = aggregate;
    obj->body = body;

    return obj;
}

void
ir_foreach_set_index(IrForeach *self, IrVariable *index)
{
    assert(IR_IS_FOREACH(self));
    assert(IR_IS_VARIABLE(index));

    self->index = index;
}

IrVariable *
ir_foreach_get_index(IrForeach *self)
{
    assert(IR_IS_FOREACH(self));

    return self->index;
}

IrVariable *
ir_foreach_get_value(IrForeach *self)
{
    assert(IR_IS_FOREACH(self));

   return self->value;
}

IrArraySlice *
ir_foreach_get_aggregate(IrForeach *self)
{
    assert(IR_IS_FOREACH(self));

    return self->aggregate;
}

IrCodeBlock *
ir_foreach_get_body(IrForeach *self)
{
    assert(IR_IS_FOREACH(self));

    return self->body;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ir_foreach_do_print(IrNode *self, FILE *out, int indention)
{
    assert(IR_IS_FOREACH(self));
    assert(out);

    IrForeach *foreach = IR_FOREACH(self);

    fprintf_indent(out, indention, "foreach [%p]\n  index: ", foreach);
    if (foreach->index == NULL)
    {
        fprintf(out, "<none>");
    }
    else
    {
        ir_node_print(IR_NODE(foreach->index), out, indention);
    }
    fprintf_indent(out, indention, "\n  value: ");
    ir_node_print(IR_NODE(foreach->value), out, indention);
    fprintf_indent(out, indention, "\n  aggregate: ");
    ir_node_print(IR_NODE(foreach->aggregate), out, indention);
    fprintf_indent(out, indention, "\n");
    ir_node_print(IR_NODE(foreach->body), out, indention + 2);
}

static void
ir_foreach_class_init(gpointer klass, gpointer foo)
{
    ((IrNodeClass *)klass)->do_print = ir_foreach_do_print;
}
