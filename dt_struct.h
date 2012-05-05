#ifndef DT_STRUCT_INC_X
#define DT_STRUCT_INC_X

#include "dt_record.h"
#include "ir_struct_literal.h"
#include "sym_table.h"

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
    DtRecord parent;

    /* private */
    bool opaque;
    IrStructLiteral *init;
    GSList *methods;             /* methods in order */
    GHashTable *methods_table;   /* methods by name */
} DtStruct;

typedef struct
{
    DtRecordClass parent_class;
} DtStructClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_struct_get_type(void);

DtStruct *
dt_struct_new(gchar *name,
              GSList *members,
              GSList *methods,
              bool opaque,
              IrModule *parent_module);

/**
 * Convenience wrapper around dt_record_get_members().
 */
GSList *
dt_struct_get_members(DtStruct *self);

void
dt_struct_set_members(DtStruct *self, GSList *members);

GSList *
dt_struct_get_methods(DtStruct *self);

/**
 * Look-up struct member by name.
 *
 * The returned object is owned by the self object and should not modified.
 *
 * @return IrStructMember object representing the member or NULL of no member
 *          with specified name is found
 */
const IrStructMember *
dt_struct_get_member(DtStruct *self, IrIdent *name);

bool
dt_struct_is_opaque(DtStruct *self);

void
dt_struct_set_method(DtStruct *self, IrFunctionDef *method);

IrFunctionDef *
dt_struct_get_method(DtStruct *self, const char *method_name);

#endif /* DT_STRUCT_INC_X */
