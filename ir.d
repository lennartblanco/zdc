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
    char *
    ir_function_get_mangled_name(IrFunction *self);
}

/*---------------------------------------------------------------------------*
 *                      function definition declarations                     *
 *---------------------------------------------------------------------------*/

struct IrFunctionDef;

extern (C)
{
    char *
    ir_function_def_get_name(IrFunctionDef *self);

    GSList *
    ir_function_def_get_parameters(IrFunctionDef *self);

    DtDataType *
    ir_function_def_get_return_type(IrFunctionDef *self);

    iml_func_frame *
    ir_function_def_get_frame(IrFunctionDef *self);

    GSList *
    ir_function_def_get_operations(IrFunctionDef *self);
}

/*---------------------------------------------------------------------------*
 *                          expression declarations                          *
 *---------------------------------------------------------------------------*/

struct IrExpression;

extern (C)
{
    DtDataType *
    ir_expression_get_data_type(IrExpression *self);
}

/*---------------------------------------------------------------------------*
 *                           literal declarations                            *
 *---------------------------------------------------------------------------*/

struct IrLiteral;

extern (C)
{
    char *
    ir_literal_get_data_label(IrLiteral *self);
}

/*---------------------------------------------------------------------------*
 *                        basic constant declarations                        *
 *---------------------------------------------------------------------------*/

struct IrBasicConstant;

extern (C)
{
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
    bool
    ir_is_array_literal(void *obj);

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

    GSList *
    ir_struct_literal_get_members(IrStructLiteral *self);
}

/*---------------------------------------------------------------------------*
 *                        struct member declarations                         *
 *---------------------------------------------------------------------------*/

struct IrStructMember;

extern (C)
{
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
    IrExpression *
    ir_enum_member_get_value(IrEnumMember *self);
}


