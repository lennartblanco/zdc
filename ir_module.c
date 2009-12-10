#include "ir_module.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType ir_module_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (IrModuleClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrModule),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(G_TYPE_OBJECT,
                                    "IrModuleType",
                                    &info, 0);
    }
    return type;
}

IrModule *
ir_module_new()
{
    IrModule *obj;

    obj = g_object_new(IR_TYPE_MODULE, NULL);
    obj->symbols = sym_table_new(NULL);
    obj->function_defs = NULL;

    return obj;
}

sym_table_t *
ir_module_get_symbols(IrModule *self)
{
    assert(IR_IS_MODULE(self));

    return self->symbols;
}

bool
ir_module_add_function_decl(IrModule *self,
                            IrFunctionDecl *function_decl)
{
    assert(IR_IS_MODULE(self));
    assert(IR_IS_FUNCTION_DECL(function_decl));

    if (sym_table_add_symbol(self->symbols, IR_SYMBOL(function_decl)) == -1)
    {
        return false;
    }
    return true;
}

bool
ir_module_add_function_def(IrModule *self,
                           IrFunctionDef *function_def)
{
    assert(IR_IS_MODULE(self));
    assert(IR_IS_FUNCTION_DEF(function_def));

    if (sym_table_add_symbol(self->symbols, IR_SYMBOL(function_def)) == -1)
    {
        return false;
    }

    self->function_defs =
      g_slist_prepend(self->function_defs, function_def);

   return true;
}

GSList *
ir_module_get_function_defs(IrModule *self)
{
    assert(IR_IS_MODULE(self));

    return self->function_defs;
}

void
ir_module_print(IrModule *self, FILE *out, int indention)
{
    assert(IR_IS_MODULE(self));
    assert(out);


    fprintf_indent(out, indention, "compile unit [%p]:\n", self);
    GList *p = sym_table_get_all_symbols(self->symbols);
    for (; p != NULL; p = g_list_next(p))
    {
        ir_symbol_print(IR_SYMBOL(p->data), out, indention + 2);
    }
    g_list_free(p);
}

