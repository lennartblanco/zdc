#include "iml_operand.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
iml_operand_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (ImlOperandClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (ImlOperand),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(G_TYPE_OBJECT,
                                    "ImlOperandType",
                                    &info, 0);
    }
    return type;
}

ImlOperand *
iml_operand(void *obj)
{
    return G_TYPE_CHECK_INSTANCE_CAST((obj), IML_TYPE_OPERAND, ImlOperand);
}

iml_data_type_t
iml_operand_get_data_type(ImlOperand *self)
{
    assert(IML_IS_OPERAND(self));
    assert(IML_OPERAND_GET_CLASS(self)->do_get_data_type);

    return IML_OPERAND_GET_CLASS(self)->do_get_data_type(self);
}

void
iml_operand_print(ImlOperand *self, FILE *out, guint indention)
{
    assert(IML_IS_OPERAND(self));
    assert(IML_OPERAND_GET_CLASS(self)->do_print);

    IML_OPERAND_GET_CLASS(self)->do_print(self, out, indention);
}

void
iml_operand_print_short(ImlOperand *self, FILE *out, guint indention)
{
    assert(IML_IS_OPERAND(self));
    assert(IML_OPERAND_GET_CLASS(self)->do_print_short);

    IML_OPERAND_GET_CLASS(self)->do_print_short(self, out, indention);
}

