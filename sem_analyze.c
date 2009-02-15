#include <stdio.h>
#include <stdbool.h>

#include "sem_analyze.h"
#include "ast_variable_declaration.h"
#include "ast_variable_definition.h"
#include "ast_assigment.h"
#include "ast_basic_type.h"
#include "ast_static_array_type.h"
#include "ast_if_else.h"
#include "ir_if_else.h"
#include "ir_if_block.h"
#include "ir_variable.h"
#include "ir_function.h"
#include "ast_array_slice_ref.h"
#include "utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
sem_analyze_ast_code_block_to_ir(const char *source_file,
                                 AstCodeBlock *ast_code_block,
                                 IrCodeBlock *ir_code_block);


/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

/**
 * convert AST if block to IR form.
 */
static IrIfBlock *
sem_analyze_ast_if_block_to_ir(const char *source_file,
                               IrCodeBlock *parent_block,
                               AstIfBlock *ast_if_block)
{
    IrCodeBlock *body = 
        ir_code_block_new(ir_code_block_get_symbols(parent_block));

    sem_analyze_ast_code_block_to_ir(source_file,
                                     ast_if_block_get_body(ast_if_block),
                                     body);

    return
        ir_if_block_new(ast_if_block_get_condition(ast_if_block),
                        body);
    
}

/**
 * convert AST if_else statment to IR form.
 */
static IrIfElse * 
sem_analyze_ast_if_else_to_ir(const char *source_file,
                              IrCodeBlock *parent_block,
                              AstIfElse *ast_if_else)
{
    IrIfElse *ifelse;
    GSList   *i;

    ifelse = ir_if_else_new();

    /* convert AST if-blocks to ir form */
    i = ast_if_else_get_if_else_blocks(ast_if_else);
    for (; i != NULL; i = g_slist_next(i))
    {
        IrIfBlock *ifblock =
               sem_analyze_ast_if_block_to_ir(source_file,
                                              parent_block,
                                              XDP_AST_IF_BLOCK(i->data));
        ir_if_else_add_if_else_block(ifelse, ifblock);
    }

    /* convert else-body code block to ir form, if else block is present */
    AstCodeBlock *ast_else_body = ast_if_else_get_else_block(ast_if_else);
    if (ast_else_body != NULL)
    {
        IrCodeBlock *else_body =
            ir_code_block_new(ir_code_block_get_symbols(parent_block));

        sem_analyze_ast_code_block_to_ir(source_file, ast_else_body,
                                         else_body);
        ir_if_else_set_else_body(ifelse, else_body);
    }

    return ifelse;
}

/**
 * convert AST code block representation to IR form.
 */
static void
sem_analyze_ast_code_block_to_ir(const char *source_file,
                                 AstCodeBlock *ast_code_block,
                                 IrCodeBlock *ir_code_block)
{
    GSList      *i;
    sym_table_t *symbols;

    symbols = ir_code_block_get_symbols(ir_code_block);

    /*
     * iterate over statments in the AST code block
     * and convert them to IR form
     */
    i = ast_code_block_get_statments(ast_code_block);
    for (; i != NULL; i = g_slist_next(i))
    {
        AstStatment *stmt = XDP_AST_STATMENT(i->data);
        /* variable declaration found */
        if (XDP_IS_AST_VARIABLE_DEFINITION(stmt))
        {
            /*
             * put the variable definition into the local symbol table
             */
            AstVariableDefinition *var_def =
                XDP_AST_VARIABLE_DEFINITION(stmt);

            AstDataType *var_data_type =
                ast_variable_definition_get_data_type(var_def);

            IrVariable *sym = 
                ir_variable_new(var_data_type,
                                ast_variable_definition_get_name(var_def),
                                ast_variable_definition_get_initializer(var_def));

            if (sym_table_add_symbol(symbols, IR_SYMBOL(sym)) != 0)
            {
                compile_error(source_file, 
                              "redeclaration of symbol '%s'\n",
                              ir_symbol_get_name(IR_SYMBOL(sym)));
                return;
            }

            /* check if there is any initialization expression */
//            AstExpression *init_exp =
//                ast_variable_definition_get_initializer(var_def);
//            if (init_exp != NULL)
//            {
//                AstVariableRef *var_ref;

//                if (XDP_IS_AST_BASIC_TYPE(var_data_type))
//                {
//                    var_ref = ast_variable_ref_new(
//                        ast_variable_definition_get_name(var_def));

//                }
//                else if (XDP_IS_AST_STATIC_ARRAY_TYPE(var_data_type))
//                {
//                    var_ref = 
//                        XDP_AST_VARIABLE_REF(ast_array_slice_ref_new(
//                            ast_variable_definition_get_name(var_def),
//                            NULL, NULL));
//                }
//                else
//                {
//                    /* unexpected variable data type */
//                    assert(false);
//                }

//                /* add the initialization assigment node to new body */
//                AstAssigment *assign =
//                    ast_assigment_new(XDP_AST_VARIABLE_REF(var_ref), init_exp);

//                ir_code_block_add_statment(ir_code_block,
//                                           XDP_AST_STATMENT(assign));
//       
//            }
            /** @todo delete the stmt node here */
        }
        /* sub-code block found */
        else if (XDP_IS_AST_CODE_BLOCK(stmt))
        {
            IrCodeBlock *sub_block =
                ir_code_block_new(symbols);

            sem_analyze_ast_code_block_to_ir(source_file,
                                             XDP_AST_CODE_BLOCK(stmt),
                                             sub_block);
            ir_code_block_add_statment(ir_code_block, sub_block);
        }
        else if (XDP_IS_AST_IF_ELSE(stmt))
        {
            IrIfElse *ifelse =
                sem_analyze_ast_if_else_to_ir(source_file,
                                              ir_code_block,
                                              XDP_AST_IF_ELSE(stmt));

            ir_code_block_add_statment(ir_code_block,
                                       ifelse);

            /** @todo delete the stmt node here */
        }
        else
        {
            ir_code_block_add_statment(ir_code_block, stmt);
        }
    }
}

/**
 * convert AST representation of a function to IR form.
 */
static IrFunction *
sem_analyze_ast_func_to_ir(const char *source_file,
                           AstFunction *ast_func,
                           sym_table_t *global_sym_table)
{
    IrFunction *ir_func;
//    IrCodeBlock *body;
//    GSList      *parameters;
//    sym_table_t *body_symbols;
//    GSList      *i;

//    parameters = ast_function_get_parameters(ast_func);

//    body = 
//        ir_code_block_new(global_sym_table);

//    body_symbols = ir_code_block_get_symbols(body);

    ir_func = 
        ir_function_new(ast_function_get_return_type(ast_func),
                        ast_function_get_name(ast_func),
                        ast_function_get_parameters(ast_func),
                        global_sym_table);

    /*
     * put function formal parameters into body's symbol table
     */
//    for (i = parameters; i != NULL; i = g_slist_next(i))
//    {
//        AstVariableDeclaration *param = 
//            XDP_AST_VARIABLE_DECLARATION(i->data);
//        IrVariable *sym;

//        sym = 
//            ir_variable_new(ast_variable_declaration_get_data_type(param),
//                            ast_variable_declaration_get_name(param));

//        if (sym_table_add_symbol(body_symbols, IR_SYMBOL(sym)) != 0)
//        {
//            compile_error(source_file, 
//                         "function %s parameter '%s' is already declared\n",
//                         ast_function_get_name(ast_func),
//                         ir_symbol_get_name(IR_SYMBOL(sym)));
//            return NULL;
//        }
//    }

    /* convert function body to ir format */
    sem_analyze_ast_code_block_to_ir(source_file,
                                     ast_function_get_body(ast_func),
                                     ir_function_get_body(ir_func));

    return ir_func;
}

//static void
//sem_analyze_function(AstFunction *ast_func, ir_compile_unit_t *comp_unit)
//{
//    GSList *listp;
//    ir_function_def_t *ir_func;

//    /* create the IR function object */
//    ir_func = 
//        ir_function_def_new(ir_compile_unit_get_global_sym_table(comp_unit));

//    /*
//     * Store arguments in the IR function object
//     */
//    listp = ast_function_get_parameters(ast_func);
//    for (;listp != NULL; listp = listp->next)
//    {
//        AstVariableDeclaration *param;

//        param = XDP_AST_VARIABLE_DECLARATION(listp->data);
//        ir_variable_def_t *var = 
//            ir_variable_def_new(ast_variable_declaration_get_name(param), param->type);
//        ir_function_def_add_parameter(ir_func, var);
//    }

//    /*
//     * Analyze the statments in the function body
//     */
//    AstCodeBlock *body = ast_function_get_body(ast_func);
//    AstCodeBlock *new_body = ast_code_block_new();

//    listp = ast_code_block_get_statments(body);
//    for (; listp != NULL; listp = listp->next)
//    {
//        AstStatment *stmt = XDP_AST_STATMENT(listp->data);
//        ir_variable_def_t *var;

//        if (XDP_IS_AST_VARIABLE_DEFINITION(stmt))
//        {
//            /* put the variable definition into the local symbol table */
//            AstVariableDefinition *var_def =
//                XDP_AST_VARIABLE_DEFINITION(stmt);

//            AstDataType *var_data_type =
//                ast_variable_definition_get_data_type(var_def);

//            var = 
//                ir_variable_def_new(ast_variable_definition_get_name(var_def), 
//                                    var_data_type);
//            ir_function_def_add_local_var(ir_func, var);


//            /* check if there is any initialization expression */
//            AstExpression *init_exp =
//                ast_variable_definition_get_initializer(var_def);
//            if (init_exp != NULL)
//            {
//                AstVariableRef *var_ref;

//                if (XDP_IS_AST_BASIC_TYPE(var_data_type))
//                {
//                    var_ref = ast_variable_ref_new(
//                        ast_variable_definition_get_name(var_def));

//                }
//                else if (XDP_IS_AST_STATIC_ARRAY_TYPE(var_data_type))
//                {
//                    var_ref = 
//                        XDP_AST_VARIABLE_REF(ast_array_slice_ref_new(
//                            ast_variable_definition_get_name(var_def),
//                            NULL, NULL));
//                }
//                else
//                {
//                    /* unexpected variable data type */
//                    assert(false);
//                }

//                /* add the initialization assigment node to new body */
//                AstAssigment *assign =
//                    ast_assigment_new(XDP_AST_VARIABLE_REF(var_ref), init_exp);

//                ast_code_block_add_statment(new_body, XDP_AST_STATMENT(assign));
//       
//            }
//            /** @todo delete the stmt node here */
//        }
//        else
//        {
//            ast_code_block_add_statment(new_body, stmt);
//        }
//    }
///** @todo: delete (old) body node */
////    ast_node_del(body);

//    ir_function_def_set_name(ir_func,
//                             ast_function_get_name(ast_func));
//    ir_function_def_set_body(ir_func, new_body);
//    ir_function_def_set_return_type(ir_func,
//                                    ast_function_get_return_type(ast_func));
//    ir_compile_unit_add_function(comp_unit, ir_func);
//}

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

IrCompileUnit *
semantic_analyze(const char *source_file, AstCompileUnit *ast_compile_unit)
{
    IrCompileUnit *comp_unit;
    GSList        *ptr;
    IrFunction    *ir_func;
    sym_table_t   *global_sym_table;

    comp_unit = ir_compile_unit_new();
    global_sym_table = ir_compile_unit_get_symbols(comp_unit);

    ptr = ast_compile_unit_get_functions(ast_compile_unit);
    for (;ptr != NULL; ptr = ptr->next)
    {
          ir_func = sem_analyze_ast_func_to_ir(
              source_file,
              XDP_AST_FUNCTION(ptr->data),
              global_sym_table);
          if (ir_func == NULL)
          {
              /* semantic error found, bail out */
              return NULL;
          }
          ir_compile_unit_add_function(comp_unit, ir_func);
    }

    return comp_unit;
}


