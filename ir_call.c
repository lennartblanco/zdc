#include "ir_call.h"
#include "dt_basic.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

enum
{
    IR_CALL_ARGUMENTS = 1
};

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_call_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_call_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (IrCallClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_call_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrCall),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_EXPRESSION,
                                    "IrCallType",
                                    &info, 0);
    }
    return type;
}

void
ir_call_set_function(IrCall *self, IrFunction *function)
{
    assert(IR_IS_CALL(self));
    assert(IR_IS_FUNCTION(function));

    self->function = function;
}

IrFunction *
ir_call_get_function(IrCall *self)
{
    assert(IR_IS_CALL(self));

    return self->function;
}

GSList *
ir_call_get_arguments(IrCall *self)
{
    assert(IR_IS_CALL(self));

    return self->arguments;
}

IrExpression *
ir_call_get_this_arg(IrCall *self)
{
    assert(IR_IS_CALL(self));
    assert(IR_CALL_GET_CLASS(self)->do_get_this_arg);

    return IR_CALL_GET_CLASS(self)->do_get_this_arg(self);
}

void
ir_call_set_arguments(IrCall *self, GSList *arguments)
{
    assert(IR_IS_CALL(self));

    self->arguments = arguments;
}

ir_linkage_type_t
ir_call_get_linkage(IrCall *self)
{
    assert(IR_IS_CALL(self));

    return ir_function_get_linkage(self->function);
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ir_call_set_property(GObject *object,
                     guint property_id,
                     const GValue *value,
                     GParamSpec *pspec)
{
    assert(IR_IS_CALL(object));
    /* we only have one property */
    assert(property_id == IR_CALL_ARGUMENTS);

    IR_CALL(object)->arguments = g_value_get_pointer(value);
}

static void
ir_call_get_property(GObject *object,
                     guint property_id,
                     GValue *value,
                     GParamSpec *pspec)
{
    /* not implemented */
    assert(false);
}

static DtDataType *
ir_call_do_get_data_type(IrExpression *self)
{
    assert(IR_IS_CALL(self));

    return ir_function_get_return_type(IR_CALL(self)->function);
}

static UtRange *
ir_call_get_value_range(IrExpression *self)
{
    assert(IR_IS_CALL(self));

    return dt_basic_get_value_range(dt_basic(ir_call_do_get_data_type(self)));
}

static void
ir_call_class_init(gpointer klass, gpointer dummy)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    GParamSpec *pspec;

    /*
     * setup this structure for setting and getting properties
     */
    gobject_class->set_property = ir_call_set_property;
    gobject_class->get_property = ir_call_get_property;

    /*
     * install 'call-arguments' property
     */
    pspec = g_param_spec_pointer("ir-call-arguments",
                                 "ir call arguments",
                                 "call arguments",
                                 G_PARAM_CONSTRUCT | G_PARAM_READWRITE);

    g_object_class_install_property(gobject_class,
                                    IR_CALL_ARGUMENTS,
                                    pspec);

    /*
     * override virtual methods
     */
    IR_EXPRESSION_CLASS(klass)->do_get_data_type = ir_call_do_get_data_type;
    IR_EXPRESSION_CLASS(klass)->get_value_range = ir_call_get_value_range;
}
