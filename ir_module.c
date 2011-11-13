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
  GSList         *imports;
  GSList         *enums;
  GSList         *structs;
  GSList         *function_decls;
  GSList         *function_defs;
  guint           label_counter; /** used to generate module unique labels */
  GHashTable     *data_section;  /** compile-time constant expressions */
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
    obj->imports = NULL;
    obj->enums = NULL;
    obj->structs = NULL;
    obj->function_decls = NULL;
    obj->function_defs = NULL;
    obj->label_counter = 0;
    obj->data_section = g_hash_table_new(g_str_hash, g_str_equal);
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

void
ir_module_add_import(IrModule *self, IrModule *import)
{
    assert(IR_IS_MODULE(self));
    assert(IR_IS_MODULE(import));

    sym_table_add_import(self->symbols, ir_module_get_symbols(import));
    self->imports = g_slist_prepend(self->imports, import);
}

GSList *
ir_module_get_imports(IrModule *self)
{
    assert(IR_IS_MODULE(self));

    return self->imports;
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

    self->function_decls = g_slist_append(self->function_decls, function_decl);

    return true;
}

GSList *
ir_module_get_function_decls(IrModule *self)
{
    assert(IR_IS_MODULE(self));

    return self->function_decls;
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

    self->function_defs = g_slist_append(self->function_defs, function_def);

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
        GString *str = g_string_new(NULL);

        for (i = self->package_name; i != NULL; i = g_slist_next(i))
        {
           g_string_append_printf(str, "%zu%s",
                                  strlen(i->data), (char *)i->data);
        }
        self->mangled_name = g_string_free(str, FALSE);
    }


    return self->mangled_name;
}

char *
ir_module_gen_label(IrModule *self)
{
    assert(IR_IS_MODULE(self));
    /* check for label counter overflow */
    assert(self->label_counter <= G_MAXUINT);

    return g_strdup_printf(".IRL%u", (self->label_counter)++);
}

void
ir_module_add_const_data(IrModule *self, IrExpression *const_expr)
{
    assert(IR_IS_MODULE(self));
    assert(IR_IS_EXPRESSION(const_expr));

    char *label;

    if (!IR_IS_LITERAL(const_expr) ||
        !ir_expression_is_constant(const_expr))
    {
        /* only constant array literal expression are stored in data section */
        return;
    }

    IrLiteral *literal = IR_LITERAL(const_expr);

    /* assign data section label if needed */
    if ((label = ir_literal_get_data_label(literal)) == NULL)
    {
        label = ir_module_gen_label(self);
        ir_literal_set_data_label(literal, label);
    }

    /* store in data section list */
    g_hash_table_insert(self->data_section, label, literal);
}

GList *
ir_module_get_data_section(IrModule *self)
{
    assert(IR_IS_MODULE(self));

    return g_hash_table_get_values(self->data_section);
}

bool
ir_module_add_type_alias(IrModule *self,
                         DtAlias *alias)
{
    assert(IR_IS_MODULE(self));
    assert(DT_IS_ALIAS(alias));

    return sym_table_add_symbol(self->symbols, IR_SYMBOL(alias)) == 0;
}

bool
ir_module_add_enum(IrModule *self,
                   DtEnum *dt_enum)
{
    assert(IR_IS_MODULE(self));
    assert(dt_is_enum(dt_enum));

    if (dt_enum_is_anonymous(dt_enum))
    {
        /*
         * for anonymous enum definitions,
         * store enum members in the symbol table
         */
        GSList *i = dt_enum_get_members(dt_enum);

        for (; i != NULL; i = g_slist_next(i))
        {
            if (sym_table_add_symbol(self->symbols, IR_SYMBOL(i->data)) != 0)
            {
                return false;
            }
        }
    }
    else
    {
        /*
         * non-anonymous enum definition, store it in symbols table
         */
        if (sym_table_add_symbol(self->symbols,
                                 IR_SYMBOL(dt_enum)) != 0)
        {
            return false;
        }
    }
    /* add it to the module's enum's list */
    self->enums = g_slist_prepend(self->enums, dt_enum);

    return true;
}

/**
 * @return enum declaration in this module, as a list of IrEnum objects
 */
GSList *
ir_module_get_enums(IrModule *self)
{
    assert(IR_IS_MODULE(self));

    return self->enums;
}

bool
ir_module_add_struct(IrModule *self, IrStruct *ir_struct)
{
    assert(IR_IS_MODULE(self));
    assert(IR_IS_STRUCT(ir_struct));

    /*
     * store struct in symbols table
     */
    if (sym_table_add_symbol(self->symbols,
                             IR_SYMBOL(ir_struct_get_data_type(ir_struct)))!=0)
    {
        return false;
    }

    /* add it to the module's structs list */
    self->structs = g_slist_prepend(self->structs, ir_struct);

    return true;
}

GSList *
ir_module_get_structs(IrModule *self)
{
    assert(IR_IS_MODULE(self));

    return self->structs;
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
        ir_node_print(IR_NODE(p->data), out, indention + 2);
    }
    g_list_free(p);
}
