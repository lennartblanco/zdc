#include "dt_struct.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

typedef struct
{
    DtDataType *type;
    gchar *name;
    guint offset;
} struct_member;

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
      type = g_type_register_static(DT_TYPE_USER,
                                    "DtStructType",
                                    &info, 0);
    }
    return type;
}

DtStruct *
dt_struct_new(gchar *name, IrModule *parent_module)
{
    DtStruct *obj;

    obj = g_object_new(DT_TYPE_STRUCT,
                       "dt-user-name", name,
                       "dt-user-parent-module", parent_module,
                       NULL);

    obj->size = 0;
    obj->members = g_hash_table_new(g_str_hash, g_str_equal);
    obj->init = NULL;

    return obj;
}

void
dt_struct_set_init(DtStruct *self, IrStructLiteral *init)
{
    assert(DT_IS_STRUCT(self));
    assert(ir_is_struct_literal(init));

    self->init = init;
}

void
dt_struct_add_member(DtStruct *self,
                     DtDataType *member_type,
                     const gchar *member_name)
{
    assert(DT_IS_STRUCT(self));
    assert(DT_IS_DATA_TYPE(self));
    assert(member_name);

    struct_member *member;

    member = g_malloc(sizeof(*member));
    member->type = member_type;
    member->name = g_strdup(member_name);
    member->offset = self->size;

    g_hash_table_insert(self->members, member->name, member);

    self->size += dt_data_type_get_size(member_type);
}

IrStructMember *
dt_struct_get_member(DtStruct *self, const gchar *name)
{
    assert(DT_IS_STRUCT(self));
    assert(name);

    struct_member *inter_member;

    inter_member = g_hash_table_lookup(self->members, name);
    if (inter_member == NULL)
    {
        /* member not found */
        return NULL;
    }

    return ir_struct_member_new(inter_member->type, inter_member->offset);
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static guint
dt_struct_get_size(DtDataType *self)
{
    assert(DT_IS_STRUCT(self));

    return DT_STRUCT(self)->size;
}

static IrExpression *
dt_struct_get_init(DtDataType *self)
{
    assert(DT_IS_STRUCT(self));
    assert(ir_is_struct_literal(DT_STRUCT(self)->init));

    return IR_EXPRESSION(DT_STRUCT(self)->init);
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
