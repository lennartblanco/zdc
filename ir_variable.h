#ifndef IR_VARIABLE_INC_X
#define IR_VARIABLE_INC_X

#include "ir_symbol.h"
#include "ir_expression.h"
#include "dt_data_type.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_VARIABLE ir_variable_get_type()

#define IR_VARIABLE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_VARIABLE, IrVariable))

#define IR_VARIABLE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_VARIABLE, IrVariableClass))

#define IR_IS_VARIABLE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_VARIABLE))

#define IR_IS_VARIABLE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_VARIABLE))

#define IR_VARIABLE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_VARIABLE, IrVariableClass))

typedef struct
{
    IrSymbol       parent;

    /*
     * private 
     */

    /* variables data type */
    DtDataType    *type;
    /* value to assign to variable when created */
    IrExpression  *initializer;
    /* backend specific location of this variable */
    GObject       *location;
} IrVariable;

typedef struct
{
  IrSymbolClass parent_class;
} IrVariableClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_variable_get_type(void);

/**
 * Create new IR variable object.
 *
 * @param type variable's data type
 * @param name variable's name
 * @param initializer inital value to assign to variable or NULL
 *        to assign default type value
 */
IrVariable *
ir_variable_new(DtDataType *type, 
                char *name,
                IrExpression *initializer,
                guint line_number);

void
ir_variable_set_location(IrVariable *self, GObject *location);

GObject *
ir_variable_get_location(IrVariable *self);

char *
ir_variable_get_name(IrVariable *self);

DtDataType *
ir_variable_get_data_type(IrVariable *self);

/**
 * Modify variables data type
 */
void
ir_variable_set_data_type(IrVariable *self, DtDataType *type);

IrExpression *
ir_variable_get_initializer(IrVariable *self);

#endif /* IR_VARIABLE_INC_X */
