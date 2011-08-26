public import GSList;

struct IrArrayLiteral;

extern (C)
{
    bool
    ir_is_array_literal(void *obj);

    GSList *
    ir_array_literal_get_values(IrArrayLiteral *self);
}
