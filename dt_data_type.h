#ifndef DT_DATA_TYPE_INC_X
#define DT_DATA_TYPE_INC_X

#include "ir_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define DT_TYPE_DATA_TYPE dt_data_type_get_type()

#define DT_DATA_TYPE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DT_TYPE_DATA_TYPE, DtDataType))

#define DT_DATA_TYPE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DT_TYPE_DATA_TYPE, DtDataTypeClass))

#define DT_IS_DATA_TYPE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DT_TYPE_DATA_TYPE))

#define DT_IS_DATA_TYPE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DT_TYPE_DATA_TYPE))

#define DT_DATA_TYPE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DT_TYPE_DATA_TYPE, DtDataTypeClass))

struct _DtDataType
{
    GObject parent;

    /* private */
    bool immutable;
    guint line_number;
};

typedef struct
{
    GObjectClass parent_class;

    /* public virtual methods */
    char * (*get_mangled) (DtDataType *self);
    char * (*get_string) (DtDataType *self);
    guint (*get_size) (DtDataType *self);
    IrExpression * (*get_init) (DtDataType *self);
    bool (*is_same) (DtDataType *self, DtDataType *type);
    bool (*is_impl_conv) (DtDataType *self, IrExpression *expression);
    bool (*is_integral) (DtDataType *self);
} DtDataTypeClass;

typedef enum basic_data_type_e
{
    void_type,
    char_type,               /* 8-bit character */
    bool_type,               /* boolean */
    byte_type,               /* int 8-bit signed */
    ubyte_type,              /* int 8-bit unsigned */
    short_type,              /* int 16-bit signed */
    ushort_type,             /* int 16-bit unsigned */
    int_type,                /* int 32-bit signed */
    uint_type,               /* int 32-bit unsigned */
    last_basic_type          /* number of basic types defined */
} basic_data_type_t;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_data_type_get_type(void);

void
dt_data_type_set_immutable(DtDataType *self, bool is_immutable);

/**
 * @return true if this type is declared as immutable
 */
bool
dt_data_type_is_immutalbe(DtDataType *self);

/**
 * @return datatype's size in bytes
 */
guint
dt_data_type_get_size(DtDataType *self);

/**
 * @return the source string representation of this data type.
 */
char *
dt_data_type_get_string(DtDataType *self);

/**
 * Return the mangle string representation of this data type.
 */
char *
dt_data_type_get_mangled(DtDataType *self);

/**
 * Get default initializer expression for this data type.
 */
IrExpression *
dt_data_type_get_init(DtDataType *self);

/**
 *  @return true if self and type are same data type
 */
bool
dt_data_type_is_same(DtDataType *self, DtDataType *type);

/**
 * @return true if this is an integral type, false otherwise
 */
bool
dt_data_type_is_integral(DtDataType *self);

/**
 * @return true if expression can be implicitly converted to self
 */
bool
dt_data_type_is_impl_conv(DtDataType *self, IrExpression *expression);

guint
dt_data_type_get_line_num(DtDataType *self);

#endif /* DT_DATA_TYPE_INC_X */
