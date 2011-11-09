public import dt_data_type;

struct DtEnum;

extern (C)
{
    bool
    dt_is_enum(void *obj);

    DtDataType *
    dt_enum_get_base_type(DtEnum *self);
}
