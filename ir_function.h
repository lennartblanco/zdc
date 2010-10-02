#ifndef IR_FUNCTION_INC_X
#define IR_FUNCTION_INC_X

#include "ir_symbol.h"
#include "ir_code_block.h"
#include "dt_data_type.h"
#include "utils.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_FUNCTION ir_function_get_type()

#define IR_FUNCTION(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_FUNCTION, IrFunction))

#define IR_FUNCTION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_FUNCTION, IrFunctionClass))

#define IR_IS_FUNCTION(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_FUNCTION))

#define IR_IS_FUNCTION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_FUNCTION))

#define IR_FUNCTION_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_FUNCTION, IrFunctionClass))

typedef enum ir_linkage_type_e
{
  ir_d_linkage,
  ir_c_linkage
} ir_linkage_type_t;

typedef struct
{
    IrSymbol          parent;
    /* private */
    GSList            *parameters;
    DtDataType        *return_type;
    ir_linkage_type_t linkage_type;
    char *mangled_name;
} IrFunction;

typedef struct
{
  IrSymbolClass parent_class;
} IrFunctionClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_function_get_type(void);

void
ir_function_set_parameters(IrFunction *self, GSList *parameters);

/**
 * Get the function formal parameters.
 *
 * @return formal parameters as a list of IrVariable objects
 */
GSList *
ir_function_get_parameters(IrFunction *self);

void
ir_function_set_return_type(IrFunction *self, DtDataType *return_type);

DtDataType *
ir_function_get_return_type(IrFunction *self);

char *
ir_function_get_name(IrFunction *self);

char *
ir_function_get_mangled_name(IrFunction *self);

ir_linkage_type_t
ir_function_get_linkage(IrFunction *self);

void
ir_function_print(IrFunction *self, FILE *out, int indention);

#endif /* IR_FUNCTION_INC_X */
