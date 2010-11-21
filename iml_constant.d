struct ImlConstant;

extern (C)
{
    bool
    iml_is_constant(void *self);
    
    uint
    iml_constant_get_val_32b(ImlConstant *self);
}