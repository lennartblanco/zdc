#include "dt_struct.h"
#include "ir_function_def.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
dt_struct_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_struct_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (DtStructClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        dt_struct_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (DtStruct),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(DT_TYPE_RECORD,
                                    "DtStructType",
                                    &info, 0);
    }
    return type;
}

DtStruct *
dt_struct_new(gchar *name,
              GSList *members,
              GSList *methods,
              bool opaque,
              IrModule *parent_module,
              sym_table_t *symbols)
{
    DtStruct *obj;

    obj = g_object_new(DT_TYPE_STRUCT,
                       "ir-symbol-name", name,
                       "ir-symbol-parent-module", parent_module,
                       "ir-symbol-scope", ir_module_get_scope(parent_module),
                       "dt-record-members", members,
                       NULL);

    obj->methods = methods;
    obj->opaque = opaque;
    if (!opaque)
    {
        obj->methods_table = g_hash_table_new(g_str_hash, g_str_equal);
    }
    obj->init = NULL;

    return obj;
}

GSList *
dt_struct_get_members(DtStruct *self)
{
    return dt_record_get_members(DT_RECORD(self));
}

void
dt_struct_set_members(DtStruct *self, GSList *members)
{
    assert(DT_IS_STRUCT(self));
    assert(!self->opaque);

    dt_record_set_members(DT_RECORD(self), members);
}

GSList *
dt_struct_get_methods(DtStruct *self)
{
    assert(DT_IS_STRUCT(self));

    return self->methods;
}

bool
dt_struct_is_opaque(DtStruct *self)
{
    assert(DT_IS_STRUCT(self));

    return self->opaque;
}

const IrStructMember *
dt_struct_get_member(DtStruct *self, IrIdent *name)
{
    assert(!self->opaque);

    return dt_record_get_member(DT_RECORD(self), name);
}

void
dt_struct_set_method(DtStruct *self, IrFunctionDef *method)
{
    assert(DT_IS_STRUCT(self));
    assert(!self->opaque);
    assert(IR_IS_FUNCTION_DEF(method));

    g_hash_table_insert(self->methods_table,
                        ir_function_def_get_name(method),
                        method);
}

IrFunctionDef *
dt_struct_get_method(DtStruct *self, const char *method_name)
{
    assert(DT_IS_STRUCT(self));
    assert(!self->opaque);
    assert(method_name);

    return g_hash_table_lookup(self->methods_table, method_name);
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static guint
dt_struct_get_size(DtDataType *self)
{
    assert(DT_IS_STRUCT(self));

    return DT_RECORD(self)->size;
}

static IrExpression *
dt_struct_get_init(DtDataType *self)
{
    assert(DT_IS_STRUCT(self));
    DtStruct *dt_struct = DT_STRUCT(self);
    if (dt_struct->init == NULL)
    {
        dt_struct->init =
            ir_struct_literal_new(dt_record_get_members(DT_RECORD(dt_struct)));
    }

    return ir_expression(dt_struct->init);
}

static gchar *
dt_struct_get_mangled_prefix(DtUser *self)
{
    assert(DT_IS_STRUCT(self));

    return "S";
}

static void
dt_struct_class_init(gpointer klass, gpointer dummy)
{
    DT_USER_CLASS(klass)->get_mangled_prefix = dt_struct_get_mangled_prefix;
    DT_DATA_TYPE_CLASS(klass)->get_size = dt_struct_get_size;
    DT_DATA_TYPE_CLASS(klass)->get_init = dt_struct_get_init;
}
