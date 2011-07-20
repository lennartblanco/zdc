public import dt_data_type;

struct DtArray;

extern (C)
{
    DtDataType *
    dt_array_get_element_type(DtArray *self);
}