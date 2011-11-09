public import ir_expression;

struct IrEnumMember;

extern (C)
{
    IrExpression *
    ir_enum_member_get_value(IrEnumMember *self);
}
