#include "ir_array.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ir_array_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (IrArrayClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrArray),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_LVALUE,
                                    "IrArrayType",
                                    &info, 0);
    }
    return type;
}

IrArray *
ir_array_new(IrVariable *variable)
{
    IrArray *obj;

    obj = g_object_new(IR_TYPE_ARRAY, NULL);

    ir_lvalue_set_variable(IR_LVALUE(obj), variable);    

    return obj;
}

