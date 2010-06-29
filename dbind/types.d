module dbind.types;

public import dbind.dt_data_type;

extern (C)
{
  bool
  types_is_int(DtDataType *data_type);
}
