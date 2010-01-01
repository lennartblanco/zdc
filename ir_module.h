#ifndef IR_MODULE_INC_X
#define IR_MODULE_INC_X

#include <glib-object.h>
#include <stdbool.h>

#include "sym_table.h"
#include "ir_types.h"
#include "ir_function_def.h"
#include "ir_function_decl.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_MODULE ir_module_get_type()

#define IR_MODULE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_MODULE, IrModule))

#define IR_MODULE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_MODULE, IrModuleClass))

#define IR_IS_MODULE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_MODULE))

#define IR_IS_MODULE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_MODULE))

#define IR_MODULE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_MODULE, IrModuleClass))

typedef struct 
{
  GObjectClass parent_class;
} IrModuleClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ir_module_get_type(void);

/**
 * Create a IR representation of a module.
 *
 * @param package_name module's fully qualified package name, as a list of 
 *                     strings (char *) for each package name component
 */
IrModule *
ir_module_new(GSList *package_name);

sym_table_t *
ir_module_get_symbols(IrModule *self);

/**
 * @return true if function declaration was successfully added,
 *         false if function with this name is already declared
 */
bool
ir_module_add_function_decl(IrModule *self,
                            IrFunctionDecl *function_decl);

/**
 * @return true if function definition was successfully added,
 *         false if function with this name is already defined
 */
bool
ir_module_add_function_def(IrModule *self,
                           IrFunctionDef *function_def);

GSList *
ir_module_get_function_defs(IrModule *self);

void
ir_module_print(IrModule *self, FILE *out, int indention);

/**
 * Get module's fully qualified name.
 */
char *
ir_module_get_fqname(IrModule *self);

char *
ir_module_get_mangled_name(IrModule *self);

#endif /* IR_MODULE_INC_X */
