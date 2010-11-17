public import iml_register;

struct ImlVariable;

extern (C)
{
    iml_register_t *
    iml_variable_get_register(ImlVariable *self);

    void
    iml_variable_set_frame_offset(ImlVariable *self, int frame_offset);
}