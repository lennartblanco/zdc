#include "dt_array_type.h"
#include "ir_expression.h"

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

IrExpression *
types_arrays_implicit_conv(DtArrayType *target_type,
                           IrExpression *expression);

