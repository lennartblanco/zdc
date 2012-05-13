#include "dt_class.h"
#include "ir_module.h"
#include "ir_null.h"
#include "types.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
dt_class_class_init(gpointer klass, gpointer dummy);

static GSList *
get_prelude_members();

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_class_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (DtClassClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        dt_class_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (DtClass),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(DT_TYPE_RECORD,
                                    "DtClassType",
                                    &info, 0);
    }
    return type;
}

DtClass *
dt_class_new(gchar *name, GSList *members, IrModule *parent_module)
{
    DtClass *obj;

    obj = g_object_new(DT_TYPE_CLASS,
                       "ir-symbol-name", name,
                       "ir-symbol-scope", ir_module_get_scope(parent_module),
                       "dt-record-members",
                            g_slist_concat(get_prelude_members(), members),
                       NULL);
    obj->blob_init = NULL;

    return obj;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static GSList *
get_prelude_members()
{
    static GSList *sys_mbrs = NULL;

    if (sys_mbrs == NULL)
    {
        /* insert dummy '__vptr' member */
        sys_mbrs =
            g_slist_append(sys_mbrs,
                           ir_variable_new(false,
                                           types_get_void_ptr(),
                                           "__vptr",
                                           ir_expression(ir_null_get()),
                                           0));

        /* insert dummy '__monitor' member */
        sys_mbrs =
            g_slist_append(sys_mbrs,
                           ir_variable_new(false,
                                           types_get_void_ptr(),
                                           "__monitor",
                                           ir_expression(ir_null_get()),
                                           0));
    }

    return sys_mbrs;
}

static gchar *
dt_class_get_mangled_prefix(DtUser *self)
{
    assert(DT_IS_CLASS(self));

    return "C";
}

static IrExpression *
dt_class_get_init(DtDataType *self)
{
    assert(DT_IS_CLASS(self));

    return ir_expression(ir_null_get());
}

static IrStructLiteral *
dt_class_get_init_blob(DtRecord *self)
{
    assert(DT_IS_CLASS(self));

    DtClass *this = DT_CLASS(self);

    if (this->blob_init == NULL)
    {
        this->blob_init = ir_struct_literal_new(dt_record_get_members(self));
    }

    return this->blob_init;
}


static void
dt_class_class_init(gpointer klass, gpointer dummy)
{
    DT_RECORD_CLASS(klass)->get_init_blob = dt_class_get_init_blob;
    DT_USER_CLASS(klass)->get_mangled_prefix = dt_class_get_mangled_prefix;
    DT_DATA_TYPE_CLASS(klass)->get_init = dt_class_get_init;
}
