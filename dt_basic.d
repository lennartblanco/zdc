public import dt_data_type;

struct DtBasic;

extern (C)
{
    bool
    dt_is_basic(void *obj);

    basic_data_type
    dt_basic_get_data_type(DtBasic *self);
}