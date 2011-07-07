#include "ast_break.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_break_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (AstBreakClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstBreak),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(AST_TYPE_STATMENT,
                                    "AstBreakType",
                                    &info, 0);
    }
    return type;
}

AstBreak *
ast_break_new(guint line_number)
{
    AstBreak *obj;

    obj = g_object_new(AST_TYPE_BREAK,
                       "ast-node-line-number", line_number,
                       NULL);


    return obj;
}
