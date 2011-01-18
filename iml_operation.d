struct iml_operation_t;

enum iml_opcode_t
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
    copy,
    mset,
    setelm,
    getelm,
    getaddr,
    jmp,
    jmpneq,
    jmpuless,
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