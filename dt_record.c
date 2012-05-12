#include "dt_record.h"
#include "ir_variable.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

enum
{
    DT_RECORD_MEMBERS = 1
};

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
dt_record_class_init(gpointer klass, gpointer dummy);

static guint
get_align_padding(int addr, int alignment);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_record_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (DtRecordClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        dt_record_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (DtRecord),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(DT_TYPE_USER,
                                    "DtRecordType",
                                    &info, 0);
    }
    return type;
}

GSList *
dt_record_get_members(DtRecord *self)
{
    assert(DT_IS_RECORD(self));

    return self->members;
}

void
dt_record_set_members(DtRecord *self, GSList *members)
{
    assert(DT_IS_RECORD(self));

    GSList *i;
    GSList *new_members = NULL;

    assert(self->members_table == NULL);
    self->members_table = g_hash_table_new(g_str_hash, g_str_equal);

    for (i = members; i != NULL; i = g_slist_next(i))
    {
        assert(IR_IS_VARIABLE(i->data));
        IrVariable *var = i->data;

        /* calculate required padding */
        guint type_size = dt_data_type_get_size(ir_variable_get_data_type(var));
        guint padding = get_align_padding(self->size + type_size, 4);

        /* create struct member object */
        IrStructMember *member =
            ir_struct_member_new(ir_variable_get_data_type(var),
                                 self->size,
                                 padding,
                                 ir_variable_get_initializer(var));

        /* store struct member in the table and sequential list */
        g_hash_table_insert(self->members_table,
                            g_strdup(ir_variable_get_name(var)),
                            member);
        new_members = g_slist_prepend(new_members, member);

        /* update struct size */
        self->size += type_size + padding;
    }
    self->members = g_slist_reverse(new_members);
}

const IrStructMember *
dt_record_get_member(DtRecord *self, IrIdent *name)
{
    assert(DT_IS_RECORD(self));
    assert(IR_IS_IDENT(name));

    return g_hash_table_lookup(self->members_table, ir_ident_get_name(name));
}

IrStructLiteral *
dt_record_get_init_blob(DtRecord *self)
{
    assert(DT_IS_RECORD(self));
    assert(DT_RECORD_GET_CLASS(self)->get_init_blob);

    return DT_RECORD_GET_CLASS(self)->get_init_blob(self);
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ir_record_set_property(GObject *object,
                       guint property_id,
                       const GValue *value,
                       GParamSpec *pspec)
{
    assert(DT_IS_RECORD(object));
    /* we only have one property */
    assert(property_id == DT_RECORD_MEMBERS);

    DT_RECORD(object)->members = g_value_get_pointer(value);
}

static void
ir_record_get_property(GObject *object,
                       guint property_id,
                       GValue *value,
                       GParamSpec *pspec)
{
    /* not implemented */
    assert(false);
}

static guint
dt_record_get_size(DtDataType *self)
{
    assert(DT_IS_RECORD(self));

    return DT_RECORD(self)->size;
}

static void
dt_record_class_init(gpointer klass, gpointer dummy)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    GParamSpec *pspec;

    /*
     * setup this structure for setting and getting properties
     */
    gobject_class->set_property = ir_record_set_property;
    gobject_class->get_property = ir_record_get_property;

    /*
     * install 'members' property
     */
    pspec = g_param_spec_pointer("dt-record-members",
                                 "dt record members",
                                 "record members",
                                 G_PARAM_CONSTRUCT | G_PARAM_READWRITE);

    g_object_class_install_property(gobject_class,
                                    DT_RECORD_MEMBERS,
                                    pspec);

    /* install virtual methods implementations */
    DT_DATA_TYPE_CLASS(klass)->get_size = dt_record_get_size;
}


/**
 * Calculated the number of padding bytes that need to be inserted to
 * align an address to specified alignment.
 */
static
guint get_align_padding(int addr, int alignment)
{
  int mod = addr % alignment;
  return mod == 0 ? 0 : alignment - mod;
}
