#include "x86.h"
#include "x86_code_block.h"
#include "x86_frame_offset.h"
#include "types.h"
#include "dt_array_type.h"
#include "ir_foreach.h"

#include <assert.h>


/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

int
x86_foreach_assign_addrs(x86_comp_params_t *params,
                         int first_offset,
                         IrForeach *foreach)
{
  IrVariable *var;
  int offset = first_offset;

  
  var = ir_foreach_get_value(foreach);
  offset -= x86_get_expression_storage_size(IR_EXPRESSION(var));
  ir_variable_set_location(var, G_OBJECT(x86_frame_offset_new(offset)));
  fprintf(params->out, "# variable '%s' location %d\n",
          ir_variable_get_name(var), offset);

  var = ir_foreach_get_index(foreach);

  if (var != NULL)
  {
      offset -= x86_get_expression_storage_size(IR_EXPRESSION(var));
      ir_variable_set_location(var, G_OBJECT(x86_frame_offset_new(offset)));
      fprintf(params->out, "# variable '%s' location %d\n",
              ir_variable_get_name(var), offset);
  }

  return x86_code_block_assign_addrs(params,
                                     offset,
                                     ir_foreach_get_body(foreach));
}

void
x86_compile_foreach(x86_comp_params_t *params,
                    IrForeach *foreach,
                    sym_table_t *sym_table)
{
    IrArraySlice *aggregate;
    IrVariable *idx;
    IrVariable *val;
    int idx_offset;
    int val_offset;
    int aggregate_offset;
    guint storage_size;
    char loop_itr[LABEL_MAX_LEN];
    char loop_end[LABEL_MAX_LEN];


    fprintf(params->out,
            "# foreach loop\n");

    /*
     * get index variable offset, if it is specified
     */
    idx = ir_foreach_get_index(foreach);
    if (idx != NULL)
    {
        idx_offset = 
            x86_frame_offset_get_offset(
                X86_FRAME_OFFSET(ir_variable_get_location(idx)));
    }

    /*
     * get value variable offset
     */
    val = ir_foreach_get_value(foreach);
    val_offset =
        x86_frame_offset_get_offset(
            X86_FRAME_OFFSET(ir_variable_get_location(val)));


    /*
     * get aggregate start offset and elemnt storage size
     */
    aggregate = ir_foreach_get_aggregate(foreach);
    aggregate_offset =
        x86_frame_offset_get_offset(
            X86_FRAME_OFFSET(
               ir_variable_get_location(
                   ir_lvalue_get_variable(IR_LVALUE(aggregate)))));
    storage_size =
      dt_array_get_element_size(
          DT_ARRAY_TYPE(ir_variable_get_data_type(
              ir_lvalue_get_variable(IR_LVALUE(aggregate)))));

    /* generate labels */
    label_gen_next(&(params->label_gen), loop_itr);
    label_gen_next(&(params->label_gen), loop_end);


    /*
     * evaluate aggregate array slice end and start indexes expressions
     */
    fprintf(params->out,
            "  # aggregate end index\n");
    x86_compile_expression(params,
                           ir_array_slice_get_end(aggregate),
                           sym_table);

    fprintf(params->out,
            "  # aggregate start index\n");

    x86_compile_expression(params,
                           ir_array_slice_get_start(aggregate),
                           sym_table);

    if (idx != NULL)
    {
        fprintf(params->out,
                "    movl $0, %d(%%ebp)  # set index variable to 0\n",
                idx_offset);
    }

    fprintf(params->out,
            "    movl (%%esp), %%eax\n"
            "  # assign current aggregate element to value variable\n"
            "%s:\n",
            loop_itr);

    switch (storage_size)
    {
        case 4:
            fprintf(params->out,
                    "    movl %d(%%ebp, %%eax, 4), %%eax\n"
                    "    movl %%eax, %d(%%ebp)\n",
                    aggregate_offset, val_offset);
            break;
       case 1:
            fprintf(params->out,
                    "    movb %d(%%ebp, %%eax, 1), %%bl\n"
                    "    movb %%bl, %d(%%ebp)\n",
                    aggregate_offset, val_offset);
            break;
       default:
            /* unexpected storage size */
            assert(false);
    }

    fprintf(params->out,
            "  # foreach body\n");
    x86_compile_code_block(params,
                           ir_foreach_get_body(foreach));

    fprintf(params->out,
            "  # advance to next aggregate element\n"
            "    popl %%eax\n"
            "    incl %%eax\n"
            "  # check if we should end the loop\n"
            "    cmpl (%%esp), %%eax\n"
            "    jge %s\n"
            "    pushl %%eax\n",
            loop_end);

    if (idx != NULL)
    {
        fprintf(params->out,
                "    incl %d(%%ebp) # increase index variable\n",
                idx_offset);
    }

    fprintf(params->out,
            "    jmp %s  # do another loop iteration\n"
            "%s:\n"
            "    addl $4, %%esp    # remove end index from stack\n",
            loop_itr, loop_end);
}



