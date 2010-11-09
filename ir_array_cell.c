#include <stdbool.h>
#include <string.h>

#include "ir_array_cell.h"
#include "dt_basic_type.h"
#include "dt_static_array_type.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_array_cell_class_init(gpointer klass, gpointer dummy);

static DtDataType *
ir_array_cell_do_get_data_type(IrExpression *self);

static bool
ir_array_cell_do_is_lvalue(IrExpression *self);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ir_array_cell_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (IrArrayCellClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_array_cell_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrArrayCell),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_EXPRESSION,
                                    "IrArrayCellType",
                                    &info, 0);
    }
    return type;
}

IrArrayCell *
ir_array_cell_new(IrVariable *array, IrExpression *index, guint line_number)
{
    assert(IR_IS_VARIABLE(array));
    assert(IR_IS_EXPRESSION(index));

    IrArrayCell *obj;



    obj = g_object_new(IR_TYPE_ARRAY_CELL,
                       "ir-node-line-number", line_number,
                       NULL);

    obj->array = array;
    obj->index = index;
    obj->data_type = NULL;

    return obj;
}

IrVariable *
ir_array_cell_get_symbol(IrArrayCell *self)
{
    assert(IR_IS_ARRAY_CELL(self));

    return self->array;
}

IrExpression *
ir_array_cell_get_index(IrArrayCell *self)
{
    assert(IR_IS_ARRAY_CELL(self));

    return self->index;
}

void
ir_array_cell_set_index(IrArrayCell *self, IrExpression *index)
{
    assert(IR_IS_ARRAY_CELL(self));
    assert(IR_IS_EXPRESSION(index));

    self->index = index;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ir_array_cell_class_init(gpointer klass, gpointer dummy)
{
    ((IrExpressionClass *)klass)->do_get_data_type =
        ir_array_cell_do_get_data_type;
    ((IrExpressionClass *)klass)->do_is_lvalue =
        ir_array_cell_do_is_lvalue;
}

static DtDataType *
ir_array_cell_do_get_data_type(IrExpression *self)
{
    assert(IR_IS_ARRAY_CELL(self));

    IrArrayCell *cell = IR_ARRAY_CELL(self);

    if (cell->data_type == NULL)
    {
        DtArrayType *array_data_type;

        array_data_type =
            DT_ARRAY_TYPE(
                ir_expression_get_data_type(IR_EXPRESSION(cell->array)));
        cell->data_type = dt_array_type_get_data_type(array_data_type);
    }

    return cell->data_type;
}

static bool
ir_array_cell_do_is_lvalue(IrExpression *self)
{
    assert(IR_IS_ARRAY_CELL(self));

    return true;
}
