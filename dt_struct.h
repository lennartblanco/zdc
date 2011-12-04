#ifndef DT_STRUCT_INC_X
#define DT_STRUCT_INC_X

#include "dt_user.h"
#include "ir_ident.h"
#include "ir_struct_literal.h"
#include "ir_struct_member.h"
#include "ir_variable.h"
#include "ir_function_call.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define DT_TYPE_STRUCT dt_struct_get_type()

#define DT_STRUCT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DT_TYPE_STRUCT, DtStruct))

#define DT_STRUCT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DT_TYPE_STRUCT, DtStructClass))

#define DT_IS_STRUCT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DT_TYPE_STRUCT))

#define DT_IS_STRUCT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DT_TYPE_STRUCT))

#define DT_STRUCT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DT_TYPE_STRUCT, DtStructClass))

typedef struct
{
    DtUser parent;

    /* private */
    IrStructLiteral *init;
    guint size;
    GSList *members;             /* members in order */
    GHashTable *members_table;   /* members by name */
    GHashTable *methods;         /* methods by name */
} DtStruct;

typedef struct
{
    DtUserClass parent_class;
} DtStructClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_struct_get_type(void);

DtStruct *
dt_struct_new(gchar *name, IrModule *parent_module);

void
dt_struct_add_member(DtStruct *self, IrVariable *var);

/**
 * Look-up struct member by name.
 *
 * @return new constructed IrStructMember object representing the member or
 *         NULL of no member with specified name is found
 */
IrStructMember *
dt_struct_get_member(DtStruct *self, IrIdent *name);

void
dt_struct_add_method(DtStruct *self, IrFunctionDef *method);

IrFunctionDef *
dt_struct_get_method(DtStruct *self, const char *method_name);

#endif /* DT_STRUCT_INC_X */
