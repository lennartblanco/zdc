#ifndef IR_SYMBOL_INC_X
#define IR_SYMBOL_INC_X

#include <glib-object.h>
#include <stdio.h>

#include "ir_types.h"
#include "ir_expression.h"
#include "ir_scope.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_SYMBOL ir_symbol_get_type()

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
    IrScope  *scope;
    bool     is_private;
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

IrSymbol *
ir_symbol(void *obj);

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

IrScope *
ir_symbol_get_scope(IrSymbol *self);

bool
ir_symbol_is_private(IrSymbol *self);

#endif /* IR_SYMBOL_INC_X */
