#include <string.h>

#include "ast_attribute.h"

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_attribute_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (AstAttributeClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstAttribute),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(AST_TYPE_NODE,
                                    "AstAttributeType",
                                    &info, 0);
    }
    return type;
}
