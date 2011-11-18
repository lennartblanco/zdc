#include "ir_struct_member.h"
#include "dt_data_type.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_struct_member_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_struct_member_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (IrStructMemberClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_struct_member_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrStructMember),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_EXPRESSION,
                                    "IrStructMemberType",
                                    &info, 0);
    }
    return type;
}

IrStructMember *
ir_struct_member(void *obj)
{
    return G_TYPE_CHECK_INSTANCE_CAST((obj),
                                      IR_TYPE_STRUCT_MEMBER,
                                      IrStructMember);
}

IrStructMember *
ir_struct_member_new(DtDataType *type,
                     guint offset,
                     guint padding,
                     IrExpression *init)
{
    assert(DT_IS_DATA_TYPE(type));

    IrStructMember *obj;

    obj = g_object_new(IR_TYPE_STRUCT_MEMBER, NULL);
    obj->type = type;
    obj->offset = offset;
    obj->padding = padding;
    obj->init = init;

    return obj;
}

void
ir_struct_member_set_base(IrStructMember *self, IrExpression *base)
{
    assert(IR_IS_STRUCT_MEMBER(self));
    assert(IR_IS_EXPRESSION(base));

    self->base = base;
}

IrExpression *
ir_struct_member_get_base(IrStructMember *self)
{
    assert(IR_IS_STRUCT_MEMBER(self));

    return self->base;
}

guint
ir_struct_member_get_offset(IrStructMember *self)
{
    assert(IR_IS_STRUCT_MEMBER(self));

    return self->offset;
}

guint
ir_struct_member_get_padding(IrStructMember *self)
{
    assert(IR_IS_STRUCT_MEMBER(self));

    return self->padding;
}

IrExpression *
ir_struct_member_get_init(IrStructMember *self)
{
    assert(IR_IS_STRUCT_MEMBER(self));
    assert(IR_IS_EXPRESSION(self->init));

    return self->init;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static bool
ir_struct_member_do_is_lvalue(IrExpression *self)
{
    assert(IR_IS_STRUCT_MEMBER(self));

    return true;
}

static DtDataType *
ir_struct_member_do_get_data_type(IrExpression *self)
{
    assert(IR_IS_STRUCT_MEMBER(self));

    return ir_struct_member(self)->type;
}

static void
ir_struct_member_class_init(gpointer klass, gpointer dummy)
{
    IR_EXPRESSION_CLASS(klass)->do_is_lvalue = ir_struct_member_do_is_lvalue;
    IR_EXPRESSION_CLASS(klass)->do_get_data_type =
        ir_struct_member_do_get_data_type;
}
