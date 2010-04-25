#include "x86_array.h"
#include "x86_frame_offset.h"
#include "types.h"
#include "ir_cast.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
x86_compile_array_literal_to_slice_assigment(x86_comp_params_t *params,
                                             IrAssigment *assigment,
                                             sym_table_t *sym_table);

static void
x86_compile_array_slice_to_slice_assigment(x86_comp_params_t *params,
                                           IrAssigment *assigment,
                                           sym_table_t *sym_table);

static void
x86_compile_cast_to_array_slice_assigment(x86_comp_params_t *params,
                                          IrAssigment *assigment,
                                          sym_table_t *sym_table);

static void
x86_compile_basic_type_to_slice_assigment(x86_comp_params_t *params,
                                          IrAssigment *assigment,
                                          sym_table_t *sym_table);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

void
x86_gen_array_handle_assigment(x86_comp_params_t *params,
                               IrAssigment *assigment,
                               sym_table_t *sym_table)
{
    assert(params);
    assert(IR_IS_ASSIGMENT(assigment));
    assert(sym_table);
    assert(IR_IS_ARRAY(ir_assigment_get_lvalue(assigment)));

    /* we only support assigment of array literals to array handles for now */
    assert(IR_IS_ARRAY_LITERAL(ir_assigment_get_value(assigment)));

    IrLvalue *lvalue;
    IrArrayLiteral *value;
    char *value_label;
    GSList *literal_values;
    GSList *i;
    guint cntr;
    guint literal_length;
    int offset;
    guint storage_size;

    lvalue = ir_assigment_get_lvalue(assigment);
    value = IR_ARRAY_LITERAL(ir_assigment_get_value(assigment));
    value_label = ir_array_literal_get_data_label(value);
    literal_values = ir_array_literal_get_values(value);
    literal_length = g_slist_length(literal_values);

    offset =
        x86_frame_offset_get_offset(
            X86_FRAME_OFFSET(ir_lvalue_get_location(lvalue)));
    storage_size =
        dt_array_get_element_size(
            DT_ARRAY_TYPE(ir_expression_get_data_type(IR_EXPRESSION(lvalue))));

    fprintf(params->out,
            "# array literal to array handle assigment\n"
            "    pushl $%d\n"
            "    call GC_malloc\n"
            "    movl %%eax, (%%esp)\n",
            literal_length);

    if (value_label != NULL)
    {
        /* this array literal is stored in .data section */
        fprintf(params->out,
                "  # copy array literal from .data section\n"
                "    pushl $%d\n"
                "    pushl $%s\n"
                "    pushl %%eax\n"
                "    call memcpy\n",
                literal_length * storage_size, value_label);
    }
    else
    {
        for (i = literal_values, cntr = 0;
             i != NULL;
             i = g_slist_next(i), cntr++)
        {
            fprintf(params->out,
                    "  # evaluate array literal value %u\n",
                    cntr);
            x86_compile_expression(params, i->data, sym_table);
            fprintf(params->out,
                    "  # store array literal value %u\n"
                    "    pop %%eax\n"
                    "    movl (%%esp), %%ebx\n"
                    "    movl $%u, %%ecx\n",
                    cntr, cntr);
            switch (storage_size)
            {
                case 4:
                    fprintf(params->out, "    movl %%eax, (%%ebx, %%ecx, 4)\n");
                    break;
                case 1:
                    fprintf(params->out, "    movb %%al, (%%ebx, %%ecx, 1)\n");
                    break;
                default:
                    /* unexpected storage size */
                    assert(false);
            }
        }
    }

    fprintf(params->out,
            "  # assign length and pointer of array literal to array handle\n"
            "    movl $%u, %d(%%ebp) # store length\n"
            "    popl %%eax          # store pointer\n"
            "    movl %%eax, %d(%%ebp)\n",
            literal_length, offset, offset + 4);
}

void
x86_gen_array_cell_assigment(x86_comp_params_t *params,
                             IrAssigment *assigment,
                             sym_table_t *sym_table)
{
    assert(params);
    assert(IR_IS_ASSIGMENT(assigment));
    assert(sym_table);

    IrArrayCell *array_cell;
    DtArrayType *array_type;
    DtDataType *cell_type;
    guint storage_size;
    X86FrameOffset *array_loc;
    IrVariable *variable;

    fprintf(params->out,
            "# array cell assigment\n");

    /* compile code to evaluate right value */
    x86_compile_expression(params,
                           ir_assigment_get_value(assigment),
                           sym_table);

    /* compile code to evaluate array index expression */
    array_cell = IR_ARRAY_CELL(ir_assigment_get_lvalue(assigment));
    x86_compile_expression(params,
                           ir_array_cell_get_index(array_cell),
                           sym_table);

    cell_type =
        ir_expression_get_data_type(IR_EXPRESSION(array_cell));

    /* only arrays of basic data types implemented */
    assert(DT_IS_BASIC_TYPE(cell_type));

    variable = ir_lvalue_get_variable(IR_LVALUE(array_cell));
    array_loc = X86_FRAME_OFFSET(ir_variable_get_location(variable));
    array_type = DT_ARRAY_TYPE(ir_variable_get_data_type(variable));

    storage_size = dt_array_get_element_size(array_type);

    fprintf(params->out,
            "    popl %%eax # store array index in eax\n"
            "    popl %%ebx # store right value in ebx\n");

    if (DT_IS_STATIC_ARRAY_TYPE(array_type))
    {
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
    else if (DT_IS_ARRAY_TYPE(array_type))
    {
        fprintf(params->out,
                "    movl %d(%%ebp), %%ecx # store array pointer in register\n",
                x86_frame_offset_get_offset(array_loc) + 4);

        switch (storage_size)
        {
            case 4:
                fprintf(params->out,
                        "    movl %%ebx, (%%ecx, %%eax, 4) # 32-bit element\n");
                break;
            case 1:
                fprintf(params->out,
                        "    movb %%bl, (%%ecx, %%eax, 1) # 8-bit element\n");
                break;
            default:
                /* unexpected storage size */
                assert(false);
        }
    }
    else
    {
        /* unexpected array type */
        assert(FALSE);
    }

}

void
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
    else if (IR_IS_CAST(value))
    {
        x86_compile_cast_to_array_slice_assigment(params,
                                                  assigment,
                                                  sym_table);
    }
    else
    {
        /* this should be an basic data type assigment to array slice */
        x86_compile_basic_type_to_slice_assigment(params,
                                                  assigment,
                                                  sym_table);
    }
}

void
x86_compile_array(x86_comp_params_t *params,
                  IrArray *array,
                  sym_table_t *sym_table)
{
    assert(params);
    assert(IR_IS_ARRAY(array));
    assert(sym_table);

    int offset;

    offset =
        x86_frame_offset_get_offset(
            X86_FRAME_OFFSET(ir_lvalue_get_location(IR_LVALUE(array))));


    fprintf(params->out,
            "# copy array handle to the stack\n"
            "    pushl %d(%%ebp)\n"
            "    pushl %d(%%ebp)\n",
            offset + 4, offset);

}

void
x86_compile_array_literal(x86_comp_params_t *params,
                          IrArrayLiteral *array_lit,
                          sym_table_t *sym_table)
{
    assert(params);
    assert(IR_IS_ARRAY_LITERAL(array_lit));
    assert(sym_table);

    GSList *literal_values;
    GSList *i;
    guint cntr;
    guint storage_size;

    literal_values = ir_array_literal_get_values(array_lit);

    storage_size =
        dt_array_get_element_size(
            DT_ARRAY_TYPE(
                ir_expression_get_data_type(IR_EXPRESSION(array_lit))));

    fprintf(params->out,
            "# evaluate array literal\n"
            "    pushl $%u    #allocate memory for array literal\n"
            "    call GC_malloc\n"
            "    pushl %%eax\n",
            ir_array_literal_get_size(array_lit));

    for (i = literal_values, cntr = 0; i != NULL; i = g_slist_next(i), cntr++) {
        x86_compile_expression(params, i->data, sym_table);
        fprintf(params->out,
                "    popl %%eax\n"
                "    movl (%%esp), %%ebx\n"
                "    movl $%u, %%ecx\n",
                cntr);

        switch (storage_size)
        {
            case 4:
                fprintf(params->out,
                        "    movl %%eax, (%%ebx, %%ecx, 4)\n");
                break;
            case 1:
                fprintf(params->out,
                        "    movb %%al, (%%ebx, %%ecx, 1)\n");
                break;
            default:
                /* unexpected storage size */
                assert(false);          
        }
    }

    fprintf(params->out,
            "    pushl $%u\n",
            g_slist_length(literal_values));
}

void
x86_compile_array_cell(x86_comp_params_t *params,
                       IrArrayCell *array_cell,
                       sym_table_t *sym_table)
{
    assert(params);
    assert(IR_IS_ARRAY_CELL(array_cell));
    assert(sym_table);

    fprintf(params->out,
            "# fetch array cell value\n"
            "  # evaluate index expression\n");

    x86_compile_expression(params,
                           ir_array_cell_get_index(array_cell),
                           sym_table);


    X86FrameOffset *array_loc;
    IrVariable *variable;
    guint storage_size;
    DtArrayType *array_type;


    variable = ir_lvalue_get_variable(IR_LVALUE(array_cell));
    array_loc = X86_FRAME_OFFSET(ir_variable_get_location(variable));
    array_type = DT_ARRAY_TYPE(ir_variable_get_data_type(variable));
    storage_size =
        dt_data_type_get_size(dt_array_type_get_data_type(array_type));

    fprintf(params->out,
            "    popl %%eax # store array index in eax\n");

    if (DT_IS_STATIC_ARRAY_TYPE(array_type))
    {
        /* fetch array cell value from a static array */
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
    else if (DT_IS_ARRAY_TYPE(array_type))
    {
        fprintf(params->out,
                "# fetch array element\n"
                "    movl %d(%%ebp), %%ebx # store array pointer in register\n",
                x86_frame_offset_get_offset(array_loc) + 4);
 

        /* fetch array cell value from a dynamic array */
        switch (storage_size)
        {
            case 4:
                fprintf(params->out,
                        "    movl (%%ebx, %%eax, 4), %%eax # 32-bit element\n"
                        "    pushl %%eax\n");
                break;
            case 1:
                fprintf(params->out,
                        "    xor %%ecx, %%ecx    # 8-bit element\n"
                        "    movb (%%ebx, %%eax, 1), %%cl\n"
                        "    pushl %%ecx\n");
                break;
            default:
                /* unexpected storage size */
                assert(false);
        }
    }
    else
    {
        /* unexpected array type */
        assert(FALSE);
    }
}

void
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
        dt_array_get_element_size(
            DT_ARRAY_TYPE(
                ir_expression_get_data_type(IR_EXPRESSION(array_slice))));

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

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

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
    guint storage_size;
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
        dt_array_get_element_size(
            DT_ARRAY_TYPE(ir_variable_get_data_type(array)));

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

    if (DT_IS_STATIC_ARRAY_TYPE(ir_variable_get_data_type(array)))
    {
        /* static array */
        fprintf(params->out,
                "  # calculate the start address of the array\n"
                "    movl %%ebp, %%eax\n"
                "    addl $%d, %%eax\n",
                array_offset);
    }
    else
    {
        /* dynamic array */
        fprintf(params->out,
                "  # fetch start address of the array\n"
                "    movl %d(%%ebp), %%eax\n",
                array_offset + 4);
    }

    fprintf(params->out,
            "    movl %u(%%esp), %%ebx # store start index in edx\n"
            "    movl %u(%%esp), %%ecx # store end index in eax\n"
            "%s:\n"
            "    popl %%edx            # store array literal element in edx\n",

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
    guint storage_size;
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
        dt_array_get_element_size(
            DT_ARRAY_TYPE(
                ir_expression_get_data_type(IR_EXPRESSION(src_slice))));
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
            "    lea %d(%%ebp, %%ebx, %u), %%ebx\n",
            src_offset, storage_size);

    fprintf(params->out,
            "# memcpy dest argument -> ecx\n"
            "    movl 12(%%esp), %%ecx\n"
            "    lea %d(%%ebp, %%ecx, %u), %%ecx\n",
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
x86_compile_cast_to_array_slice_assigment(x86_comp_params_t *params,
                                          IrAssigment *assigment,
                                          sym_table_t *sym_table)
{
    assert(params);
    assert(IR_IS_ASSIGMENT(assigment));
    assert(sym_table);

    IrCast *cast;
    IrExpression *value;
    DtArrayType *trgt_type;
    DtArrayType *src_type;

    cast = IR_CAST(ir_assigment_get_value(assigment));
    trgt_type = DT_ARRAY_TYPE(ir_cast_get_target_type(cast));

    value = ir_cast_get_value(cast);
    src_type = DT_ARRAY_TYPE(ir_expression_get_data_type(value));

    /* only casting between uint[] and int[] arrays implemented */
    assert((types_is_int(dt_array_type_get_data_type(trgt_type)) ||
            types_is_uint(dt_array_type_get_data_type(trgt_type))) &&
           (types_is_int(dt_array_type_get_data_type(src_type)) ||
            types_is_uint(dt_array_type_get_data_type(src_type))));
 
    /*
     * no need to generate explicit casting code when going between
     * uint[] and int[] arrays, just overwrite the cast expression with
     * it's value and compile the assigment.
     */
    ir_assigment_set_value(assigment, value);
    x86_compile_array_slice_assigment(params, assigment, sym_table);
}

static void
x86_compile_basic_type_to_slice_assigment(x86_comp_params_t *params,
                                          IrAssigment *assigment,
                                          sym_table_t *sym_table)
{
    assert(params);
    assert(IR_IS_ASSIGMENT(assigment));
    assert(sym_table);

    IrExpression *value;
    IrArraySlice *array_slice;
    IrVariable *var;
    int array_offset;
    guint storage_size;
    char loop_label[LABEL_MAX_LEN];


    label_gen_next(&(params->label_gen), loop_label);

    array_slice = IR_ARRAY_SLICE(ir_assigment_get_lvalue(assigment));
    value = ir_assigment_get_value(assigment);
    assert(DT_IS_BASIC_TYPE(ir_expression_get_data_type(value)));

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
        dt_array_get_element_size(
            DT_ARRAY_TYPE(
                ir_expression_get_data_type(IR_EXPRESSION(array_slice))));

    fprintf(params->out,
            "# assign basic type expression to an array slice\n"
            " # eval assignee\n");
    x86_compile_expression(params, value, sym_table);

    fprintf(params->out,
            " # eval array slice start index expression\n");
    x86_compile_expression(params,
                           ir_array_slice_get_start(array_slice),
                           sym_table);
    fprintf(params->out,
            " # eval array slice end index expression\n");
    x86_compile_expression(params,
                           ir_array_slice_get_end(array_slice),
                           sym_table);

    fprintf(params->out,
            "    popl %%ecx   # calc array slice length\n"
            "    subl (%%esp), %%ecx\n"
            "    pop %%eax\n"
            "    lea %d(%%ebp, %%eax, %u), %%eax\n"
            "    pop %%edx\n"
            "%s:\n",
            array_offset, storage_size, loop_label);

    switch (storage_size)
    {
        case 4:
            fprintf(params->out,
                    "    movl %%edx, (%%eax)\n");
            break;
        case 1:
            fprintf(params->out,
                    "    movb %%dl, (%%eax)\n");
            break;
        default:
            /* unexpected storage size */
            g_assert(false);
    }

    fprintf(params->out,
            "    addl $%u, %%eax\n"
            "    loop %s\n",
            storage_size, loop_label);
}
