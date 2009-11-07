#ifndef IR_ARRAY_SLICE_INC_X
#define IR_ARRAY_SLICE_INC_X

#include "ir_lvalue.h"
#include "ir_variable.h"

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
    IrLvalue parent;

    /* private */
    IrExpression *start;
    IrExpression *end;
    DtDataType   *data_type;
} IrArraySlice;

typedef struct
{
    IrLvalueClass parent_class;
} IrArraySliceClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ir_array_slice_get_type(void);

IrArraySlice *
ir_array_slice_new(char *array_name,
                   IrExpression *start,
                   IrExpression *end,
                   guint line_number);

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
