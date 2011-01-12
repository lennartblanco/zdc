#include "ir_ptr_dref.h"
#include "dt_pointer.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_ptr_dref_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_ptr_dref_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (IrPtrDrefClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_ptr_dref_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrPtrDref),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_EXPRESSION,
                                    "IrPtrDrefType",
                                    &info, 0);
    }
    return type;
}

IrPtrDref *
ir_ptr_dref_new(IrExpression *ptr_expression)
{
    IrPtrDref *obj;

    assert(IR_IS_EXPRESSION(ptr_expression));

    obj = g_object_new(IR_TYPE_PTR_DREF, NULL);
    obj->ptr_expression = ptr_expression;

    return obj;
}

IrExpression *
ir_ptr_dref_get_expression(IrPtrDref *self)
{
    assert(IR_IS_PTR_DREF(self));

    return self->ptr_expression;
}

guint
ir_ptr_dref_get_dest_size(IrPtrDref *self)
{
    assert(IR_IS_PTR_DREF(self));

    DtDataType *dt;

    dt = ir_expression_get_data_type(self->ptr_expression);
    assert(DT_IS_POINTER(dt));

    return dt_data_type_get_size(dt_pointer_get_base_type(DT_POINTER(dt)));
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static DtDataType *
ir_ptr_dref_do_get_data_type(IrExpression *self)
{
    assert(IR_IS_PTR_DREF(self));

    DtDataType *dt;

    dt = ir_expression_get_data_type(IR_PTR_DREF(self)->ptr_expression);
    assert(DT_IS_POINTER(dt));

    return dt_pointer_get_base_type(DT_POINTER(dt));
}

static bool
ir_ptr_dref_do_is_lvalue(IrExpression *self)
{
    assert(IR_IS_PTR_DREF(self));

    return true;
}

static void
ir_ptr_dref_class_init(gpointer klass, gpointer dummy)
{
    ((IrExpressionClass *)klass)->do_get_data_type =
        ir_ptr_dref_do_get_data_type;
    ((IrExpressionClass *)klass)->do_is_lvalue =
        ir_ptr_dref_do_is_lvalue;
}
