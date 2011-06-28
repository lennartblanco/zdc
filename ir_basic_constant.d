struct IrBasicConstant;

extern (C)
{
    int
    ir_basic_constant_get_int(IrBasicConstant *self);

    uint
    ir_basic_constant_get_uint(IrBasicConstant *self);

    bool
    ir_basic_constant_get_bool(IrBasicConstant *self);

    ubyte
    ir_basic_constant_get_char(IrBasicConstant *self);
}