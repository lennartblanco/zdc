#include <string.h>

#include "ir_function_call.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_function_call_class_init(gpointer klass, gpointer dummy);

static AstDataType *
ir_function_call_do_get_data_type(IrExpression *self);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_function_call_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (IrFunctionCallClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_function_call_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrFunctionCall),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_EXPRESSION,
                                    "IrFunctionCallType",
                                    &info, 0);
    }
    return type;
}

IrFunctionCall *
ir_function_call_new(AstDataType *return_type,
                     char *name,
                     GSList *arguments)
{
    IrFunctionCall *obj;

    obj = g_object_new(IR_TYPE_FUNCTION_CALL, NULL);

    obj->name = strdup(name);
    obj->arguments = arguments;
    obj->return_type = return_type;

    return obj;
}

GSList *
ir_function_call_get_arguments(IrFunctionCall *self)
{
    assert(self);
    assert(IR_IS_FUNCTION_CALL(self));

    return self->arguments;
}

char *
ir_function_call_get_name(IrFunctionCall *self)
{
    assert(self);
    assert(IR_IS_FUNCTION_CALL(self));

    return self->name;
}


/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ir_function_call_class_init(gpointer klass, gpointer dummy)
{
    ((IrExpressionClass *)klass)->do_get_data_type =
        ir_function_call_do_get_data_type;
}

static AstDataType *
ir_function_call_do_get_data_type(IrExpression *self)
{
  return IR_FUNCTION_CALL(self)->return_type;
}