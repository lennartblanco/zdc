#include <stdbool.h>
#include <string.h>

#include "label_gen.h"
#include "types.h"
#include "x86.h"
#include "x86_cast.h"
#include "x86_if_else.h"
#include "x86_func_call.h"
#include "x86_code_block.h"
#include "x86_frame_offset.h"
#include "x86_reg_location.h"
#include "dt_static_array_type.h"
#include "ast_variable_declaration.h"
#include "ir_array_literal.h"
#include "ir_array_cell_ref.h"
#include "ir_array_slice.h"
#include "ir_cast.h"
#include "ir_unary_operation.h"
#include "ir_binary_operation.h"
#include "ir_int_constant.h"
#include "ir_uint_constant.h"
#include "ir_bool_constant.h"
#include "ir_scalar.h"
#include "ir_variable.h"
#include "ir_return.h"
#include "ir_function_call.h"
#include "ir_if_else.h"
#include "ir_assigment.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
x86_prelude(x86_comp_params_t *params,
            const char *source_file,
            sym_table_t *sym_table);

static void
x86_compile_function_def(x86_comp_params_t *params, IrFunctionDef *func_def);

static void
x86_compile_binary_op(x86_comp_params_t *params,
                      IrBinaryOperation *op,
                      sym_table_t *sym_table);

static void
x86_compile_unary_op(x86_comp_params_t *params,
                     IrUnaryOperation *op,
                     sym_table_t *sym_table);

void
x86_gen_variable_assigment(x86_comp_params_t *params,
                           IrVariable *variable,
                           IrExpression *expression,
                           sym_table_t *sym_table);
void
x86_gen_array_literal_assigment(x86_comp_params_t *params, 
                                IrVariable *variable,
                                IrArrayLiteral *array_literal,
                                sym_table_t *sym_table);

static void
x86_compile_array_cell_ref(x86_comp_params_t *params,
                           IrArrayCellRef *array_cell,
                           sym_table_t *sym_table);

static void
x86_compile_iarithm_op(x86_comp_params_t *params,
                       IrBinaryOperation *op,
                       sym_table_t *sym_table);

static void
x86_compile_icomp_op(x86_comp_params_t *params,
                     IrBinaryOperation *op,
                     sym_table_t *sym_table);

static void
x86_compile_conditional_op(x86_comp_params_t *params,
                           IrBinaryOperation *op,
                           sym_table_t *sym_table);

static void
x86_compile_scalar(x86_comp_params_t *params,
                   IrScalar *scalar);

static void
x86_gen_array_cell_assigment(x86_comp_params_t *params,
                             IrAssigment *assigment,
                             sym_table_t *sym_table);

/**
 * Generate code to pop 32-bit value off the stack and
 * move it to frame offset location (e.g. local variable).
 *
 * @param params       compilation pass parameters handle
 * @param frame_offset function frame offset (offset relative to eps register)
 *                     where to move pop-ed data
 * @param storage_size number of bytes to store, if less then 4 bytes (32-bit),
 *                     more significant bytes will be discarded
 */
static void
x86_gen_store_value(x86_comp_params_t *params,
                    int frame_offset,
                    int storage_size);

/**
 * Generate code for default initialization of an array.
 *
 * @param params       compilation pass parameters handle
 * @param variable     array which should be default initialized
 * @param sym_table    symbols where the array is defined
 */
void
x86_gen_default_array_initializer(x86_comp_params_t *params,
                                  IrVariable *variable,
                                  sym_table_t *sym_table);

static void
x86_compile_array_slice_assigment(x86_comp_params_t *params,
                                  IrAssigment *assigment,
                                  sym_table_t *sym_table);

static void
x86_compile_array_literal_to_slice_assigment(x86_comp_params_t *params,
                                             IrAssigment *assigment,
                                             sym_table_t *sym_table);

static void
x86_compile_array_slice_to_slice_assigment(x86_comp_params_t *params,
                                             IrAssigment *assigment,
                                             sym_table_t *sym_table);
static void
x86_compile_array_slice(x86_comp_params_t *params,
                        IrArraySlice *array_slice,
                        sym_table_t *sym_table);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

void
x86_gen_code(IrCompileUnit *comp_unit,
             FILE *out_stream,
             const char *source_file)
{
    x86_comp_params_t params;
    sym_table_t *global_sym_table;
    GSList *i;

    params.out = out_stream;
    label_gen_init(&(params.label_gen));
    global_sym_table = ir_compile_unit_get_symbols(comp_unit);

    x86_prelude(&params, source_file, global_sym_table);

    i = ir_compile_unit_get_function_defs(comp_unit);
    for (; i != NULL; i = g_slist_next(i))
    {
        assert(IR_IS_FUNCTION_DEF(i->data));
        x86_compile_function_def(&params, i->data);
    }
}

void
x86_compile_expression(x86_comp_params_t *params,
                       IrExpression *expression,
                       sym_table_t *sym_table)
{
    assert(params);
    assert(expression);
    assert(IR_IS_EXPRESSION(expression));

    if (IR_IS_INT_CONSTANT(expression))
    {
        fprintf(params->out,
                "    pushl $%d     # push integer constant onto stack\n", 
                ir_int_constant_get_value(IR_INT_CONSTANT(expression)));
    }
    else if (IR_IS_UINT_CONSTANT(expression))
    {
        fprintf(params->out,
                "    pushl $%u     # push unsigned integer constant onto stack\n", 
                ir_uint_constant_get_value(IR_UINT_CONSTANT(expression)));
    }
    else if (IR_IS_BOOL_CONSTANT(expression))
    {
        gboolean val;

        val = ir_bool_constant_get_value(IR_BOOL_CONSTANT(expression));
        fprintf(params->out,
               "# push boolean constant onto stack\n"
               "    pushl $%d\n",
               val ? 1 : 0);
    }
    else if (IR_IS_UNARY_OPERATION(expression))
    {
        x86_compile_unary_op(params,
                             IR_UNARY_OPERATION(expression),
                             sym_table);
    }
    else if (IR_IS_BINARY_OPERATION(expression))
    {
        x86_compile_binary_op(params,
                              IR_BINARY_OPERATION(expression),
                              sym_table);
    }
    else if (IR_IS_ARRAY_CELL_REF(expression))
    {
        x86_compile_array_cell_ref(params,
                                   IR_ARRAY_CELL_REF(expression),
                                   sym_table);
    }
    else if (IR_IS_SCALAR(expression))
    {
        x86_compile_scalar(params, IR_SCALAR(expression));
    }
    else if (IR_IS_FUNCTION_CALL(expression))
    {
        x86_compile_func_call(params,
                              IR_FUNCTION_CALL(expression),
                              sym_table,
                              true);
    }
    else if (IR_IS_ARRAY_SLICE(expression))
    {
        x86_compile_array_slice(params,
                                IR_ARRAY_SLICE(expression),
                                sym_table);
                               
    }
    else if (IR_IS_CAST(expression))
    {
        x86_compile_cast(params,
                         IR_CAST(expression),
                         sym_table);
    }
    else
    {
        /* unexpected expression type */
        printf("%s\n", g_type_name(G_TYPE_FROM_INSTANCE(expression)));
        assert(false);
    }
}

int
x86_get_variable_storage_size(IrVariable *variable)
{
    assert(variable);
    assert(IR_IS_VARIABLE(variable));

    DtDataType *variable_type;

    variable_type = ir_variable_get_data_type(variable);
    if (DT_IS_BASIC_TYPE(variable_type))
    {
        DtBasicType *basic_type = DT_BASIC_TYPE(variable_type);

        return types_get_storage_size(dt_basic_type_get_data_type(basic_type));
    }
    else if (DT_IS_STATIC_ARRAY_TYPE(variable_type))
    {
        DtStaticArrayType *array_type;
        int len;

        array_type = DT_STATIC_ARRAY_TYPE(variable_type);
        len = dt_static_array_type_get_length(array_type);

        return 
            len * types_get_storage_size(
                      dt_static_array_type_get_data_type(array_type));
    }
    else
    {
        /* unexpected data type */
        assert(false);
    }
    /* we should not get here */
    assert(false);
    return 0;
}

void
x86_compile_assigment(x86_comp_params_t *params,
                      IrAssigment *assigment,
                      sym_table_t *sym_table)
{
    assert(params);
    assert(IR_IS_ASSIGMENT(assigment));
    assert(sym_table);

    IrLvalue *lvalue;

    lvalue = ir_assigment_get_lvalue(assigment);

    if (IR_IS_SCALAR(lvalue))
    {
        x86_gen_variable_assigment(params,
                                   ir_lvalue_get_variable(lvalue),
                                   ir_assigment_get_value(assigment),
                                   sym_table);
    }
    else if (IR_IS_ARRAY_CELL_REF(lvalue))
    {
        x86_gen_array_cell_assigment(params, assigment, sym_table);
    }
    else if (IR_IS_ARRAY_SLICE(lvalue))
    {
        x86_compile_array_slice_assigment(params, assigment, sym_table);
    }
    else
    {
        /* unexpected lvalue type */
        assert(false);
    }
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
x86_prelude(x86_comp_params_t *params,
            const char *source_file,
            sym_table_t *sym_table)
{
    IrSymbol *main_symb;

    fprintf(params->out,
            "    .file \"%s\"\n"
            "    .text\n",
            source_file);

    /*
     * if entry point function main() is defined,
     * generate code to call it and to terminate the application
     * after main() have returned
     */
    main_symb = sym_table_get_symbol(sym_table, "main");
    if (IR_IS_FUNCTION(main_symb))
    {
      IrFunction *main_func;
      bool exit_code_returned;

      main_func = IR_FUNCTION(main_symb);
      
      /* only entry point without arguments supported */
      assert(ir_function_get_parameters(main_func) == NULL);

      /*
       * if main() returns a value, use it as exit code,
       * otherwise exit with code 0
       */
      exit_code_returned = 
        types_is_int(ir_function_get_return_type(main_func));

      fprintf(params->out,
              ".globl _start\n"
              "_start:\n"
              "    call main\n"
              "    movl %s, %%ebx\n"
              "    movl $1, %%eax\n"
              "    int $0x80\n",
              exit_code_returned ? "%eax" : "$0");
    }
}

static void
x86_compile_function_def(x86_comp_params_t *params, IrFunctionDef *func_def)
{
    GSList *i;
    char *func_name;
    int len;
    int addr;
    int stack_start = -4;
    int stack_size;
    sym_table_t *param_symbols;
    bool push_last_arg = false;

    func_name = ir_function_def_get_name(func_def);
    /* generate function symbol declaration and function entry point label */
    fprintf(params->out,
            ".globl %s\n"
            "    .type %s, @function\n"
            "%s:\n",
            func_name, func_name, func_name);

    /* assign locations to function parameter variables */
    i = ir_function_def_get_parameters(func_def);
    param_symbols = ir_function_def_get_parameter_symbols(func_def);

    len = g_slist_length(i);
    addr = len * 4;
    for (; i != NULL; i = g_slist_next(i))
    {
        AstVariableDeclaration *var = i->data;


        /* convert ast variable declaration to IR variable object */
        IrVariable *variable =
            IR_VARIABLE(sym_table_get_symbol(param_symbols,
                                 ast_variable_declaration_get_name(var)));


        /* assign variable number */
        if (addr >= 8)
        {
            ir_variable_set_location(variable,
                                     G_OBJECT(x86_frame_offset_new(addr)));
            fprintf(params->out,
                    "# variable '%s' location %d\n",
                    ir_variable_get_name(variable), addr);
        }
        else
        {
            /* last argument is stored in EAX register */
            push_last_arg = true;
            stack_start = -4;
            ir_variable_set_location(variable,
                                     G_OBJECT(x86_frame_offset_new(-4)));
            fprintf(params->out,
                    "# variable '%s' location %d\n",
                    ir_variable_get_name(variable), -4);

        }
        addr -= 4;
    }

    /* assign stack offset to local variables in function body */
    stack_size = 
        x86_code_block_assign_addrs(params,
                                    stack_start,
                                    ir_function_def_get_body(func_def));

    /* pad stack to allign it on 4-byte boundary */
    if ((stack_size % 4) != 0)
    {
        stack_size -= 4 + stack_size % 4;
    }

    /* generate code to allocate function frame on the stack */
    fprintf(params->out,
            "    enter $%d, $0\n",
            -stack_size);

    /* generate code to store last function argument on the stack */
    if (push_last_arg)
    {
        fprintf(params->out, "    movl %%eax, -4(%%ebp)\n");
    }

    /* generate code for function body */
    x86_compile_code_block(params,
                           ir_function_def_get_body(func_def));
}

static void
x86_gen_store_value(x86_comp_params_t *params,
                    int frame_offset,
                    int storage_size)
{
    switch (storage_size)
    {
        case 4:
            fprintf(params->out,
                    "# store 32-bit variable value from the stack\n"
                    "    popl %d(%%ebp)\n",
                    frame_offset);
            break;
        case 1:
            fprintf(params->out,
                    "# store 8-bit variable value from the stack\n"
                    "    popl %%eax\n"
                    "    movb %%al, %d(%%ebp)\n",
                    frame_offset);
            break;
        default:
            /* unexpected/unsupported storage size */
            assert(false);
    }
}

void
x86_gen_variable_assigment(x86_comp_params_t *params,
                           IrVariable *variable,
                           IrExpression *expression,
                           sym_table_t *sym_table)
{
    X86FrameOffset *addr;

    addr = X86_FRAME_OFFSET(ir_variable_get_location(variable));
    x86_compile_expression(params, expression, sym_table);
    x86_gen_store_value(params,
                        x86_frame_offset_get_offset(addr),
                        x86_get_variable_storage_size(variable));
}

void
x86_gen_default_array_initializer(x86_comp_params_t *params,
                                  IrVariable *variable,
                                  sym_table_t *sym_table)
{
    assert(params);
    assert(IR_IS_VARIABLE(variable));
    assert(sym_table);

    char start_label[LABEL_MAX_LEN];
    IrExpression *init_exp;
    DtStaticArrayType *array_type;
    X86FrameOffset *array_loc;
    int storage_size;

    /* fetch array data type */
    array_type = DT_STATIC_ARRAY_TYPE(ir_variable_get_data_type(variable));
    /* fetch array frame offset */
    array_loc = X86_FRAME_OFFSET(ir_variable_get_location(variable));
    /* fetch array element storage size */
    storage_size =
      types_get_storage_size(dt_static_array_type_get_data_type(array_type));

    /* get default initilizer expression for array element type */
    init_exp =
        types_get_default_initializer(
            dt_static_array_type_get_data_type(array_type));

    /* generate loop label */
    label_gen_next(&(params->label_gen), start_label);


    /* code to evaluate array element default initilizer */
    fprintf(params->out,
            "# evaluate array element default initilizer\n");
    x86_compile_expression(params, init_exp, sym_table);

    /*
     * code to store element default initilizer in register
     * and set loop counter to last element in the array
     */   
    fprintf(params->out,
            "    popl %%ebx\n"
            "    movl $%d, %%eax\n"
            "%s:\n",
            dt_static_array_type_get_length(array_type) - 1,
            start_label);

    /* code to store default value in array element at loop counter index */
    switch (storage_size)
    {
        case 4:
            fprintf(params->out,
                    "    movl %%ebx, %d(%%ebp, %%eax, 4)\n",
                    x86_frame_offset_get_offset(array_loc));

            break;
        case 1:
            fprintf(params->out,
                    "    movb %%bl, %d(%%ebp, %%eax, 1)\n",
                    x86_frame_offset_get_offset(array_loc));
            break;
        default:
            /* unexpected storage size */
            assert(false);
    }

    /* decrement loop counter and check if we should make another iteration */
    fprintf(params->out,
            "    sub $1, %%eax\n"
            "    jae %s\n",
            start_label);
}

void
x86_gen_array_literal_assigment(x86_comp_params_t *params, 
                                IrVariable *variable,
                                IrArrayLiteral *array_literal,
                                sym_table_t *sym_table)
{
    assert(params);
    assert(IR_IS_VARIABLE(variable));
    assert(DT_IS_STATIC_ARRAY_TYPE(ir_variable_get_data_type(variable)));
    assert(X86_IS_FRAME_OFFSET(ir_variable_get_location(variable)));
    assert(IR_IS_ARRAY_LITERAL(array_literal));
    assert(sym_table);

    X86FrameOffset *array_loc;
    int loc;
    int storage_size;
    GSList *i;
    DtStaticArrayType *array_type;

    array_loc = X86_FRAME_OFFSET(ir_variable_get_location(variable));
    array_type = DT_STATIC_ARRAY_TYPE(ir_variable_get_data_type(variable));
    storage_size =
      types_get_storage_size(dt_static_array_type_get_data_type(array_type));

    loc = x86_frame_offset_get_offset(array_loc);
    i = ir_array_literal_get_values(array_literal);

    fprintf(params->out, "# assign array literal to array '%s'\n",
            ir_variable_get_name(variable));

    for (; i != NULL; i = g_slist_next(i), loc += storage_size)
    {
        fprintf(params->out,
                "# evaluate value for loc %d(%%ebp)\n",
                loc);
        x86_compile_expression(params, i->data, sym_table);
        fprintf(params->out,
                "# store value in array cell\n");

        x86_gen_store_value(params, loc, storage_size);
    }
}

/**
 * Generate code for assigment of an array literal expression to
 * array slice.
 */
static void
x86_compile_array_literal_to_slice_assigment(x86_comp_params_t *params,
                                             IrAssigment *assigment,
                                             sym_table_t *sym_table)
{
    assert(params);
    assert(IR_IS_ASSIGMENT(assigment));
    assert(IR_IS_ARRAY_LITERAL(ir_assigment_get_value(assigment)));
    assert(sym_table);

    IrArraySlice *lvalue;
    IrArrayLiteral *rvalue;
    IrVariable *array;
    X86FrameOffset *array_loc;
    int array_offset;
    int storage_size;
    guint array_literal_len;
    GSList *i;
    char loop_label[LABEL_MAX_LEN];

    lvalue = IR_ARRAY_SLICE(ir_assigment_get_lvalue(assigment));
    rvalue = IR_ARRAY_LITERAL(ir_assigment_get_value(assigment));

    array = ir_lvalue_get_variable(IR_LVALUE(lvalue));
    array_loc = X86_FRAME_OFFSET(ir_variable_get_location(array));
    array_offset = x86_frame_offset_get_offset(array_loc);

    label_gen_next(&(params->label_gen), loop_label);

    /* fetch array element storage size */
    storage_size =
      types_get_storage_size(
          dt_static_array_type_get_data_type(DT_STATIC_ARRAY_TYPE(ir_variable_get_data_type(array))));

    /*
     * evaluate array slice start and stop index expressions
     */
    fprintf(params->out,
            "# assign array literal to array slice\n"
            "  # evaluate array slice start expression\n");
    x86_compile_expression(params,
                           ir_array_slice_get_start(lvalue),
                           sym_table);

    fprintf(params->out,
            "  # evaluate array slice end expression\n");
    x86_compile_expression(params,
                           ir_array_slice_get_end(lvalue),
                           sym_table);

    /*
     * evaluate array literal expressions, in reversed order
     */
    i = g_slist_copy(ir_array_literal_get_values(rvalue));
    array_literal_len = g_slist_length(i);
    i = g_slist_reverse(i);
    fprintf(params->out, "  #evaluate array literal expressions\n");
    for (; i != NULL; i = g_slist_next(i))
    {
        x86_compile_expression(params,
                               i->data,
                               sym_table);
    }
    g_slist_free(i);

    fprintf(params->out,
            "  # calculate the start address of the array\n"
            "    movl %%ebp, %%eax\n"
            "    addl $%d, %%eax\n"
            "    movl %u(%%esp), %%ebx # store start index in edx\n"
            "    movl %u(%%esp), %%ecx # store end index in eax\n"
            "%s:\n"
            "    popl %%edx            # store array literal element in edx\n",
            array_offset,
            (array_literal_len + 1) * 4,
            array_literal_len * 4,
            loop_label);

    switch (storage_size)
    {
        case 4:
            fprintf(params->out,
                    "    movl %%edx, (%%eax, %%ebx, 4)\n");
            break;
        case 1:
            fprintf(params->out,
                    "    movb %%dl, (%%eax, %%ebx, 1)\n");
            break;
        default:
            /* unexpected storage size */
            assert(false);
    }

    fprintf(params->out,
            "    inc %%ebx\n"
            "    cmp %%ecx, %%ebx\n"
            "    jl %s\n"
            "    # remove slice start and end values from stack\n"
            "    addl $8, %%esp\n",
            loop_label);
}

static void
x86_compile_array_slice(x86_comp_params_t *params,
                        IrArraySlice *array_slice,
                        sym_table_t *sym_table)
{
    IrVariable *var;
    int array_offset;
    int storage_size;
    guint index_shift_steps;

    /*
     * get source slice arrays frame offset
     */
    var = ir_lvalue_get_variable(IR_LVALUE(array_slice));
    array_offset = 
        x86_frame_offset_get_offset(
             X86_FRAME_OFFSET(ir_variable_get_location(var)));

    /*
     * get array elements storage size
     */
    storage_size =
      types_get_storage_size(
          dt_array_type_get_data_type(
              DT_ARRAY_TYPE(ir_expression_get_data_type(
                  IR_EXPRESSION(array_slice)))));

    /*
     * we use left shift to operation to multiply index of element 
     * with arrays storage size, figure out how many bits should be shifted
     */
    switch (storage_size)
    {
        case 4:
            index_shift_steps = 2;
            break;
        case 2:
            index_shift_steps = 1;
            break;
        case 1:
            index_shift_steps = 0;
            break;
        default:
            /* unsupported element array size */
            assert(false);
    }


    fprintf(params->out,
            "# put contents of array slice on the stack\n"
            " # evaluate array slice start index\n");
    x86_compile_expression(params,
                           ir_array_slice_get_start(array_slice),
                           sym_table);

    fprintf(params->out,
            " #evaluate array slice end index\n");
    x86_compile_expression(params,
                           ir_array_slice_get_end(array_slice),
                           sym_table);

    fprintf(params->out,
            " # calculate array slice length\n"
            "    popl %%eax\n"
            "    subl (%%esp), %%eax\n");

    if (index_shift_steps > 0)
    {
        fprintf(params->out,
                "    # multiply slice length by array element storage size\n"
                "    sall $%u, %%eax\n",
                index_shift_steps);
    }

    fprintf(params->out,
            " # calculate array slice effective start address\n"
            "    popl %%ecx\n"
            "    lea %d(%%ebp, %%ecx, %d), %%ecx\n"
            " # make room for array slice on the stack\n"
            "    movl %%esp, %%edx\n"
            "    subl %%eax, %%edx\n"
            "    movl %%edx, %%esp\n"
            "    pushl %%eax\n"
            "    pushl %%ecx\n"
            "    pushl %%edx\n"
            "    call memcpy\n"
            "    addl $12, %%esp\n",
            array_offset, storage_size);
}

/**
 * Generate code for assigment of an array slice expression to
 * array slice.
 */
static void
x86_compile_array_slice_to_slice_assigment(x86_comp_params_t *params,
                                             IrAssigment *assigment,
                                             sym_table_t *sym_table)
{
    assert(params);
    assert(IR_IS_ASSIGMENT(assigment));
    assert(IR_IS_ARRAY_SLICE(ir_assigment_get_lvalue(assigment)));
    assert(IR_IS_ARRAY_SLICE(ir_assigment_get_value(assigment)));
    assert(sym_table);

    IrArraySlice *src_slice;
    IrArraySlice *dst_slice;
    IrVariable *var;
    int src_offset;
    int dst_offset;
    int storage_size;
    guint index_shift_steps;

    src_slice = IR_ARRAY_SLICE(ir_assigment_get_value(assigment));
    dst_slice = IR_ARRAY_SLICE(ir_assigment_get_lvalue(assigment));


    /*
     * get source slice arrays frame offset
     */
    var = ir_lvalue_get_variable(IR_LVALUE(src_slice));
    src_offset = 
        x86_frame_offset_get_offset(
             X86_FRAME_OFFSET(ir_variable_get_location(var)));

    /*
     * get destination slice arrays frame offset
     */
    var = ir_lvalue_get_variable(IR_LVALUE(dst_slice));
    dst_offset =
        x86_frame_offset_get_offset(
             X86_FRAME_OFFSET(ir_variable_get_location(var)));

    /*
     * get array elements storage size
     */
    storage_size =
      types_get_storage_size(
          dt_array_type_get_data_type(
              DT_ARRAY_TYPE(ir_expression_get_data_type(
                  IR_EXPRESSION(src_slice)))));

    /*
     * we use left shift to operation to multiply index of element 
     * with arrays storage size, figure out how many bits should be shifted
     */
    switch (storage_size)
    {
        case 4:
            index_shift_steps = 2;
            break;
        case 2:
            index_shift_steps = 1;
            break;
        case 1:
            index_shift_steps = 0;
            break;
        default:
            /* unsupported element array size */
            assert(false);
    }

    /*
     * The assigment indexes are placed as follows in the stack:
     * 
     * destination slice start index  12(%%esp)
     * destination slice end index     8(%%esp)
     * source slice start index        4(%%esp)
     * source slice end index           (%%esp)
     *
     * During calculation we store memcpy arguments in following registers:
     * 
     * %eax - memcpy n (length) argument
     * %ebx - memcpy src argument
     * %ecx - memcpy dest argument
     */

    /*
     * Generate code to evalute slice index expression and put
     * result on the stack, as desribe above.
     */
    fprintf(params->out,
            "# array slice to array slice assigment\n"
            " # destination slice start index\n");

    x86_compile_expression(params,
                           ir_array_slice_get_start(dst_slice),
                           sym_table);

    fprintf(params->out,
            " # destination slice end index\n");

    x86_compile_expression(params,
                           ir_array_slice_get_end(dst_slice),
                           sym_table);

    fprintf(params->out,
            " # source slice start index\n");

    x86_compile_expression(params,
                           ir_array_slice_get_start(src_slice),
                           sym_table);

    fprintf(params->out,
            " # source slice end index\n");

    x86_compile_expression(params,
                           ir_array_slice_get_end(src_slice),
                           sym_table);

    /*
     * Generate code to calculate arguments to memcpy and call it
     */
    fprintf(params->out,
            "    movl 8(%%esp), %%eax  # calculate slice length\n"
            "    subl 12(%%esp), %%eax\n");

    if (index_shift_steps > 0)
    {
        fprintf(params->out,
                "    # multiply slice length by array element storage size\n"
                "    sall $%u, %%eax\n",
                index_shift_steps);
    }

    fprintf(params->out,
            "# memcpy src argument -> ebx\n"
            "    movl 4(%%esp), %%ebx\n"
            "    lea %d(%%ebp, %%ebx, %d), %%ebx\n",
            src_offset, storage_size);

    fprintf(params->out,
            "# memcpy dest argument -> ecx\n"
            "    movl 12(%%esp), %%ecx\n"
            "    lea %d(%%ebp, %%ecx, %d), %%ecx\n",
            dst_offset, storage_size);

    fprintf(params->out,
            "# put memcpy arguments on stack\n"
            "    addl $16, %%esp\n"
            "    pushl %%eax\n"
            "    pushl %%ebx\n"
            "    pushl %%ecx\n"
            "    call memcpy\n"
            "    addl $4, %%esp     # remove memcpy retrun value from stack\n");
}

static void
x86_compile_array_slice_assigment(x86_comp_params_t *params,
                                  IrAssigment *assigment,
                                  sym_table_t *sym_table)
{
    assert(params);
    assert(IR_IS_ASSIGMENT(assigment));
    assert(sym_table);

    IrExpression *value;

    value = ir_assigment_get_value(assigment);

    if (IR_IS_ARRAY_LITERAL(value))
    {
        x86_compile_array_literal_to_slice_assigment(params,
                                                     assigment,
                                                     sym_table);
    }
    else if (IR_IS_ARRAY_SLICE(value))
    {
        x86_compile_array_slice_to_slice_assigment(params,
                                                   assigment,
                                                   sym_table);
    }
    else
    {
        /* unexpected right value type */
        assert(false);
    }

}


static void
x86_compile_array_cell_ref(x86_comp_params_t *params,
                           IrArrayCellRef *array_cell,
                           sym_table_t *sym_table)
{
    assert(params);
    assert(IR_IS_ARRAY_CELL_REF(array_cell));
    assert(sym_table);

    fprintf(params->out,
            "# fetch array cell value\n"
            "  # evaluate index expression\n");

    x86_compile_expression(params,
                           ir_array_cell_ref_get_index(array_cell),
                           sym_table);


    X86FrameOffset *array_loc;
    IrVariable *variable;
    int storage_size;
    DtStaticArrayType *array_type;


    variable = ir_lvalue_get_variable(IR_LVALUE(array_cell));
    array_loc = X86_FRAME_OFFSET(ir_variable_get_location(variable));
    array_type = DT_STATIC_ARRAY_TYPE(ir_variable_get_data_type(variable));
    storage_size = 
      types_get_storage_size(dt_static_array_type_get_data_type(array_type));

    fprintf(params->out,
            "    popl %%eax # store array index in eax\n");

    switch (storage_size)
    {
        case 4:
            fprintf(params->out,
                    "   # fetch 32-bit array element\n"
                    "    movl %d(%%ebp, %%eax, 4), %%eax\n"
                    "    pushl %%eax\n",
                    x86_frame_offset_get_offset(array_loc));
            break;
        case 1:
            fprintf(params->out,
                    "   # fetch 8-bit array element\n"
                    "    xor %%ebx, %%ebx\n"
                    "    movb %d(%%ebp, %%eax, 1), %%bl\n"
                    "    pushl %%ebx\n",
                    x86_frame_offset_get_offset(array_loc));
            break;
        default:
            /* unexpected storage size */
            assert(false);
    }              
}

static void
x86_gen_array_cell_assigment(x86_comp_params_t *params,
                             IrAssigment *assigment,
                             sym_table_t *sym_table)
{
    assert(params);
    assert(IR_IS_ASSIGMENT(assigment));
    assert(sym_table);

    IrArrayCellRef *array_cell;
    DtDataType *cell_type;
    int storage_size;
    X86FrameOffset *array_loc;
    IrVariable *variable;

    fprintf(params->out,
            "# array cell assigment\n");

    /* compile code to evaluate right value */
    x86_compile_expression(params,
                           ir_assigment_get_value(assigment),
                           sym_table);

    /* compile code to evaluate array index expression */
    array_cell = IR_ARRAY_CELL_REF(ir_assigment_get_lvalue(assigment));
    x86_compile_expression(params,
                           ir_array_cell_ref_get_index(array_cell),
                           sym_table);

    cell_type =
        ir_expression_get_data_type(IR_EXPRESSION(array_cell));

    /* only arrays of basic data types implemented */
    assert(DT_IS_BASIC_TYPE(cell_type));

    variable = ir_lvalue_get_variable(IR_LVALUE(array_cell));
    array_loc = X86_FRAME_OFFSET(ir_variable_get_location(variable));

    storage_size =
       types_get_storage_size(
            dt_basic_type_get_data_type(DT_BASIC_TYPE(cell_type)));

    fprintf(params->out,
            "    popl %%eax # store array index in eax\n"
            "    popl %%ebx # store right value in ebx\n");

    switch (storage_size)
    {
        case 4:
            fprintf(params->out,
                    "   # store 32-bit array element\n"
                    "    movl %%ebx, %d(%%ebp, %%eax, 4)\n",
                    x86_frame_offset_get_offset(array_loc));
            break;
        case 1:
            fprintf(params->out,
                    "   # store 8-bit array element\n"
                    "    movb  %%bl, %d(%%ebp, %%eax, 1)\n",
                    x86_frame_offset_get_offset(array_loc));
            break;
        default:
            /* unexpected storage size */
            assert(false);
    }    
}

static void
x86_compile_unary_op(x86_comp_params_t *params,
                     IrUnaryOperation *op,
                     sym_table_t *sym_table)
{
    ast_unary_op_type_t op_type;

    op_type = ir_unary_operation_get_operation(op);

    x86_compile_expression(params,
                           ir_unary_operation_get_operand(op),
                           sym_table);

    switch (op_type)
    {    
        case ast_arithm_neg_op:
            fprintf(params->out,
                    "    negl (%%esp)\n");
            break;
        case ast_bool_neg_op:
            fprintf(params->out,
                    "    notl (%%esp)\n"
                    "    andl $0x1, (%%esp)\n");
            break;
        default:
            /* unexpected unary operation */
            assert(false);
    }

}

static void
x86_compile_iarithm_op(x86_comp_params_t *params,
                       IrBinaryOperation *op,
                       sym_table_t *sym_table)
{
    char *op_mnemonic;
    bool signextend_left_op = false;

    switch (ir_binary_operation_get_operation(op))
    {
        case ast_plus_op:
            op_mnemonic = "addl";
            break;
        case ast_minus_op:
            op_mnemonic = "subl";
            break;
        case ast_mult_op:
            op_mnemonic = "imul";
            break;
        case ast_division_op:
            op_mnemonic = "idiv";
            /*
             * when doing 32-bit signed division divident with idev we
             * must be first extended to 64-bit value
             */
            signextend_left_op = true;
            break;
        default:
            /* unexpected operation type */
            assert(false);
    }
    x86_compile_expression(params,
                           ir_binary_operation_get_left(op),
                           sym_table);
    x86_compile_expression(params,
                           ir_binary_operation_get_right(op),
                           sym_table);
    fprintf(params->out,
            /* move left operand into eax */
            "    movl 4(%%esp), %%eax\n"
            /* place-holder for possible 64-bit extension instruction */
            "%s"
            /* evalute operation */
            "    %s (%%esp), %%eax\n"
            /* remove right operand  from the top of stack */
            "    addl $4, %%esp\n"
            /* overwrite the top stack element with operation result */
            "    movl %%eax, (%%esp)\n",
            /* insert extension of left operand to 64-bit value if needed */
            signextend_left_op ? "    cdq\n" : "",
            op_mnemonic);
    
}

static void
x86_compile_icomp_op(x86_comp_params_t *params,
                     IrBinaryOperation *op,
                     sym_table_t *sym_table)
{
    char *set_suffix;

    switch (ir_binary_operation_get_operation(op))
    {
        case ast_equal_op:
            set_suffix = "e";
            break;
        case ast_not_equal_op:
            set_suffix = "ne";
            break;
        case ast_less_op:
            set_suffix = "l";
            break;
        case ast_greater_op:
            set_suffix = "g";
            break;
        case ast_less_or_eq_op:
            set_suffix = "le";
            break;
        case ast_greater_or_eq_op:
            set_suffix = "ge";
            break;
        default:
            /* unexpected operation type */
            assert(false);
    }

    x86_compile_expression(params,
                           ir_binary_operation_get_left(op),
                           sym_table);
    x86_compile_expression(params,
                           ir_binary_operation_get_right(op),
                           sym_table);
    fprintf(params->out,
            "    xor %%ebx, %%ebx\n"
            "    popl %%eax\n"
            "    cmp %%eax, (%%esp)\n"
            "    set%s %%bl\n"
            "    movl %%ebx, (%%esp)\n",
            set_suffix);
    
}

static void
x86_compile_conditional_op(x86_comp_params_t *params,
                           IrBinaryOperation *op,
                           sym_table_t *sym_table)
{
    char end_label[LABEL_MAX_LEN];
    int shortcut_value;

    switch (ir_binary_operation_get_operation(op))
    {
        case ast_and_op:
            shortcut_value = 0;
            break;
        case ast_or_op:
            shortcut_value = 1;
            break;
        default:
            /* unexpected conditional operation type */
            assert(false);
    }

    label_gen_next(&(params->label_gen), end_label);

    x86_compile_expression(params,
                           ir_binary_operation_get_left(op),
                           sym_table);
    fprintf(params->out,
            /* skip evaluating right operand if we know the operations result */
            "    cmpl $%d, (%%esp)\n"
            "    je %s\n"
            /* remove left operands result from the stack */
            "    addl $4, %%esp\n",
            shortcut_value,
            end_label);

    x86_compile_expression(params,
                           ir_binary_operation_get_right(op),
                           sym_table);
    fprintf(params->out,
            "%s:\n",
            end_label);
}

static void
x86_compile_binary_op(x86_comp_params_t *params,
                      IrBinaryOperation *op,
                      sym_table_t *sym_table)
{
    if (ir_binary_operation_is_arithm(op))
    {
        x86_compile_iarithm_op(params, op, sym_table);
    }
    else if (ir_binary_operation_is_icomp(op))
    {
        x86_compile_icomp_op(params, op, sym_table);
    }
    else if (ir_binary_operation_is_conditional(op))
    {
        x86_compile_conditional_op(params, op, sym_table);
    }
    else
    {
        /* unexpected operation type */
        assert(false);
    }
}

static void
x86_compile_scalar(x86_comp_params_t *params,
                   IrScalar *scalar)
{
    IrVariable *var;
    X86FrameOffset *addr;

    var = ir_lvalue_get_variable(IR_LVALUE(scalar));
    addr = X86_FRAME_OFFSET(ir_variable_get_location(var));

    /* generate code to put the value of the variable on top of the stack */
    switch (x86_get_variable_storage_size(var))
    {
        case 4:
            fprintf(params->out,
                    "# integer variable (32-bit) value fetch\n"
                    "    pushl %d(%%ebp)\n",
                    x86_frame_offset_get_offset(addr));
            break;
        case 1:
            fprintf(params->out,
                    "# boolean variable (8-bit) value fetch\n"
                    "    xor %%eax, %%eax\n"
                    "    movb %d(%%ebp), %%al\n"
                    "    pushl %%eax\n",
                    x86_frame_offset_get_offset(addr));
            break;
        default:
            /* unexpected/unsupported storage size */
            assert(false);
    }
}

