#ifndef DT_BASIC_TYPE_INC_X
#define DT_BASIC_TYPE_INC_X

#include "ast_data_type.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define DT_TYPE_BASIC_TYPE dt_basic_type_get_type()

#define DT_BASIC_TYPE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DT_TYPE_BASIC_TYPE, DtBasicType))

#define DT_BASIC_TYPE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DT_TYPE_BASIC_TYPE, DtBasicTypeClass))

#define DT_IS_BASIC_TYPE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DT_TYPE_BASIC_TYPE))

#define DT_IS_BASIC_TYPE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DT_TYPE_BASIC_TYPE))

#define DT_BASIC_TYPE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DT_TYPE_BASIC_TYPE, DtBasicTypeClass))

typedef struct
{
    AstDataType parent;
    /* private */
    basic_data_type_t data_type;
} DtBasicType;

typedef struct
{
    AstDataTypeClass parent_class;
} DtBasicTypeClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_basic_type_get_type(void);

DtBasicType * 
dt_basic_type_new(basic_data_type_t data_type);

basic_data_type_t
dt_basic_type_get_data_type(DtBasicType *self);

#endif /* DT_BASIC_TYPE_INC_X */
