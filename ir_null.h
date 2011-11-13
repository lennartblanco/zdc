#ifndef IR_NULL_INC_X
#define IR_NULL_INC_X

#include "ir_constant.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_NULL ir_null_get_type()

#define IR_NULL(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_NULL, IrNull))

#define IR_NULL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_NULL, IrNullClass))

#define IR_IS_NULL(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_NULL))

#define IR_IS_NULL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_NULL))

#define IR_NULL_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_NULL, IrNullClass))

typedef struct
{
    IrConstant parent;
} IrNull;

typedef struct
{
    IrConstantClass parent_class;
} IrNullClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_null_get_type(void);

IrNull *
ir_null_new();

#endif /* IR_NULL_INC_X */
