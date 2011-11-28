#include <glib-object.h>

/**
 * export G_TYPE_FROM_INSTANCE() macro to D code
 */
GType
g_type_from_instance(GObject *obj)
{
    return G_TYPE_FROM_INSTANCE(obj);
}

/**
 * export G_OBJECT() macro to D code
 *
 * (Casts a GObject or derived pointer into a (GObject*) pointer with run-time
 *  checks.)
 */
GObject *
g_object(void *obj)
{
    return G_OBJECT(obj);
}
