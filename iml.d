public import GSList;
public import std.stdio;

extern (C):

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
}

/* a workaround for dmd bug(?) where cast(iml_operation*) does not work */
iml_operation *
cast_iml_operation(void *obj);

iml_opcode
iml_operation_get_opcode(iml_operation *self);

void *
iml_operation_get_operand(iml_operation *self, uint operand_num);

void
iml_operation_print(iml_operation *self, FILE *out_stream, int indention);

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
}

iml_data_type
iml_operand_get_data_type(ImlOperand *self);

ImlOperand *
iml_operand(void *obj);

/*---------------------------------------------------------------------------*
 *                         constant declarations                             *
 *---------------------------------------------------------------------------*/

struct ImlConstant;

bool
iml_is_constant(void *obj);

ImlConstant *
iml_constant(void *obj);

uint
iml_constant_get_val_32b(ImlConstant *self);

char *
iml_constant_get_val_ptr(ImlConstant *self);

/*---------------------------------------------------------------------------*
 *                        variable declarations                              *
 *---------------------------------------------------------------------------*/

struct ImlVariable;

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

/*---------------------------------------------------------------------------*
 *                          function declarations                            *
 *---------------------------------------------------------------------------*/

struct iml_function;

/* a workaround for dmd bug(?) where cast(iml_function*) does not work */
iml_function *
cast_iml_function(void *obj);

char *
iml_function_get_name(iml_function *self);

GSList *
iml_function_get_parameters(iml_function *self);

GSList *
iml_function_get_locals(iml_function *self);

void
iml_function_set_frame_size(iml_function *self, uint stack_size);

uint
iml_function_get_frame_size(iml_function *self);

GSList *
iml_function_get_used_regs(iml_function *self);

GSList *
iml_function_get_operations(iml_function *self);

void
iml_function_print(iml_function *self, FILE *output, int indention);