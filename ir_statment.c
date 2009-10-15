#include "ir_statment.h"
#include "utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_statment_do_print(IrStatment *self, FILE *out, int indention);

static void
ir_statment_class_init(gpointer klass, gpointer foo);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_statment_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (IrStatmentClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_statment_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrStatment),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(G_TYPE_OBJECT,
                                    "IrStatmentType",
                                    &info, 0);
    }
    return type;
}

void
ir_statment_print(IrStatment *self, FILE *out, int indention)
{
    assert(IR_IS_STATMENT(self));
    assert(out);

    IR_STATMENT_GET_CLASS(self)->do_print(self, out, indention);
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ir_statment_do_print(IrStatment *self, FILE *out, int indention)
{
    assert(IR_IS_STATMENT(self));
    assert(out);

    fprintf_indent(out, indention, "%s statment [%p]\n", 
                   g_type_name(G_TYPE_FROM_INSTANCE(self)), self);
}

static void
ir_statment_class_init(gpointer klass, gpointer foo)
{
    ((IrStatmentClass *)klass)->do_print = ir_statment_do_print;
}

