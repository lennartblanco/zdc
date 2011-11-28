public import GSList;
public import std.stdio;

/*---------------------------------------------------------------------------*
 *                        operation declarations                             *
 *---------------------------------------------------------------------------*/

struct iml_operation;

enum iml_opcode
{
    add,
    sub,
    smult,
    umult,
    sdiv,
    udiv,
    smod,
    umod,
    and,
    or,
    ineg,
    bneg,
    equal,
    nequal,
    sless,
    uless,
    sgreater,
    ugreater,
    slesseq,
    ulesseq,
    sgreatereq,
    ugreatereq,
    bconv,
    zpad,
    sigext,
    trunc,
    copy,
    mset,
    set,
    get,
    setelm,
    getelm,
    getaddr,
    jmp,
    jmpneq,
    jmpuless,
    jmpugreatereq,
    call,
    call_c,
    ret,
    label
};

extern (C)
{
    iml_operation *
    cast_iml_operation(void *obj);

    iml_opcode
    iml_operation_get_opcode(iml_operation *self);

    void *
    iml_operation_get_operand(iml_operation *self,
                              uint operand_num);

    void
    iml_operation_print(iml_operation *self,
                        FILE *out_stream,
                        int indention);
}

/*---------------------------------------------------------------------------*
 *                         operand declarations                              *
 *---------------------------------------------------------------------------*/

struct ImlOperand;

enum iml_data_type
{
    _8b,
    _16b,
    _32b,
    ptr,
    blob
};

extern (C)
{
    iml_data_type
    iml_operand_get_data_type(ImlOperand *self);

    ImlOperand *
    iml_operand(void *obj);
}

/*---------------------------------------------------------------------------*
 *                         constant declarations                             *
 *---------------------------------------------------------------------------*/

struct ImlConstant;

extern (C)
{
    bool
    iml_is_constant(void *obj);

    ImlConstant *
    iml_constant(void *obj);

    uint
    iml_constant_get_val_32b(ImlConstant *self);

    char *
    iml_constant_get_val_ptr(ImlConstant *self);
}

/*---------------------------------------------------------------------------*
 *                        variable declarations                              *
 *---------------------------------------------------------------------------*/

struct ImlVariable;

extern (C)
{
    bool
    iml_is_variable(void *obj);

    ImlVariable *
    iml_variable(void *obj);

    bool
    iml_variable_is_mem_pinned(ImlVariable *self);

    char *
    iml_variable_get_register(ImlVariable *self);

    void
    iml_variable_set_frame_offset(ImlVariable *self, int frame_offset);

    int
    iml_variable_get_frame_offset(ImlVariable *self);

    iml_data_type
    iml_variable_get_data_type(ImlVariable *self);

    uint
    iml_variable_get_size(ImlVariable *self);
}

/*---------------------------------------------------------------------------*
 *                       function frame declarations                         *
 *---------------------------------------------------------------------------*/

struct iml_func_frame;

extern (C)
{
    GSList *
    iml_func_frame_get_parameters(iml_func_frame *self);

    GSList *
    iml_func_frame_get_locals(iml_func_frame *self);

    void
    iml_func_frame_set_size(iml_func_frame *self, uint stack_size);

    uint
    iml_func_frame_get_size(iml_func_frame *self);

    GSList *
    iml_func_frame_get_used_regs(iml_func_frame *self);

    void
    iml_func_frame_print(iml_func_frame *self, FILE *output, int indention);
}
