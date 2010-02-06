#ifndef DT_DATA_TYPE_INC_X
#define DT_DATA_TYPE_INC_X

#include <glib-object.h>
#include <stdio.h>

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

typedef struct
{
    GObject parent;
} DtDataType;

typedef struct
{
    GObjectClass parent_class;

    /* public virtual methods */
    void (*do_print) (DtDataType *self, FILE *out);
    char * (*get_mangled) (DtDataType *self);
    guint (*get_size) (DtDataType *self);
} DtDataTypeClass;

typedef enum basic_data_type_e
{
    void_type, 
    bool_type,               /* boolean */
    int_type,                /* int 32-bit signed */
    uint_type                /* int 32-bit unsigned */
} basic_data_type_t;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_data_type_get_type(void);

void
dt_data_type_print(DtDataType *self, FILE *out);

/**
 * @return datatype's size in bytes
 */
guint
dt_data_type_get_size(DtDataType *self);

/**
 * Return the mangle string representation of this data type.
 */
char *
dt_data_type_get_mangled(DtDataType *self);

#endif /* DT_DATA_TYPE_INC_X */
