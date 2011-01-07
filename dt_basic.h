#ifndef DT_BASIC_INC_X
#define DT_BASIC_INC_X

#include "dt_data_type.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define DT_TYPE_BASIC dt_basic_get_type()

#define DT_BASIC(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DT_TYPE_BASIC, DtBasic))

#define DT_BASIC_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DT_TYPE_BASIC, DtBasicClass))

#define DT_IS_BASIC(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DT_TYPE_BASIC))

#define DT_IS_BASIC_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DT_TYPE_BASIC))

#define DT_BASIC_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DT_TYPE_BASIC, DtBasicClass))

typedef struct
{
    DtDataType parent;
    /* private */
    basic_data_type_t data_type;
} DtBasic;

typedef struct
{
    DtDataTypeClass parent_class;
} DtBasicClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_basic_get_type(void);

DtBasic *
dt_basic_new(basic_data_type_t data_type);

basic_data_type_t
dt_basic_get_data_type(DtBasic *self);

bool
dt_basic_is_signed(DtBasic *self);

#endif /* DT_BASIC_INC_X */
