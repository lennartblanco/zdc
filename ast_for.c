#include "ast_for.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_for_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (AstForClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstFor),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(AST_TYPE_STATMENT,
                                    "AstForType",
                                    &info, 0);
    }
    return type;
}

AstFor *
ast_for_new(AstCodeBlock *init,
            AstExpression *test,
            AstExpression *step,
            AstCodeBlock *body)
{
    assert(AST_IS_CODE_BLOCK(init));
    assert(AST_IS_EXPRESSION(test) || test == NULL);
    assert(AST_IS_EXPRESSION(step) || step == NULL);
    assert(AST_IS_CODE_BLOCK(body));

    AstFor *obj;

    obj = g_object_new(AST_TYPE_FOR, NULL);
    obj->init = init;
    obj->test = test;
    obj->step = step;
    obj->body = body;

    return obj;
}

AstCodeBlock *
ast_for_get_init(AstFor *self)
{
    assert(AST_IS_FOR(self));

    return self->init;
}

AstExpression *
ast_for_get_test(AstFor *self)
{
    assert(AST_IS_FOR(self));

    return self->test;
}

AstExpression *
ast_for_get_step(AstFor *self)
{
    assert(AST_IS_FOR(self));

    return self->step;
}

AstCodeBlock *
ast_for_get_body(AstFor *self)
{
    assert(AST_IS_FOR(self));

    return self->body;
}
