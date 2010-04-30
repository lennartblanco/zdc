#include "ir_cast.h"
#include "utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_cast_class_init(gpointer klass, gpointer dummy);

static DtDataType *
ir_cast_do_get_data_type(IrExpression *self);

static void
ir_cast_do_print(IrStatment *self, FILE *out, int indention);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ir_cast_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (IrCastClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_cast_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrCast),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_EXPRESSION,
                                    "IrCastType",
                                    &info, 0);
    }
    return type;
}

IrCast *
ir_cast_new(DtDataType *target_type, IrExpression *value)
{
    IrCast *obj;

    obj = g_object_new(IR_TYPE_CAST, NULL);
    obj->target_type = target_type;
    obj->value = value;

    return obj;
}

DtDataType *
ir_cast_get_target_type(IrCast *self)
{
    assert(self);
    assert(IR_IS_CAST(self));

    return self->target_type;
}

IrExpression *
ir_cast_get_value(IrCast *self)
{
    assert(self);
    assert(IR_IS_CAST(self));

    return self->value;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ir_cast_class_init(gpointer klass, gpointer dummy)
{
    ((IrExpressionClass *)klass)->do_get_data_type = ir_cast_do_get_data_type;
    ((IrStatmentClass *)klass)->do_print = ir_cast_do_print;
}

static DtDataType *
ir_cast_do_get_data_type(IrExpression *self)
{
    return IR_CAST(self)->target_type;
}

static void
ir_cast_do_print(IrStatment *self, FILE *out, int indention)
{
    assert(self);
    assert(IR_IS_CAST(self));
    assert(out);

    IrCast *cast = IR_CAST(self);

    fprintf_indent(out, indention, "cast [%p]\n", cast);
    fprintf_indent(out, indention + 2, "target_type: %s\n",
                   dt_data_type_get_string(cast->target_type));
    fprintf_indent(out, indention + 2, "value: ");
    ir_statment_print(IR_STATMENT(cast->value), out, 0);
}
