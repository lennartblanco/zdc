#include "ast_null.h"

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_null_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (AstNullClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstNull),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(AST_TYPE_EXPRESSION,
                                    "AstNullType",
                                    &info, 0);
    }
    return type;
}

AstNull *
ast_null_new(guint line_number)
{
    return g_object_new(AST_TYPE_NULL,
                        "ast-node-line-number", line_number,
                        NULL);
}
