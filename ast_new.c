#include "ast_new.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_new_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (AstNewClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstNew),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(AST_TYPE_EXPRESSION,
                                    "AstNewType",
                                    &info, 0);
    }
    return type;
}

AstNew *
ast_new_new(DtDataType *type, guint line_number)
{
    assert(DT_IS_DATA_TYPE(type));

    AstNew *obj;

    obj = g_object_new(AST_TYPE_NEW,
                       "ast-node-line-number", line_number,
                       NULL);

    obj->type = type;

    return obj;
}

DtDataType *
ast_new_get_dt_type(AstNew *self)
{
    assert(AST_IS_NEW(self));

    return self->type;
}
