#ifndef IR_SYMBOL_INC_X
#define IR_SYMBOL_INC_X

#include <glib-object.h>
#include <stdio.h>

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

typedef struct 
{
  GObject parent;
  /* private */
  char    *name;
} IrSymbol;

typedef struct 
{
  GObjectClass parent_class;
  /* public virtual methods */
  void (*do_print) (IrSymbol *self, FILE *out, int indention);
} IrSymbolClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ir_symbol_get_type(void);

char *
ir_symbol_get_name(IrSymbol *self);

void
ir_symbol_print(IrSymbol *self, FILE *out, int indention);

#endif /* IR_SYMBOL_INC_X */
