#include <stdbool.h>
#include <string.h>

#include "ir_array_cell_ref.h"
#include "dt_basic_type.h"
#include "dt_static_array_type.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_array_cell_ref_class_init(gpointer klass, gpointer dummy);

static DtDataType *
ir_array_cell_ref_do_get_data_type(IrExpression *self);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ir_array_cell_ref_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (IrArrayCellRefClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_array_cell_ref_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrArrayCellRef),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_LVALUE,
                                    "IrArrayCellRefType",
                                    &info, 0);
    }
    return type;
}

IrArrayCellRef *
ir_array_cell_ref_new(char *array_name,
                      IrExpression *index,
                      guint line_number)
{
    assert(array_name);
    assert(IR_IS_EXPRESSION(index));

    IrArrayCellRef *obj;

assert(line_number > 0);

    obj = g_object_new(IR_TYPE_ARRAY_CELL_REF,
                       "ir-node-line-number", line_number,
                       "ir-lvalue-symbol-name", array_name,
                       NULL);

    obj->index = index;
    obj->data_type = NULL;

    return obj;
}

char *
ir_array_cell_get_name(IrArrayCellRef *self)
{
    assert(IR_IS_ARRAY_CELL_REF(self));

    return ir_lvalue_get_name(IR_LVALUE(self));
}

IrExpression *
ir_array_cell_ref_get_index(IrArrayCellRef *self)
{
    assert(IR_IS_ARRAY_CELL_REF(self));

    return self->index;
}

void
ir_array_cell_ref_set_index(IrArrayCellRef *self, IrExpression *index)
{
    assert(IR_IS_ARRAY_CELL_REF(self));
    assert(IR_IS_EXPRESSION(index));

    self->index = index;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ir_array_cell_ref_class_init(gpointer klass, gpointer dummy)
{
    ((IrExpressionClass *)klass)->do_get_data_type =
        ir_array_cell_ref_do_get_data_type;
}

static DtDataType *
ir_array_cell_ref_do_get_data_type(IrExpression *self)
{
    assert(IR_IS_ARRAY_CELL_REF(self));

    IrArrayCellRef *cell = IR_ARRAY_CELL_REF(self);

    if (cell->data_type == NULL)
    {
        IrVariable *array_symbol;
        DtStaticArrayType *array_data_type;
        basic_data_type_t array_basic_type;

        array_symbol = ir_lvalue_get_variable(IR_LVALUE(cell));
        array_data_type =
            DT_STATIC_ARRAY_TYPE(ir_variable_get_data_type(array_symbol));
        array_basic_type = dt_static_array_type_get_data_type(array_data_type);

        cell->data_type = DT_DATA_TYPE(dt_basic_type_new(array_basic_type));
    }
 
    return cell->data_type;
}
