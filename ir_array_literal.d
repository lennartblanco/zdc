public import GSList;

struct IrArrayLiteral;

extern (C)
{
    bool
    ir_is_array_literal(void *obj);

    char *
    ir_array_literal_get_data_label(IrArrayLiteral *self);

    GSList *
    ir_array_literal_get_values(IrArrayLiteral *self);
}
