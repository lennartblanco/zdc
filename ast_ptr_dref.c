#include "ast_ptr_dref.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_ptr_dref_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (AstPtrDrefClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstPtrDref),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(AST_TYPE_EXPRESSION,
                                    "AstPtrDrefType",
                                    &info, 0);
    }
    return type;
}

AstPtrDref *
ast_ptr_dref_new(AstExpression *ptr_expression)
{
    AstPtrDref *obj;

    obj = g_object_new(AST_TYPE_PTR_DREF, NULL);
    obj->ptr_expression = ptr_expression;

    return obj;
}

AstExpression *
ast_ptr_dref_get_ptr_expression(AstPtrDref *self)
{
    assert(AST_IS_PTR_DREF(self));

    return self->ptr_expression;
}
