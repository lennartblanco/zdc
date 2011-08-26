public import GSList;

struct IrStructLiteral;

extern (C)
{
    bool
    ir_is_struct_literal(void *obj);

    GSList *
    ir_struct_literal_get_members(IrStructLiteral *self);
}
