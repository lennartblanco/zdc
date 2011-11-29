#include <glib-object.h>
#include <stdbool.h>

/**
 * export G_TYPE_FROM_INSTANCE() macro to D code
 */
GType
g_type_from_instance(void *obj)
{
    return G_TYPE_FROM_INSTANCE(obj);
}

/**
 * export G_IS_OBJECT macro to D code
 */
bool
g_is_object(void *obj)
{
    return G_IS_OBJECT(obj);
}
