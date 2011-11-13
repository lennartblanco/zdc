#ifndef IR_SYMBOL_INC_X
#define IR_SYMBOL_INC_X

#include <glib-object.h>
#include <stdio.h>

#include "ir_types.h"
#include "ir_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_SYMBOL ir_symbol_get_type()

#define IR_SYMBOL(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_SYMBOL, IrSymbol))

#define IR_SYMBOL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_SYMBOL, IrSymbolClass))

#define IR_IS_SYMBOL(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_SYMBOL))

#define IR_IS_SYMBOL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_SYMBOL))

#define IR_SYMBOL_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_SYMBOL, IrSymbolClass))

struct _IrSymbol
{
  IrExpression parent;
  /* private */
  char    *name;
  char    *fq_name;
  IrModule *parent_module;
};

typedef struct
{
  IrExpressionClass parent_class;
} IrSymbolClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ir_symbol_get_type(void);

/**
 * Get symbol's local name
 */
char *
ir_symbol_get_name(IrSymbol *self);

/**
 * Get symbol's fully qualified name
 */
char *
ir_symbol_get_fqname(IrSymbol *self);

IrModule *
ir_symbol_get_parent_module(IrSymbol *self);

#endif /* IR_SYMBOL_INC_X */
