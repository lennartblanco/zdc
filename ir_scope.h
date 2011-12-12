#ifndef IR_SCOPE_INC_X
#define IR_SCOPE_INC_X

#include <glib-object.h>

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_SCOPE ir_scope_get_type()

#define IR_SCOPE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_SCOPE, IrScope))

#define IR_SCOPE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_SCOPE, IrScopeClass))

#define IR_IS_SCOPE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_SCOPE))

#define IR_IS_SCOPE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_SCOPE))

#define IR_SCOPE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_SCOPE, IrScopeClass))

typedef struct
{
    GObject parent;
    /* private */
    char *mangle_prefix;
} IrScope;

typedef struct
{
    GObjectClass parent_class;
} IrScopeClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_scope_get_type(void);

/**
 * Create new root scope, e.g. module's scope.
 *
 * @param names scope's fully qualified package name, as a list of
 *                     strings (char *) for each package name component
 *
 */
IrScope *
ir_scope_new_root(GSList *names);

/**
 * Create a sub-scrope, e.g. a child scope inside parent scope.
 */
IrScope *
ir_scope_new_sub(const char *name, IrScope *parent);

char *
ir_scope_get_mangle_prefix(IrScope *self);

#endif /* IR_SCOPE_INC_X */
