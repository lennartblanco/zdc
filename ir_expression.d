public import dt_data_type;

struct IrExpression;

extern (C)
{
    DtDataType *
    ir_expression_get_data_type(IrExpression *self);
}
