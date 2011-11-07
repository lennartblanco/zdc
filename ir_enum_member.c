#include "ir_enum_member.h"
#include "utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_enum_member_class_init(gpointer klass, gpointer dummy);

static DtDataType *
ir_enum_member_do_get_data_type(IrExpression *self);

static void
ir_enum_member_do_print(IrNode *self, FILE *out, int indention);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_enum_member_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info =
      {
        sizeof (IrEnumMemberClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_enum_member_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrEnumMember),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_SYMBOL,
                                    "IrEnumMemberType",
                                    &info, 0);
    }
    return type;
}

IrEnumMember *
ir_enum_member_new(DtEnum *enum_type, gchar *name, IrExpression *value)
{
    IrEnumMember *obj;

    obj = g_object_new(IR_TYPE_ENUM_MEMBER,
                       "ir-symbol-name", name,
                       NULL);

    obj->enum_type = enum_type;
    obj->value = value;

    return obj;
}

IrExpression *
ir_enum_member_get_value(IrEnumMember *self)
{
    assert(IR_IS_ENUM_MEMBER(self));

    return self->value;
}

void
ir_enum_member_set_value(IrEnumMember *self, IrExpression *value)
{
    assert(IR_IS_ENUM_MEMBER(self));
    assert(IR_IS_EXPRESSION(value));

    self->value = value;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ir_enum_member_class_init(gpointer klass, gpointer dummy)
{
    IR_NODE_CLASS(klass)->do_print = ir_enum_member_do_print;
    IR_EXPRESSION_CLASS(klass)->do_get_data_type =
        ir_enum_member_do_get_data_type;

}

static DtDataType *
ir_enum_member_do_get_data_type(IrExpression *self)
{
    assert(IR_IS_ENUM_MEMBER(self));
    IrEnumMember *enum_member = IR_ENUM_MEMBER(self);

    if (dt_enum_is_anonymous(enum_member->enum_type))
    {
        return ir_expression_get_data_type(enum_member->value);
    }

    return DT_DATA_TYPE(enum_member->enum_type);
}

static void
ir_enum_member_do_print(IrNode *self, FILE *out, int indention)
{
    assert(IR_IS_ENUM_MEMBER(self));
    assert(out);

    IrEnumMember *em = IR_ENUM_MEMBER(self);

    fprintf_indent(out, indention,
                   "enum member\n"
                   "  value:\n");

    if (em->value == NULL)
    {
        fprintf_indent(out, indention + 4, "unknown\n");
    }
    else
    {
        ir_node_print(IR_NODE(em->value), out, indention + 4);
        fprintf_indent(out, 0, "\n");
    }
}
