#include "ir_return.h"
#include "utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_return_do_print(IrStatment *self, FILE *out, int indention);

static void
ir_return_class_init(gpointer klass, gpointer foo);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_return_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (IrReturnClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_return_class_init, /* class_init */
        NULL, /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrReturn),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_STATMENT,
                                    "IrReturnType",
                                    &info, 0);
    }
    return type;
}

IrReturn *
ir_return_new(IrExpression *return_value)
{
    IrReturn *obj;

    obj = g_object_new(IR_TYPE_RETURN, NULL);
    obj->return_value = return_value;

    return obj;
}

IrExpression *
ir_return_get_return_value(IrReturn *self)
{
    assert(self);
    assert(IR_IS_RETURN(self));

    return self->return_value;
}


/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ir_return_do_print(IrStatment *self, FILE *out, int indention)
{
    assert(self);
    assert(IR_IS_RETURN(self));
    assert(out);

    IrReturn *ret = IR_RETURN(self);

    fprintf_indent(out, indention, "return [%p]\n", ret);
}

static void
ir_return_class_init(gpointer klass, gpointer foo)
{
    ((IrStatmentClass *)klass)->do_print = ir_return_do_print;
}
