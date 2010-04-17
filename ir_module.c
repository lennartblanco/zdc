#include <string.h>

#include "ir_module.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

struct _IrModule
{
  GObject        parent;
  /* private */
  GSList         *package_name;
  sym_table_t    *symbols;
  GSList         *function_defs;
  GSList         *data_section;  /** compile-time constant expressions */
  char *fq_name;
  char *mangled_name;
};

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
ir_module_new(GSList *package_name)
{
    IrModule *obj;

    obj = g_object_new(IR_TYPE_MODULE, NULL);
    obj->symbols = sym_table_new(NULL);
    obj->function_defs = NULL;
    obj->data_section = NULL;
    obj->package_name = package_name;
    obj->fq_name = NULL;
    obj->mangled_name = NULL;

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
      g_slist_append(self->function_defs, function_def);

   return true;
}

GSList *
ir_module_get_function_defs(IrModule *self)
{
    assert(IR_IS_MODULE(self));

    return self->function_defs;
}

char *
ir_module_get_fqname(IrModule *self)
{
    assert(IR_IS_MODULE(self));

    if (self->fq_name == NULL)
    {
        GSList *i;
        GString *str = g_string_new(NULL);

        for (i = self->package_name; i != NULL; i = g_slist_next(i))
        {
           g_string_append_printf(str, "%s%s", 
                                  (char *)i->data,
                                  g_slist_next(i) != NULL ? "." : "");
        }
        self->fq_name = g_string_free(str, FALSE);
    }

    return self->fq_name;
}

char *
ir_module_get_mangled_name(IrModule *self)
{
    assert(IR_IS_MODULE(self));

    if (self->mangled_name == NULL)
    {
        GSList *i;
        GString *str = g_string_new("_D");

        for (i = self->package_name; i != NULL; i = g_slist_next(i))
        {
           g_string_append_printf(str, "%zu%s", 
                                  strlen(i->data), (char *)i->data);
        }
        self->mangled_name = g_string_free(str, FALSE);
    }


    return self->mangled_name;
}

void
ir_module_add_array_literal_data(IrModule *self,
                                 IrArrayLiteral *array_literal)
{
    assert(IR_IS_MODULE(self));
    assert(IR_ARRAY_LITERAL(array_literal));

    self->data_section = g_slist_prepend(self->data_section, array_literal);
}

GSList *
ir_module_get_data_section(IrModule *self)
{
    assert(IR_IS_MODULE(self));

    return self->data_section;
}

void
ir_module_print(IrModule *self, FILE *out, int indention)
{
    assert(IR_IS_MODULE(self));
    assert(out);
    assert(self->package_name);

    GList *p;
    GSList *i;

    fprintf_indent(out, indention, "module [%p]:\n  package: ", self);

    for (i = self->package_name; i != NULL; i = g_slist_next(i))
    {
       fprintf(out, "%s%s",
               (char*)i->data, g_slist_next(i) != NULL ? "." : "\n");
    }

    p = sym_table_get_all_symbols(self->symbols);
    for (; p != NULL; p = g_list_next(p))
    {
        ir_symbol_print(IR_SYMBOL(p->data), out, indention + 2);
    }
    g_list_free(p);
}

