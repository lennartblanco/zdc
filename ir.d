public import GType;
public import GSList;
public import GList;
public import sym_table;
public import dt;

extern (C):

/*---------------------------------------------------------------------------*
 *                          modile declarations                              *
 *---------------------------------------------------------------------------*/

struct IrModule;

GType
ir_module_get_type();

IrModule *
ir_module(void *obj);

IrScope *
ir_module_get_scope(IrModule *self);

GSList *
ir_module_get_functions(IrModule *self);

GSList *
ir_module_get_function_defs(IrModule *self);

sym_table *
ir_module_get_symbols(IrModule *self);

char *
ir_module_gen_label(IrModule *self);

GList *
ir_module_get_data_section(IrModule *self);

/*---------------------------------------------------------------------------*
 *                          scope  declarations                              *
 *---------------------------------------------------------------------------*/

struct IrScope;

char *
ir_scope_get_fqname(IrScope *self);

/*---------------------------------------------------------------------------*
 *                           symbol declarations                             *
 *---------------------------------------------------------------------------*/

struct IrSymbol;

IrSymbol *
ir_symbol(void *obj);

IrModule *
ir_symbol_get_parent_module(IrSymbol *self);

/*---------------------------------------------------------------------------*
 *                          function declarations                            *
 *---------------------------------------------------------------------------*/

struct IrFunction;

enum ir_linkage_type
{
    d_linkage,
    c_linkage
}

IrFunction *
ir_function(void *obj);

char *
ir_function_get_mangled_name(IrFunction *self);

/*---------------------------------------------------------------------------*
 *                      function definition declarations                     *
 *---------------------------------------------------------------------------*/

struct IrFunctionDef;

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

/*---------------------------------------------------------------------------*
 *                          code block declarations                          *
 *---------------------------------------------------------------------------*/

struct IrCodeBlock;

GType
ir_code_block_get_type();

IrCodeBlock *
ir_code_block(void *obj);

GSList *
ir_code_block_get_statments(IrCodeBlock *self);

/*---------------------------------------------------------------------------*
 *                        return statment declarations                       *
 *---------------------------------------------------------------------------*/

struct IrReturn;

GType
ir_return_get_type();

IrReturn *
ir_return(void *obj);

IrExpression *
ir_return_get_return_value(IrReturn *self);


/*---------------------------------------------------------------------------*
 *                          foreach loop declarations                        *
 *---------------------------------------------------------------------------*/

struct IrForeach;

GType
ir_foreach_get_type();

IrForeach *
ir_foreach(void *obj);

IrVariable *
ir_foreach_get_index(IrForeach *self);

IrVariable *
ir_foreach_get_value(IrForeach *self);

IrCodeBlock *
ir_foreach_get_body(IrForeach *self);

/*---------------------------------------------------------------------------*
 *                          expression declarations                          *
 *---------------------------------------------------------------------------*/

struct IrExpression;

IrExpression *
ir_expression(void *obj);

DtDataType *
ir_expression_get_data_type(IrExpression *self);

/*---------------------------------------------------------------------------*
 *                           assignment declarations                         *
 *---------------------------------------------------------------------------*/

struct IrAssignment;

GType
ir_assignment_get_type();

IrAssignment *
ir_assignment(void *obj);

IrExpression *
ir_assignment_get_lvalue(IrAssignment *self);

IrExpression *
ir_assignment_get_value(IrAssignment *self);

/*---------------------------------------------------------------------------*
 *                         function call declarations                        *
 *---------------------------------------------------------------------------*/

struct IrFunctionCall;

GType
ir_function_call_get_type();

IrFunctionCall *
ir_function_call(void *obj);

char *
ir_function_call_get_name(IrFunctionCall *self);

GSList *
ir_function_call_get_arguments(IrFunctionCall *self);

/*---------------------------------------------------------------------------*
 *                           variable declarations                           *
 *---------------------------------------------------------------------------*/

struct IrVariable;

GType
ir_variable_get_type();

IrVariable *
ir_variable(void *obj);

char *
ir_variable_get_name(IrVariable *self);


/*---------------------------------------------------------------------------*
 *                          var value declarations                           *
 *---------------------------------------------------------------------------*/

struct IrVarValue;

GType
ir_var_value_get_type();

IrVarValue *
ir_var_value(void *obj);

IrVariable *
ir_var_value_get_var(IrVarValue *self);

/*---------------------------------------------------------------------------*
 *                        var reference declarations                         *
 *---------------------------------------------------------------------------*/

struct IrVarRef;

GType
ir_var_ref_get_type();

IrVarValue *
ir_var_ref(void *obj);

IrVariable *
ir_var_ref_get_var(IrVarValue *self);

/*---------------------------------------------------------------------------*
 *                         address of declarations                           *
 *---------------------------------------------------------------------------*/

struct IrAddressOf;

GType
ir_address_of_get_type();

IrAddressOf *
ir_address_of(void *obj);

IrExpression *
ir_address_of_get_expression(IrAddressOf *self);

/*---------------------------------------------------------------------------*
 *                       cast operation declarations                         *
 *---------------------------------------------------------------------------*/

struct IrCast;

GType
ir_cast_get_type();

IrCast *
ir_cast(void *obj);

DtDataType *
ir_cast_get_target_type(IrCast *self);

IrExpression *
ir_cast_get_value(IrCast *self);

/*---------------------------------------------------------------------------*
 *                      null expression declarations                         *
 *---------------------------------------------------------------------------*/

struct IrNull;

bool
ir_is_null(void *obj);


/*---------------------------------------------------------------------------*
 *                       binary operation declarations                       *
 *---------------------------------------------------------------------------*/

/*
 * binary operations types (defined in operations.h)
 */
enum binary_op_type
{
    or,             /* || */
    and,            /* && */
    less,           /*  <  */
    greater,        /*  >  */
    less_or_eq,     /* <=  */
    greater_or_eq,  /* >=  */
    equal,          /* ==  */
    not_equal,      /* !=  */
    plus,           /*  +  */
    minus,          /*  -  */
    mult,           /*  *  */
    division,       /*  /  */
    modulo          /*  %  */
}

struct IrBinaryOperation;

GType
ir_binary_operation_get_type();

IrBinaryOperation *
ir_binary_operation(void *obj);

binary_op_type
ir_binary_operation_get_operation(IrBinaryOperation *self);

IrExpression *
ir_binary_operation_get_left(IrBinaryOperation *self);

IrExpression *
ir_binary_operation_get_right(IrBinaryOperation *self);

/*---------------------------------------------------------------------------*
 *                           literal declarations                            *
 *---------------------------------------------------------------------------*/

struct IrLiteral;

IrLiteral *
ir_literal(void *obj);

char *
ir_literal_get_data_label(IrLiteral *self);

/*---------------------------------------------------------------------------*
 *                        basic constant declarations                        *
 *---------------------------------------------------------------------------*/

struct IrBasicConstant;

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

/*---------------------------------------------------------------------------*
 *                        array literal declarations                         *
 *---------------------------------------------------------------------------*/

struct IrArrayLiteral;

GType
ir_array_literal_get_type();

bool
ir_is_array_literal(void *obj);

IrArrayLiteral *
ir_array_literal(void *obj);

GSList *
ir_array_literal_get_values(IrArrayLiteral *self);

/*---------------------------------------------------------------------------*
 *                       struct literal declarations                         *
 *---------------------------------------------------------------------------*/

struct IrStructLiteral;

bool
ir_is_struct_literal(void *obj);

IrStructLiteral *
ir_struct_literal(void *obj);

GSList *
ir_struct_literal_get_members(IrStructLiteral *self);

/*---------------------------------------------------------------------------*
 *                        struct member declarations                         *
 *---------------------------------------------------------------------------*/

struct IrStructMember;

GType
ir_struct_member_get_type();

IrExpression *
ir_struct_member_get_base(IrStructMember *self);

IrStructMember *
ir_struct_member(void *obj);

IrExpression *
ir_struct_member_get_init(IrStructMember *self);

uint
ir_struct_member_get_padding(IrStructMember *self);

/*---------------------------------------------------------------------------*
 *                         enum member declarations                          *
 *---------------------------------------------------------------------------*/

struct IrEnumMember;


IrEnumMember *
ir_enum_member(void *obj);

IrExpression *
ir_enum_member_get_value(IrEnumMember *self);
