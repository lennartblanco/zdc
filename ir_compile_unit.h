#ifndef IR_COMPILE_UNIT_INC_X
#define IR_COMPILE_UNIT_INC_X

#include <glib-object.h>

#include "sym_table.h"
#include "ir_function.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_COMPILE_UNIT ir_compile_unit_get_type()

#define IR_COMPILE_UNIT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_COMPILE_UNIT, IrCompileUnit))

#define IR_COMPILE_UNIT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_COMPILE_UNIT, IrCompileUnitClass))

#define IR_IS_COMPILE_UNIT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_COMPILE_UNIT))

#define IR_IS_COMPILE_UNIT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_COMPILE_UNIT))

#define IR_COMPILE_UNIT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_COMPILE_UNIT, IrCompileUnitClass))

typedef struct 
{
  GObject        parent;
  /* private */
  sym_table_t    *symbols;
} IrCompileUnit;

typedef struct 
{
  GObjectClass parent_class;
} IrCompileUnitClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ir_compile_unit_get_type(void);

IrCompileUnit *
ir_compile_unit_new();

sym_table_t *
ir_compile_unit_get_symbols(IrCompileUnit *self);

void
ir_compile_unit_add_function(IrCompileUnit *self, IrFunction *function);

void
ir_compile_unit_print(IrCompileUnit *self, FILE *out, int indention);

#endif /* IR_COMPILE_UNIT_INC_X */
