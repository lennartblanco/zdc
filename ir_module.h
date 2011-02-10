#ifndef IR_MODULE_INC_X
#define IR_MODULE_INC_X

#include <glib-object.h>
#include <stdbool.h>

#include "sym_table.h"
#include "dt_user.h"
#include "ir_types.h"
#include "ir_function_def.h"
#include "ir_function_decl.h"
#include "ir_array_literal.h"
#include "ir_enum.h"
#include "ir_struct.h"

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

/**
 * Store a compile-time constant array literal in this module's data section.
 *
 * If const_expr is not an array literal or is not constant, nothing will be
 * added. If an array literal have same data section label as previously stored
 * array literal, the new expression will replace previously stored.
 */
void
ir_module_add_const_data(IrModule *self, IrExpression *const_expr);

/**
 * Get all expression in this modules data section.
 *
 * Returned list must be g_list_free():ed by the caller when no longer used.
 */
GList *
ir_module_get_data_section(IrModule *self);

/**
 * @return false if this alias definition conflicts with some other user type
 */
bool
ir_module_add_type_alias(IrModule *self,
                         DtDataType *data_type,
                         gchar *alias_name);

/**
 * @return false if this enum definition conflicts with some other user type
 */
bool
ir_module_add_enum(IrModule *self,
                   IrEnum *ir_enum);

/**
 * @return enum declaration in this module, as a list of IrEnum objects
 */
GSList *
ir_module_get_enums(IrModule *self);

/**
 * @return false if this struct definition conflicts with some other user type
 */
bool
ir_module_add_struct(IrModule *self, IrStruct *ir_struct);

/**
 * Look-up the user defined type by it's name.
 *
 * @return the data type or NULL if no user type with provided name is defined.
 */
DtDataType *
ir_module_get_user_type(IrModule *self,
                        DtUser *user_type);
void
ir_module_print(IrModule *self, FILE *out, int indention);

/**
 * Get module's fully qualified name.
 */
char *
ir_module_get_fqname(IrModule *self);

char *
ir_module_get_mangled_name(IrModule *self);

/**
 * Generate a unique for this module label.
 */
char *
ir_module_gen_label(IrModule *self);

#endif /* IR_MODULE_INC_X */
