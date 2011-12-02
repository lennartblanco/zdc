#include "ast_cast.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_cast_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (AstCastClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstCast),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(AST_TYPE_EXPRESSION,
                                    "AstCastType",
                                    &info, 0);
    }
    return type;
}

AstCast *
ast_cast_new(DtDataType *target_type, AstExpression *value, guint line_number)
{
    assert(DT_IS_DATA_TYPE(target_type));
    assert(AST_IS_EXPRESSION(value));

    AstCast *obj;

    obj = g_object_new(AST_TYPE_CAST,
                       "ast-node-line-number", line_number,
                       NULL);

    obj->target_type = target_type;
    obj->value = value;

    return obj;
}

DtDataType *
ast_cast_get_target_type(AstCast *self)
{
    assert(AST_IS_CAST(self));

    return self->target_type;
}

AstExpression *
ast_cast_get_value(AstCast *self)
{
    assert(AST_IS_CAST(self));

    return self->value;
}
