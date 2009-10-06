#include "ir_compile_unit.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType ir_compile_unit_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (IrCompileUnitClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrCompileUnit),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(G_TYPE_OBJECT,
                                    "IrCompileUnitType",
                                    &info, 0);
    }
    return type;
}

IrCompileUnit *
ir_compile_unit_new()
{
    IrCompileUnit *obj;

    obj = g_object_new(IR_TYPE_COMPILE_UNIT, NULL);
    obj->symbols = sym_table_new(NULL);
    obj->function_defs = NULL;

    return obj;
}

sym_table_t *
ir_compile_unit_get_symbols(IrCompileUnit *self)
{
    assert(self);
    assert(IR_IS_COMPILE_UNIT(self));

    return self->symbols;
}

bool
ir_compile_unit_add_function_decl(IrCompileUnit *self,
                                  IrFunctionDecl *function_decl)
{
    assert(IR_IS_COMPILE_UNIT(self));
    assert(IR_IS_FUNCTION_DECL(function_decl));

    if (sym_table_add_symbol(self->symbols, IR_SYMBOL(function_decl)) == -1)
    {
        return false;
    }
    return true;
}

bool
ir_compile_unit_add_function_def(IrCompileUnit *self,
                                 IrFunctionDef *function_def)
{
    assert(IR_IS_COMPILE_UNIT(self));
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
ir_compile_unit_get_function_defs(IrCompileUnit *self)
{
    assert(IR_IS_COMPILE_UNIT(self));

    return self->function_defs;
}

void
ir_compile_unit_print(IrCompileUnit *self, FILE *out, int indention)
{
    assert(self);
    assert(IR_IS_COMPILE_UNIT(self));
    assert(out);


    fprintf_indent(out, indention, "compile unit [%p]:\n", self);
    GList *p = sym_table_get_all_symbols(self->symbols);
    for (; p != NULL; p = g_list_next(p))
    {
        ir_symbol_print(IR_SYMBOL(p->data), out, indention + 2);
    }
    g_list_free(p);
}

