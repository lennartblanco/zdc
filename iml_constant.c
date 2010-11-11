#include <stdbool.h>
#include "iml_constant.h"
#include "utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static iml_data_type_t
iml_constant_do_get_data_type(ImlOperand *self);

static void
iml_constant_do_print_short(ImlOperand *self, FILE *out, guint indention);

static void
iml_constant_class_init(gpointer klass, gpointer foo);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
iml_constant_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (ImlConstantClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        iml_constant_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (ImlConstant),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IML_TYPE_OPERAND,
                                    "ImlConstantType",
                                    &info, 0);
    }
    return type;
}

ImlConstant *
iml_constant_new_8b(guint8 val)
{
    ImlConstant *obj;

    obj = g_object_new(IML_TYPE_CONSTANT, NULL);

    obj->datatype = iml_8b;
    obj->value.v8 = val;

    return obj;
}

ImlConstant *
iml_constant_new_16b(guint16 val)
{
    assert(false); /* not implemented */
}

ImlConstant *
iml_constant_new_32b(guint32 val)
{
    ImlConstant *obj;

    obj = g_object_new(IML_TYPE_CONSTANT, NULL);

    obj->datatype = iml_32b;
    obj->value.v32 = val;

    return obj;
}

ImlConstant *
iml_constant_new_ptr(const gchar *label)
{
    ImlConstant *obj;

    obj = g_object_new(IML_TYPE_CONSTANT, NULL);

    obj->datatype = iml_ptr;
    obj->value.ptr_label = label;

    return obj;
}

guint8
iml_constant_get_val_8b(ImlConstant *self)
{
    assert(IML_IS_CONSTANT(self));

    return self->value.v8;
}

guint32
iml_constant_get_val_32b(ImlConstant *self)
{
	assert(IML_IS_CONSTANT(self));

	return self->value.v32;
}

const gchar *
iml_constant_get_val_ptr(ImlConstant *self)
{
    assert(IML_IS_CONSTANT(self));

    return self->value.ptr_label;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static iml_data_type_t
iml_constant_do_get_data_type(ImlOperand *self)
{
    assert(IML_IS_CONSTANT(self));

    return IML_CONSTANT(self)->datatype;
}

static void
iml_constant_do_print_short(ImlOperand *self, FILE *out, guint indention)
{
    assert(IML_IS_CONSTANT(self));

    ImlConstant *constant = IML_CONSTANT(self);

    if (constant->datatype == iml_ptr)
    {
        fprintf_indent(out, indention, "'%s'", constant->value);
    }
    else
    {
        fprintf_indent(out, indention, "0x%x", constant->value);
    }
}

static void
iml_constant_class_init(gpointer klass, gpointer foo)
{
    assert(IML_IS_OPERAND_CLASS(klass));

    /* install virtual methods implementation */
    IML_OPERAND_CLASS(klass)->do_get_data_type = iml_constant_do_get_data_type;
    IML_OPERAND_CLASS(klass)->do_print_short = iml_constant_do_print_short;
}
