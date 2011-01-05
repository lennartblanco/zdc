#ifndef IR_ARRAY_SLICE_INC_X
#define IR_ARRAY_SLICE_INC_X

#include "ir_variable.h"
#include "dt_array_type.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_ARRAY_SLICE ir_array_slice_get_type()

#define IR_ARRAY_SLICE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_ARRAY_SLICE, IrArraySlice))

#define IR_ARRAY_SLICE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_ARRAY_SLICE, IrArraySliceClass))

#define IR_IS_ARRAY_SLICE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_ARRAY_SLICE))

#define IR_IS_ARRAY_SLICE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_ARRAY_SLICE))

#define IR_ARRAY_SLICE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_ARRAY_SLICE, IrArraySliceClass))

typedef struct
{
    IrExpression parent;

    /* private */
    DtArrayType *data_type;
    IrExpression *array;
    IrExpression *start;
    IrExpression *end;
} IrArraySlice;

typedef struct
{
    IrExpressionClass parent_class;
} IrArraySliceClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ir_array_slice_get_type(void);

IrArraySlice *
ir_array_slice_new(IrExpression *array,
                   IrExpression *start,
                   IrExpression *end,
                   guint line_number);

/**
 * get the array expression for this slice
 */
IrExpression *
ir_array_slice_get_array(IrArraySlice *self);

/**
 * set the array expression for this slice
 */
void
ir_array_slice_set_array(IrArraySlice *self, IrExpression *array);

IrExpression *
ir_array_slice_get_start(IrArraySlice *self);

IrExpression *
ir_array_slice_get_end(IrArraySlice *self);

void
ir_array_slice_set_start(IrArraySlice *self,
                         IrExpression *start);

void
ir_array_slice_set_end(IrArraySlice *self,
                       IrExpression *end);

#endif /* IR_ARRAY_SLICE_INC_X */
