#ifndef DT_ALIAS_INC_X
#define DT_ALIAS_INC_X

#include "dt_user.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define DT_TYPE_ALIAS dt_alias_get_type()

#define DT_ALIAS(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DT_TYPE_ALIAS, DtAlias))

#define DT_ALIAS_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DT_TYPE_ALIAS, DtAliasClass))

#define DT_IS_ALIAS(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DT_TYPE_ALIAS))

#define DT_IS_ALIAS_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DT_TYPE_ALIAS))

#define DT_ALIAS_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DT_TYPE_ALIAS, DtAliasClass))

typedef struct
{
    DtUser parent;

    /* private */
    DtDataType *aliased_type;
} DtAlias;

typedef struct
{
    DtUserClass parent_class;
} DtAliasClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_alias_get_type(void);

DtAlias *
dt_alias_new(DtDataType *aliased_type,
             gchar *name);

const char *
dt_alias_get_name(DtAlias *self);

DtDataType *
dt_alias_get_aliased_type(DtAlias *self);

#endif /* DT_ALIAS_INC_X */
