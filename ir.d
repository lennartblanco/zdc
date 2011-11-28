public import GType;
public import GSList;
public import GList;
public import sym_table;
public import iml;
public import dt;

/*---------------------------------------------------------------------------*
 *                          modile declarations                              *
 *---------------------------------------------------------------------------*/

struct IrModule;

extern (C)
{
    GType
    ir_module_get_type();

    IrModule *
    ir_module(void *obj);

    char *
    ir_module_get_fqname(IrModule *self);

    GSList *
    ir_module_get_function_defs(IrModule *self);

    sym_table *
    ir_module_get_symbols(IrModule *self);

    char *
    ir_module_gen_label(IrModule *self);

    GList *
    ir_module_get_data_section(IrModule *self);
}

/*---------------------------------------------------------------------------*
 *                           symbol declarations                             *
 *---------------------------------------------------------------------------*/

struct IrSymbol;

extern (C)
{
    IrSymbol *
    ir_symbol(void *obj);

    IrModule *
    ir_symbol_get_parent_module(IrSymbol *self);
}

/*---------------------------------------------------------------------------*
 *                          function declarations                            *
 *---------------------------------------------------------------------------*/

struct IrFunction;

enum ir_linkage_type
{
  d_linkage,
  c_linkage
}

extern (C)
{
    IrFunction *
    ir_function(void *obj);

    char *
    ir_function_get_mangled_name(IrFunction *self);
}

/*---------------------------------------------------------------------------*
 *                      function definition declarations                     *
 *---------------------------------------------------------------------------*/

struct IrFunctionDef;

extern (C)
{
    GType
    ir_function_def_get_type();

    IrFunctionDef *
    ir_function_def(void *obj);

    char *
    ir_function_def_get_name(IrFunctionDef *self);

    GSList *
    ir_function_def_get_parameters(IrFunctionDef *self);

    DtDataType *
    ir_function_def_get_return_type(IrFunctionDef *self);

    IrCodeBlock *
    ir_function_def_get_body(IrFunctionDef *self);

    iml_func_frame *
    ir_function_def_get_frame(IrFunctionDef *self);

    GSList *
    ir_function_def_get_operations(IrFunctionDef *self);
}

/*---------------------------------------------------------------------------*
 *                          code block declarations                          *
 *---------------------------------------------------------------------------*/

struct IrCodeBlock;

extern (C)
{
    GType
    ir_code_block_get_type();

    IrCodeBlock *
    ir_code_block(void *obj);

    GSList *
    ir_code_block_get_statments(IrCodeBlock *self);
}

/*---------------------------------------------------------------------------*
 *                        return statment declarations                       *
 *---------------------------------------------------------------------------*/

struct IrReturn;

extern (C)
{
    GType
    ir_return_get_type();

    IrReturn *
    ir_return(void *obj);

    IrExpression *
    ir_return_get_return_value(IrReturn *self);
}

/*---------------------------------------------------------------------------*
 *                          foreach loop declarations                        *
 *---------------------------------------------------------------------------*/

struct IrForeach;

extern (C)
{
    GType
    ir_foreach_get_type();
}

/*---------------------------------------------------------------------------*
 *                          expression declarations                          *
 *---------------------------------------------------------------------------*/

struct IrExpression;

extern (C)
{
    IrExpression *
    ir_expression(void *obj);

    DtDataType *
    ir_expression_get_data_type(IrExpression *self);
}

/*---------------------------------------------------------------------------*
 *                           assignment declarations                         *
 *---------------------------------------------------------------------------*/

struct IrAssignment;

extern (C)
{
    GType
    ir_assignment_get_type();

    IrAssignment *
    ir_assignment(void *obj);

    IrExpression *
    ir_assignment_get_lvalue(IrAssignment *self);

    IrExpression *
    ir_assignment_get_value(IrAssignment *self);
}

/*---------------------------------------------------------------------------*
 *                         function call declarations                        *
 *---------------------------------------------------------------------------*/

struct IrFunctionCall;

extern (C)
{
    GType
    ir_function_call_get_type();

    IrFunctionCall *
    ir_function_call(void *obj);

    char *
    ir_function_call_get_name(IrFunctionCall *self);

    GSList *
    ir_function_call_get_arguments(IrFunctionCall *self);
}

/*---------------------------------------------------------------------------*
 *                           variable declarations                           *
 *---------------------------------------------------------------------------*/

struct IrVariable;

extern (C)
{
    GType
    ir_variable_get_type();

    IrVariable *
    ir_variable(void *obj);

    char *
    ir_variable_get_name(IrVariable *self);
}

/*---------------------------------------------------------------------------*
 *                          var value declarations                           *
 *---------------------------------------------------------------------------*/

struct IrVarValue;

extern (C)
{
    GType
    ir_var_value_get_type();

    IrVarValue *
    ir_var_value(void *obj);

    IrVariable *
    ir_var_value_get_var(IrVarValue *self);
}

/*---------------------------------------------------------------------------*
 *                        var reference declarations                         *
 *---------------------------------------------------------------------------*/

struct IrVarRef;

extern (C)
{
    GType
    ir_var_ref_get_type();

    IrVarValue *
    ir_var_ref(void *obj);

    IrVariable *
    ir_var_ref_get_var(IrVarValue *self);
}

/*---------------------------------------------------------------------------*
 *                         address of declarations                           *
 *---------------------------------------------------------------------------*/

struct IrAddressOf;

extern (C)
{
    GType
    ir_address_of_get_type();

    IrAddressOf *
    ir_address_of(void *obj);

    IrExpression *
    ir_address_of_get_expression(IrAddressOf *self);
}

/*---------------------------------------------------------------------------*
 *                           literal declarations                            *
 *---------------------------------------------------------------------------*/

struct IrLiteral;

extern (C)
{
    IrLiteral *
    ir_literal(void *obj);

    char *
    ir_literal_get_data_label(IrLiteral *self);
}

/*---------------------------------------------------------------------------*
 *                        basic constant declarations                        *
 *---------------------------------------------------------------------------*/

struct IrBasicConstant;

extern (C)
{
    GType
    ir_basic_constant_get_type();

    IrBasicConstant *
    ir_basic_constant(void *obj);

    bool
    ir_basic_constant_get_bool(IrBasicConstant *self);

    char
    ir_basic_constant_get_char(IrBasicConstant *self);

    byte
    ir_basic_constant_get_byte(IrBasicConstant *self);

    ubyte
    ir_basic_constant_get_ubyte(IrBasicConstant *self);

    short
    ir_basic_constant_get_short(IrBasicConstant *self);

    ushort
    ir_basic_constant_get_ushort(IrBasicConstant *self);

    int
    ir_basic_constant_get_int(IrBasicConstant *self);

    uint
    ir_basic_constant_get_uint(IrBasicConstant *self);
}

/*---------------------------------------------------------------------------*
 *                        array literal declarations                         *
 *---------------------------------------------------------------------------*/

struct IrArrayLiteral;

extern (C)
{
    GType
    ir_array_literal_get_type();

    bool
    ir_is_array_literal(void *obj);

    IrArrayLiteral *
    ir_array_literal(void *obj);

    GSList *
    ir_array_literal_get_values(IrArrayLiteral *self);
}

/*---------------------------------------------------------------------------*
 *                       struct literal declarations                         *
 *---------------------------------------------------------------------------*/

struct IrStructLiteral;

extern (C)
{
    bool
    ir_is_struct_literal(void *obj);

    IrStructLiteral *
    ir_struct_literal(void *obj);

    GSList *
    ir_struct_literal_get_members(IrStructLiteral *self);
}

/*---------------------------------------------------------------------------*
 *                        struct member declarations                         *
 *---------------------------------------------------------------------------*/

struct IrStructMember;

extern (C)
{
    IrStructMember *
    ir_struct_member(void *obj);

    IrExpression *
    ir_struct_member_get_init(IrStructMember *self);

    uint
    ir_struct_member_get_padding(IrStructMember *self);
}

/*---------------------------------------------------------------------------*
 *                         enum member declarations                          *
 *---------------------------------------------------------------------------*/

struct IrEnumMember;

extern (C)
{
    IrEnumMember *
    ir_enum_member(void *obj);

    IrExpression *
    ir_enum_member_get_value(IrEnumMember *self);
}