#include "ast_struct.h"

#include <assert.h>

///*---------------------------------------------------------------------------*
// *                  local functions forward declaration                      *
// *---------------------------------------------------------------------------*/
//
//static void
//ast_enum_do_print(AstNode *self, FILE *out, int indention);
//
//static void
//ast_enum_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ast_struct_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (AstStructClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstStruct),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(AST_TYPE_USER_TYPE,
                                    "AstStructType",
                                    &info, 0);
    }
    return type;
}

AstStruct *
ast_struct_new(gchar *name, GSList *members, guint line_number)
{
    AstStruct *obj;

    obj = g_object_new(AST_TYPE_STRUCT,
                       "ast-node-line-number", line_number,
                       NULL);

    obj->name = g_strdup(name);
    obj->members = members;

    return obj;
}

gchar *
ast_struct_get_name(AstStruct *self)
{
    assert(AST_IS_STRUCT(self));

    return self->name;
}

GSList *
ast_struct_get_members(AstStruct *self)
{
    assert(AST_IS_STRUCT(self));

    return self->members;
}

///*---------------------------------------------------------------------------*
// *                             local functions                               *
// *---------------------------------------------------------------------------*/
//
//static void
//ast_enum_do_print(AstNode *self, FILE *out, int indention)
//{
//    assert(AST_IS_ENUM(self));
//    assert(out);
//
//    AstEnum *e = AST_ENUM(self);
//    GSList *i;
//
//    fprintf(out, "  enum\n");
//    fprintf(out, "    tag: %s\n",
//            e->tag ? e->tag : "<anonymous>");
//    fprintf(out,
//            "    base type: %s\n"
//            "    members:\n",
//            e->base_type ? dt_data_type_get_string(e->base_type) :
//                           "<unspecified>");
//
//    for (i = e->members; i != NULL; i = g_slist_next(i))
//    {
//        fprintf(out, "      ");
//        ast_node_print(i->data, out, indention);
//    }
//}
//
//static void
//ast_enum_class_init(gpointer klass, gpointer dummy)
//{
//    ((AstNodeClass *)klass)->do_print = ast_enum_do_print;
//}

