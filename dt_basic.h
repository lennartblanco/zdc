#ifndef DT_BASIC_INC_X
#define DT_BASIC_INC_X

#include "dt_data_type.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define DT_TYPE_BASIC dt_basic_get_type()

#define DT_BASIC_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DT_TYPE_BASIC, DtBasicClass))

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

bool
dt_is_basic(void *obj);

DtBasic *
dt_basic(void *obj);

DtBasic *
dt_basic_new(basic_data_type_t data_type);

basic_data_type_t
dt_basic_get_data_type(DtBasic *self);

bool
dt_basic_is_signed(DtBasic *self);

UtRange *
dt_basic_get_value_range(DtBasic *self);

/**
 * Returns true if data_type represent basic type void,
 * false otherwise.
 */
bool
dt_basic_is_void(DtDataType *data_type);

/**
 * Returns true if data_type represent basic type bool,
 * false otherwise.
 */
bool
dt_basic_is_bool(DtDataType *data_type);

/**
 * Returns true if data_type represent basic type char,
 * false otherwise.
 */
bool
dt_basic_is_char(DtDataType *data_type);


/**
 * Returns true if data_type represent basic type int,
 * false otherwise.
 */
bool
dt_basic_is_int(DtDataType *data_type);

/**
 * Returns true if data_type represent basic type uint,
 * false otherwise.
 */
bool
dt_basic_is_uint(DtDataType *data_type);

/**
 * Returns true if data_type represent basic type short,
 * false otherwise.
 */
bool
dt_basic_is_short(DtDataType *data_type);

/**
 * Returns true if data_type represent basic type ushort,
 * false otherwise.
 */
bool
dt_basic_is_ushort(DtDataType *data_type);

/**
 * Returns true if data_type represent basic type byte,
 * false otherwise.
 */
bool
dt_basic_is_byte(DtDataType *data_type);

/**
 * Returns true if data_type represent basic type ubyte,
 * false otherwise.
 */
bool
dt_basic_is_ubyte(DtDataType *data_type);

#endif /* DT_BASIC_INC_X */
