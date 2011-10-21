struct IrBasicConstant;

extern (C)
{
    bool
    ir_basic_constant_get_bool(IrBasicConstant *self);

    char
    ir_basic_constant_get_char(IrBasicConstant *self);

    byte
    ir_basic_constant_get_byte(IrBasicConstant *self);

    ubyte
    ir_basic_constant_get_ubyte(IrBasicConstant *self);

    short
    ir_basic_constant_get_short(IrBasicConstant *self);

    ushort
    ir_basic_constant_get_ushort(IrBasicConstant *self);

    int
    ir_basic_constant_get_int(IrBasicConstant *self);

    uint
    ir_basic_constant_get_uint(IrBasicConstant *self);
}