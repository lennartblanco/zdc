#include <string.h>

#include "ast_array_cell_ref.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_array_cell_ref_do_print(AstNode *self, FILE *out);

static void
ast_array_cell_ref_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_array_cell_ref_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (AstArrayCellRefClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ast_array_cell_ref_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstArrayCellRef),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(XDP_TYPE_AST_VARIABLE_REF,
                                    "AstArrayCellRefType",
                                    &info, 0);
    }
    return type;
}

AstArrayCellRef *
ast_array_cell_ref_new(char *name, int index)
{
    AstArrayCellRef *obj;

    obj = g_object_new(XDP_TYPE_AST_ARRAY_CELL_REF, NULL);
    obj->name = strdup(name);
    obj->index = index;

    return obj;
}

char *
ast_array_cell_get_name(AstArrayCellRef *self)
{
    assert(self);
    assert(XDP_IS_AST_ARRAY_CELL_REF(self));

    return self->name;
}


int
ast_array_cell_get_index(AstArrayCellRef *self)
{
    assert(self);
    assert(XDP_IS_AST_ARRAY_CELL_REF(self));

    return self->index;
}


/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_array_cell_ref_do_print(AstNode *self, FILE *out)
{
    assert(self);
    assert(XDP_IS_AST_ARRAY_CELL_REF(self));
    assert(out);

    AstArrayCellRef *ref = XDP_AST_ARRAY_CELL_REF(self);
    fprintf(out, "%s[%d]", ref->name, ref->index);
}

static void
ast_array_cell_ref_class_init(gpointer klass, gpointer dummy)
{
    ((AstNodeClass *)klass)->do_print = ast_array_cell_ref_do_print;
}

