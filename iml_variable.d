import iml_operand;

struct ImlVariable;

extern (C)
{
    bool
    iml_is_variable(void *obj);

    char *
    iml_variable_get_register(ImlVariable *self);

    void
    iml_variable_set_frame_offset(ImlVariable *self, int frame_offset);

    int
    iml_variable_get_frame_offset(ImlVariable *self);

    iml_data_type
    iml_variable_get_data_type(ImlVariable *self);

    uint
    iml_variable_get_size(ImlVariable *self);
}
