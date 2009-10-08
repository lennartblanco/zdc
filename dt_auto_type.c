#include "dt_auto_type.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
dt_auto_type_do_print(AstNode *self, FILE *out);

static void
dt_auto_type_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_auto_type_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (DtAutoTypeClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        dt_auto_type_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (DtAutoType),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(DT_TYPE_DATA_TYPE,
                                    "DtAutoTypeType",
                                    &info, 0);
    }
    return type;
}

DtAutoType *
dt_auto_type_new()
{
    DtAutoType *obj;

    obj = g_object_new(DT_TYPE_AUTO_TYPE, NULL);

    return obj;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
dt_auto_type_do_print(AstNode *self, FILE *out)
{
    fprintf(out, "auto");
}

static void
dt_auto_type_class_init(gpointer klass, gpointer dummy)
{
    ((AstNodeClass *)klass)->do_print = dt_auto_type_do_print;
}

