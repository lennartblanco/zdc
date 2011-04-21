struct ImlOperand;

enum iml_data_type_t
{
    iml_8b,
    iml_16b,
    iml_32b,
    iml_ptr,
    iml_blob
};

extern (C)
{
    iml_data_type_t
    iml_operand_get_data_type(ImlOperand *self);
}