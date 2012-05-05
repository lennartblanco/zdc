extern (C):

/*---------------------------------------------------------------------------*
 *                         data type declarations                            *
 *---------------------------------------------------------------------------*/

struct DtDataType;

enum basic_data_type
{
    char_type,               /* 8-bit character */
    bool_type,               /* boolean */
    byte_type,               /* int 8-bit signed */
    ubyte_type,              /* int 8-bit unsigned */
    short_type,              /* int 16-bit signed */
    ushort_type,             /* int 16-bit unsigned */
    int_type,                /* int 32-bit signed */
    uint_type                /* int 32-bit unsigned */
}

char *
dt_data_type_get_string(DtDataType *self);

/*---------------------------------------------------------------------------*
 *                       basic data type declarations                        *
 *---------------------------------------------------------------------------*/

struct DtBasic;

bool
dt_is_basic(void *obj);

DtBasic *
dt_basic(void *obj);

basic_data_type
dt_basic_get_data_type(DtBasic *self);

bool
dt_basic_is_int(DtDataType *data_type);

/*---------------------------------------------------------------------------*
 *                       array data type declarations                        *
 *---------------------------------------------------------------------------*/

struct DtArray;

DtArray *
dt_array(void *obj);

DtDataType *
dt_array_get_element_type(DtArray *self);

/*---------------------------------------------------------------------------*
 *                        enum data type declarations                        *
 *---------------------------------------------------------------------------*/

struct DtEnum;

bool
dt_is_enum(void *obj);

DtEnum *
dt_enum(void *obj);

DtDataType *
dt_enum_get_base_type(DtEnum *self);

/*---------------------------------------------------------------------------*
 *                      pointer data type declarations                       *
 *---------------------------------------------------------------------------*/

struct DtPointer;

bool
dt_is_pointer(void *obj);