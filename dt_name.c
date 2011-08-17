#include "dt_name.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static char *
dt_user_type_get_string(DtDataType *self);

static void
dt_user_type_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_name_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (DtNameClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        dt_user_type_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (DtName),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(DT_TYPE_DATA_TYPE,
                                    "DtNameType",
                                    &info, 0);
    }
    return type;
}

DtName *
dt_name_new(gchar *name, guint line_number)
{
    DtName *obj;

    obj = g_object_new(DT_TYPE_NAME,
                       "dt-data-type-line-number", line_number,
                       NULL);
    obj->name = g_strdup(name);

    return obj;
}

gchar *
dt_name_get_name(DtName *self)
{
    assert(DT_IS_NAME(self));

    return self->name;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static char *
dt_user_type_get_string(DtDataType *self)
{
    assert(DT_IS_NAME(self));

    return g_strdup_printf("[user type '%s']",
                           DT_NAME(self)->name);
}

static void
dt_user_type_class_init(gpointer klass, gpointer dummy)
{
    DT_DATA_TYPE_CLASS(klass)->get_string = dt_user_type_get_string;
}

