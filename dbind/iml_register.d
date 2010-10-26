module dbind.iml_register;

struct iml_register_t;

extern (C)
{
    iml_register_t *
    iml_register_new(uint id, const char *name);
}