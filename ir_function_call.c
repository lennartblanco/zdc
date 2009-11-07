#include <string.h>

#include "ir_function_call.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_function_call_class_init(gpointer klass, gpointer dummy);

static DtDataType *
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
ir_function_call_new(char *name,
                     GSList *arguments,
                     guint line_number)
{
    IrFunctionCall *obj;

    obj = g_object_new(IR_TYPE_FUNCTION_CALL,
                       "ir-node-line-number", line_number,
                       NULL);

    obj->name = strdup(name);
    obj->arguments = arguments;
    obj->return_type = NULL;
    obj->linkage = ir_d_linkage;

    return obj;
}

void
ir_function_call_set_return_type(IrFunctionCall *self,
                                 DtDataType *return_type)
{
    assert(IR_IS_FUNCTION_CALL(self));

    self->return_type = return_type;
}

void
ir_function_call_set_linkage(IrFunctionCall *self,
                             ir_linkage_type_t linkage)
{
    assert(IR_IS_FUNCTION_CALL(self));

    self->linkage = linkage;
}

ir_linkage_type_t
ir_function_call_get_linkage(IrFunctionCall *self)
{
    assert(IR_IS_FUNCTION_CALL(self));

    return self->linkage;
}

GSList *
ir_function_call_get_arguments(IrFunctionCall *self)
{
    assert(IR_IS_FUNCTION_CALL(self));

    return self->arguments;
}

void
ir_function_call_set_arguments(IrFunctionCall *self, GSList *arguments)
{
    assert(IR_IS_FUNCTION_CALL(self));

    self->arguments = arguments;
}

char *
ir_function_call_get_name(IrFunctionCall *self)
{
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

static DtDataType *
ir_function_call_do_get_data_type(IrExpression *self)
{
  return IR_FUNCTION_CALL(self)->return_type;
}
