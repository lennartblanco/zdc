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
      type = g_type_register_static(IR_TYPE_LVALUE,
                                    "IrArraySliceType",
                                    &info, 0);
    }
    return type;
}

IrArraySlice *
ir_array_slice_new(char *array_name,
                   IrExpression *start,
                   IrExpression *end,
                   guint line_number)
{
    assert(array_name);

    IrArraySlice *obj;

    obj = g_object_new(IR_TYPE_ARRAY_SLICE,
                       "ir-node-line-number", line_number,
                       "ir-lvalue-symbol-name", array_name,
                       NULL);

    obj->start = start;
    obj->end = end;
    obj->data_type = NULL;

    return obj;
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



char *
ir_array_slice_get_name(IrArraySlice *self)
{
    assert(IR_IS_ARRAY_SLICE(self));

    return ir_lvalue_get_name(IR_LVALUE(self));
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

    IrArraySlice *slice = IR_ARRAY_SLICE(self);

    if (slice->data_type == NULL)
    {
        IrVariable *array_variable;
        DtArrayType *array_data_type;
        DtDataType *array_element_type;


        array_variable = ir_lvalue_get_variable(IR_LVALUE(slice));
        array_data_type =
            DT_ARRAY_TYPE(ir_variable_get_data_type(array_variable));
        array_element_type =
            dt_array_type_get_data_type(array_data_type);

        if (IR_IS_UINT_CONSTANT(slice->start) &&
            IR_IS_UINT_CONSTANT(slice->end))
        {
            guint32 slice_len;

            slice_len =
                ir_uint_constant_get_value(IR_UINT_CONSTANT(slice->end)) -
                ir_uint_constant_get_value(IR_UINT_CONSTANT(slice->start));
            slice->data_type =
                DT_DATA_TYPE(dt_static_array_type_new(array_element_type,
                                                      slice_len));
        }
        else
        {
            slice->data_type =
                DT_DATA_TYPE(dt_array_type_new(array_element_type));
        }
    }
 
    return slice->data_type;
}

static void
ir_array_slice_do_print(IrNode *self, FILE *out, int indention)
{
    assert(IR_IS_ARRAY_SLICE(self));

    IrArraySlice *slice = IR_ARRAY_SLICE(self);
    fprintf(out, "(");
    if (slice->data_type != NULL)
    {
        fprintf(out, "%s", dt_data_type_get_string(slice->data_type));
    }
    else
    {
        fprintf(out, "?[?]");
    }
    fprintf(out, ") %s[", ir_lvalue_get_name(IR_LVALUE(slice)));

    if (slice->start != NULL)
    {  
        ir_node_print(IR_NODE(slice->start), out, 0);
        fprintf(out, "..");
        ir_node_print(IR_NODE(slice->end), out, 0);
    }
    fprintf(out, "]");
}
