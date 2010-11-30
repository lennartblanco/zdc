struct iml_operation_t;

enum iml_opcode_t
{
    add,
    sub,
    smult,
    umult,
    and,
    or,
    ineg,
    bneg,
    equal,
    nequal,
    sless,
    iuless,
    sgreater,
    ugreater,
    slesseq,
    ulesseq,
    sgreatereq,
    ugreatereq,
    copy,
    mset,
    setfld,
    getfld,
    jmp,
    jmpneq,
    call,
    call_c,
    ret,
    cast_op,
    label
};

extern (C)
{
    iml_opcode_t
    iml_operation_get_opcode(iml_operation_t *self);
    
    void *
    iml_operation_get_operand(iml_operation_t *self,
                              uint operand_num);
}