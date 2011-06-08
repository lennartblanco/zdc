struct ImlOperand;

enum iml_data_type
{
    _8b,
    _16b,
    _32b,
    ptr,
    blob
};

extern (C)
{
    iml_data_type
    iml_operand_get_data_type(ImlOperand *self);
}