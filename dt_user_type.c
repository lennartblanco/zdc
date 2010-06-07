#include "dt_user_type.h"

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
dt_user_type_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (DtUserTypeClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        dt_user_type_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (DtUserType),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(DT_TYPE_DATA_TYPE,
                                    "DtUserTypeType",
                                    &info, 0);
    }
    return type;
}

DtUserType *
dt_user_type_new(gchar *name, guint line_number)
{
    DtUserType *obj;

    obj = g_object_new(DT_TYPE_USER_TYPE, NULL);
    obj->name = g_strdup(name);
    obj->line_number = line_number;

    return obj;
}

gchar *
dt_user_type_get_name(DtUserType *self)
{
    assert(DT_IS_USER_TYPE(self));

    return self->name;
}

guint
dt_user_type_get_line_num(DtUserType *self)
{
    assert(DT_IS_USER_TYPE(self));

    return self->line_number;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static char *
dt_user_type_get_string(DtDataType *self)
{
    assert(DT_IS_USER_TYPE(self));

    return g_strdup_printf("[user type '%s']",
                           DT_USER_TYPE(self)->name);
}

static void
dt_user_type_class_init(gpointer klass, gpointer dummy)
{
    ((DtDataTypeClass *)klass)->get_string = dt_user_type_get_string;
}

