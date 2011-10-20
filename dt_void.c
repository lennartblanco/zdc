#include "dt_void.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
dt_void_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_void_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (DtVoidClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        dt_void_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (DtVoid),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(DT_TYPE_DATA_TYPE,
                                    "DtVoidType",
                                    &info, 0);
    }
    return type;
}

DtVoid *
dt_void_new()
{
    DtVoid *obj;

    obj = g_object_new(DT_TYPE_VOID, NULL);

    return obj;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static char *
dt_void_get_mangled(DtDataType *self)
{
    assert(DT_IS_VOID(self));

    return "v";
}

static char *
dt_void_get_string(DtDataType *self)
{
    assert(DT_IS_VOID(self));

    return "void";
}

static bool
dt_void_is_same(DtDataType *self, DtDataType *type)
{
    assert(DT_IS_VOID(self));
    assert(DT_IS_DATA_TYPE(type));

    return DT_IS_VOID(type);
}

static void
dt_void_class_init(gpointer klass, gpointer dummy)
{
    DT_DATA_TYPE_CLASS(klass)->get_mangled = dt_void_get_mangled;
    DT_DATA_TYPE_CLASS(klass)->get_string = dt_void_get_string;
    DT_DATA_TYPE_CLASS(klass)->is_same = dt_void_is_same;
}

