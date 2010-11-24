struct iml_register_t;

extern (C)
{
    iml_register_t *
    iml_register_new(uint id, const char *name);

    uint
    iml_register_get_id(iml_register_t *self);

    char *
    iml_register_get_name(iml_register_t *self);
}
