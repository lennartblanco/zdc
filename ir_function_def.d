public import GSList;
public import dt_data_type;
public import iml_func_frame;

struct IrFunctionDef;

extern (C)
{
    char *
    ir_function_def_get_name(IrFunctionDef *self);

    GSList *
    ir_function_def_get_parameters(IrFunctionDef *self);

    DtDataType *
    ir_function_def_get_return_type(IrFunctionDef *self);

    iml_func_frame_t *
    ir_function_def_get_frame(IrFunctionDef *self);

    GSList *
    ir_function_def_get_operations(IrFunctionDef *self);
}

