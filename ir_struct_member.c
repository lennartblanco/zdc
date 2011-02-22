#include "ir_struct_member.h"

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
ir_struct_member_new(DtDataType *type, guint offset)
{
    IrStructMember *obj;

    obj = g_object_new(IR_TYPE_STRUCT_MEMBER, NULL);
    obj->type = type;
    obj->offset = offset;

    return obj;
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

    return IR_STRUCT_MEMBER(self)->type;
}

static void
ir_struct_member_class_init(gpointer klass, gpointer dummy)
{
    IR_EXPRESSION_CLASS(klass)->do_is_lvalue = ir_struct_member_do_is_lvalue;
    IR_EXPRESSION_CLASS(klass)->do_get_data_type =
        ir_struct_member_do_get_data_type;
}
