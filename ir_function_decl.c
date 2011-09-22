#include "ir_function_decl.h"

#include <assert.h>


/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_function_decl_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info =
      {
        sizeof (IrFunctionDeclClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrFunctionDecl),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_FUNCTION,
                                    "IrFunctionDeclType",
                                    &info, 0);
    }
    return type;
}

IrFunctionDecl *
ir_function_decl_new(DtDataType *return_type,
                     char *name,
                     GSList *parameters,
                     IrModule *parent_module,
                     ir_linkage_type_t linkage_type,
                     guint line_number)
{
    assert(DT_IS_DATA_TYPE(return_type));
    assert(name);

    IrFunctionDecl *obj;

    obj = g_object_new(IR_TYPE_FUNCTION_DECL,
                       "ir-symbol-name", name,
                       "ir-function-linkage-type", linkage_type,
                       "ir-node-line-number", line_number,
                       "ir-symbol-parent-module", parent_module,
                       NULL);
   
    ir_function_set_return_type(IR_FUNCTION(obj), return_type);
    ir_function_set_parameters(IR_FUNCTION(obj), parameters);
    
    return obj;
}

