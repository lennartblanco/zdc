#ifndef DT_RECORD_INC_X
#define DT_RECORD_INC_X

/**
 * Abstract base class for compound data types. User defined types which
 * contains other types (members) and functions to operate on the type
 * (methods). For example struct and class types.
 */

#include "dt_user.h"
#include "ir_struct_member.h"
#include "ir_ident.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define DT_TYPE_RECORD dt_record_get_type()

#define DT_RECORD(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DT_TYPE_RECORD, DtRecord))

#define DT_RECORD_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DT_TYPE_RECORD, DtRecordClass))

#define DT_IS_RECORD(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DT_TYPE_RECORD))

#define DT_IS_RECORD_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DT_TYPE_RECORD))

#define DT_RECORD_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DT_TYPE_RECORD, DtRecordClass))

typedef struct
{
    DtUser parent;

    /* private */
    guint size;
    GSList *members;             /* members in order */
    GHashTable *members_table;   /* members by name */
} DtRecord;

typedef struct
{
    DtUserClass parent_class;
} DtRecordClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_record_get_type(void);

GSList *
dt_record_get_members(DtRecord *self);

void
dt_record_set_members(DtRecord *self, GSList *members);

const IrStructMember *
dt_record_get_member(DtRecord *self, IrIdent *name);

#endif /* DT_RECORD_INC_X */
