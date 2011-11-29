alias size_t GType;
struct GObject;

extern (C)
{

    GType
    g_type_from_instance(void *obj);

    bool
    g_is_object(void *obj);

    char *
    g_type_name(GType type);
}




