#include "ir_enum.h"
#include "utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_enum_class_init(gpointer klass, gpointer foo);

static void
ir_enum_do_print(IrNode *self, FILE *out, int indention);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_enum_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (IrEnumClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_enum_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrEnum),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_NODE,
                                    "IrEnumType",
                                    &info, 0);
    }
    return type;
}

IrEnum *
ir_enum_new(gchar *tag,
            DtDataType *base_type,
            GSList *members,
            guint line_number)
{
    IrEnum *obj;

    obj = g_object_new(IR_TYPE_ENUM,
                       "ir-node-line-number", line_number,
                       NULL);

    obj->tag = g_strdup(tag);
    obj->members = members;
    obj->data_type = dt_enum_type_new(tag, base_type, members->data);

    return obj;
}

gchar *
ir_enum_get_tag(IrEnum *self)
{
    assert(IR_IS_ENUM(self));

    return self->tag;
}

DtDataType *
ir_enum_get_base_type(IrEnum *self)
{
    assert(IR_IS_ENUM(self));

    return dt_enum_type_get_base_type(self->data_type);
}

void
ir_enum_set_base_type(IrEnum *self, DtDataType *base_type)
{
    assert(IR_IS_ENUM(self));
    assert(DT_IS_DATA_TYPE(base_type));

    dt_enum_type_set_base_type(self->data_type, base_type);
}

GSList *
ir_enum_get_members(IrEnum *self)
{
    assert(IR_IS_ENUM(self));

    return self->members;
}

DtEnumType *
ir_enum_get_data_type(IrEnum *self)
{
    assert(IR_IS_ENUM(self));

    return self->data_type;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ir_enum_do_print(IrNode *self, FILE *out, int indention)
{
    assert(IR_IS_ENUM(self));
    assert(out);

    IrEnum *e = IR_ENUM(self);
    DtDataType *base_type;
    GSList *i;

    base_type = dt_enum_type_get_base_type(e->data_type);

    fprintf_indent(out, indention,
                   "enum [%p]\n"
                   "  tag: '%s'\n"
                   "  base type: '%s'\n"
                   "  members:\n",
                   e, e->tag,
                   base_type ? dt_data_type_get_string(base_type) : "unknow");

    for (i = e->members; i != NULL; i = g_slist_next(i))
    {
        ir_node_print(i->data, out, indention + 4);
    }
}

static void
ir_enum_class_init(gpointer klass, gpointer foo)
{
    ((IrNodeClass *)klass)->do_print = ir_enum_do_print;
}

