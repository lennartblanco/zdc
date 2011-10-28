public import ir_expression;

struct IrStructMember;

extern (C)
{
    IrExpression *
    ir_struct_member_get_init(IrStructMember *self);

    uint
    ir_struct_member_get_padding(IrStructMember *self);
}
