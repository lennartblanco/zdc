import std.stdio;

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
