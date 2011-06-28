public import dt_data_type;

struct DtArray;

extern (C)
{
    DtDataType *
    dt_array_get_data_type(DtArray *self);
}