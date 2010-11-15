#include <stdbool.h>
#include <string.h>

#include "ir_array_slice.h"
#include "ir_uint_constant.h"
#include "dt_array_type.h"
#include "dt_static_array_type.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_array_slice_class_init(gpointer klass, gpointer dummy);

static DtDataType *
ir_array_slice_do_get_data_type(IrExpression *self);

static void
ir_array_slice_do_print(IrNode *self, FILE *out, int indention);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ir_array_slice_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (IrArraySliceClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_array_slice_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrArraySlice),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_EXPRESSION,
                                    "IrArraySliceType",
                                    &info, 0);
    }
    return type;
}

IrArraySlice *
ir_array_slice_new(IrExpression *array,
                   IrExpression *start,
                   IrExpression *end,
                   guint line_number)
{
    assert(IR_IS_EXPRESSION(array));

    IrArraySlice *obj;

    obj = g_object_new(IR_TYPE_ARRAY_SLICE,
                       "ir-node-line-number", line_number,
                       NULL);

    obj->array = array;
    obj->start = start;
    obj->end = end;

    return obj;
}

IrExpression *
ir_array_slice_get_array(IrArraySlice *self)
{
    assert(IR_IS_ARRAY_SLICE(self));

    return self->array;
}

void
ir_array_slice_set_array(IrArraySlice *self, IrExpression *array)
{
    assert(IR_IS_ARRAY_SLICE(self));
    assert(IR_IS_EXPRESSION(array));

    self->array = array;
}

IrExpression *
ir_array_slice_get_start(IrArraySlice *self)
{
    assert(IR_IS_ARRAY_SLICE(self));

    return self->start;
}

IrExpression *
ir_array_slice_get_end(IrArraySlice *self)
{
    assert(IR_IS_ARRAY_SLICE(self));

    return self->end;
}

void
ir_array_slice_set_start(IrArraySlice *self,
                         IrExpression *start)
{
    assert(IR_IS_ARRAY_SLICE(self));
    assert(IR_IS_EXPRESSION(start));

    self->start = start;
}

void
ir_array_slice_set_end(IrArraySlice *self,
                       IrExpression *end)
{
    assert(IR_IS_ARRAY_SLICE(self));
    assert(IR_IS_EXPRESSION(end));

    self->end = end;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ir_array_slice_class_init(gpointer klass, gpointer dummy)
{
    ((IrExpressionClass *)klass)->do_get_data_type =
        ir_array_slice_do_get_data_type;
    ((IrNodeClass *)klass)->do_print = ir_array_slice_do_print;
}

static DtDataType *
ir_array_slice_do_get_data_type(IrExpression *self)
{
    assert(IR_IS_ARRAY_SLICE(self));

    return ir_expression_get_data_type(IR_ARRAY_SLICE(self)->array);
}

static void
ir_array_slice_do_print(IrNode *self, FILE *out, int indention)
{
/* needs to be ported */
assert(false);
//    assert(IR_IS_ARRAY_SLICE(self));
//
//    IrArraySlice *slice = IR_ARRAY_SLICE(self);
//    fprintf(out, "(");
//    if (slice->data_type != NULL)
//    {
//        fprintf(out, "%s", dt_data_type_get_string(slice->data_type));
//    }
//    else
//    {
//        fprintf(out, "?[?]");
//    }
//    fprintf(out, ") %s[", ir_lvalue_get_name(IR_LVALUE(slice)));
//
//    if (slice->start != NULL)
//    {
//        ir_node_print(IR_NODE(slice->start), out, 0);
//        fprintf(out, "..");
//        ir_node_print(IR_NODE(slice->end), out, 0);
//    }
//    fprintf(out, "]");
}
