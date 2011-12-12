import std.stdio;
import std.conv;
import ir;
import gt_dispatcher;

extern (C) void
ir_print_module(IrModule *mod)
{
    static gt_dispatcher *dispatcher;

    if (dispatcher == null)
    {
      dispatcher = new gt_dispatcher
       ([
         ir_module_get_type() : &print_module,
         ir_function_def_get_type() : &print_function_def,
         ir_code_block_get_type(): &print_code_block,
         ir_function_call_get_type(): &print_function_call,
         ir_return_get_type(): &print_return,
         ir_foreach_get_type(): &print_foreach,
         ir_array_literal_get_type(): &print_array_literal,
         ir_assignment_get_type(): &print_assignment,
         ir_var_value_get_type(): &print_var_value,
         ir_var_ref_get_type(): &print_var_ref,
         ir_variable_get_type(): &print_variable,
         ir_address_of_get_type(): &print_address_of,
         ir_cast_get_type(): &print_cast,
         ir_basic_constant_get_type(): &print_basic_constant
       ]);
    }

    dispatcher.dispatch(mod, null);
}

private:

struct layout
{
    string node_name;
    uint indent;
    bool[] last_node;
}

string
get_prefix(layout *l)
{
    string prefix;

    foreach (dummy; 0 .. l.indent)
    {
        prefix ~= " ";
    }

    foreach (pipe; l.last_node[0..$-1])
    {
        prefix ~= (pipe ? " " : "│") ~ "  ";
    }

    prefix ~= (l.last_node[$-1] ? "└" : "├") ~ "─ ";

    if (l.node_name != null)
    {
        prefix ~= l.node_name ~ ": ";
    }

  return prefix;
}

void
print_module(gt_dispatcher *dispatcher, void *obj, void *)
{
    IrModule *mod = ir_module(obj);

    writefln("IrModule: '%s'",
             to!string(ir_scope_get_fqname(ir_module_get_scope(mod))));

    for (GSList *i = ir_module_get_function_defs(mod); i != null; i = i.next)
    {
        dispatcher.dispatch(i.data, null);
    }
}

void
print_function_def(gt_dispatcher *dispatcher, void *obj, void *)
{
    writefln("\nIrFunctionDef: '%s'",
             to!string(ir_function_get_mangled_name(ir_function(obj))));

    dispatcher.dispatch(
        ir_function_def_get_body(ir_function_def(obj)),
                 &layout("body", 2, [true]));
}

void
print_code_block(gt_dispatcher *dispatcher, void *obj, void *data)
{
    layout *l = cast(layout*)data;
    writefln("%sIrCodeBlock", get_prefix(l));

    layout child_layout = layout(null, l.indent);
    for (GSList *i = ir_code_block_get_statments(ir_code_block(obj));
         i != null;
         i = i.next)
    {
        child_layout.last_node = l.last_node ~ [i.next == null];
        dispatcher.dispatch(i.data, &child_layout);
    }
}

void
print_function_call(gt_dispatcher *dispatcher, void *obj, void *data)
{
    layout *l = cast(layout*)data;
    IrFunctionCall *fcall = ir_function_call(obj);

    writefln("%sIrFunctionCall", get_prefix(l));

    layout cl = layout(null, l.indent);
    cl.last_node = l.last_node ~ [false];
    writefln("%sname: %s",
             get_prefix(&cl),
             to!string(ir_function_call_get_name(fcall)));

    cl.last_node = l.last_node ~ [true];
    writefln("%sarguments", get_prefix(&cl));

    for (GSList *i = ir_function_call_get_arguments(fcall);
         i != null;
         i = i.next)
    {
      cl.last_node = l.last_node ~ [true, i.next == null];
      dispatcher.dispatch(i.data, &cl);
    }
}

void
print_return(gt_dispatcher *dispatcher, void *obj, void *data)
{
    layout *l = cast(layout*)data;
    writefln("%sIrReturn", get_prefix(l));

    IrExpression *ret_val =
        ir_return_get_return_value(ir_return(obj));

    if (ret_val != null)
    {
        dispatcher.dispatch(ret_val, &layout(null, 2, l.last_node ~ [true]));
    }
}

void
print_foreach(gt_dispatcher *dispatcher, void *obj, void *data)
{
    layout *l = cast(layout*)data;
    writefln("%sIrForeach", get_prefix(l));

    layout cl = layout(null, l.indent, null);
    IrForeach *ir_foreach = ir_foreach(obj);

    /* print index node */
    IrVariable *var = ir_foreach_get_index(ir_foreach);
    if (var != null)
    {
        cl.node_name = "index";
        cl.last_node = l.last_node ~ [false];
        dispatcher.dispatch(var, &cl);
    }

    /* print value node */
    cl.node_name = "value";
    cl.last_node = l.last_node ~ [false];
    dispatcher.dispatch(ir_foreach_get_value(ir_foreach), &cl);

    /* print body subtree */
    cl.node_name = "body";
    cl.last_node = l.last_node ~ [true];
    dispatcher.dispatch(ir_foreach_get_body(ir_foreach), &cl);

}

void
print_assignment(gt_dispatcher *dispatcher, void *obj, void *data)
{
    layout *l = cast(layout*)data;
    writefln("%sIrAssignment", get_prefix(l));

    IrAssignment *assignment = ir_assignment(obj);
    layout cl;
    cl.indent = l.indent;

    /* print lvalue node */
    cl.node_name = "lvalue";
    cl.last_node = l.last_node ~ [false];
    dispatcher.dispatch(ir_assignment_get_lvalue(assignment), &cl);

    /* print rvalue node */
    cl.node_name = "value";
    cl.last_node = l.last_node ~ [true];
    dispatcher.dispatch(ir_assignment_get_value(assignment), &cl);
}

void
print_address_of(gt_dispatcher *dispatcher, void *obj, void *data)
{
    layout *l = cast(layout*)data;
    writefln("%sIrAddressOf", get_prefix(l));

    IrAddressOf *addrof = ir_address_of(obj);

    dispatcher.dispatch(ir_address_of_get_expression(addrof),
                        &layout("expression", l.indent, l.last_node ~ [true]));
}

void
print_cast(gt_dispatcher *dispatcher, void *obj, void *data)
{
    layout *l = cast(layout*)data;
    IrCast *ir_cast = ir_cast(obj);

    writefln("%sIrCast: '%s'",
             get_prefix(l),
             to!string(
                 dt_data_type_get_string(ir_cast_get_target_type(ir_cast))));

    /* print value node */
    dispatcher.dispatch(ir_cast_get_value(ir_cast),
                        &layout("value", l.indent, l.last_node ~ [true]));
}

void
print_var_value(gt_dispatcher *dispatcher, void *obj, void *data)
{
    layout *l = cast(layout*)data;
    writefln("%sIrVarValue", get_prefix(l));

    dispatcher.dispatch(ir_var_value_get_var(ir_var_value(obj)),
                        &layout("var", l.indent, l.last_node ~ [true]));
}

void
print_var_ref(gt_dispatcher *dispatcher, void *obj, void *data)
{
    layout *l = cast(layout*)data;
    writefln("%sIrVarRef", get_prefix(l));

    dispatcher.dispatch(ir_var_ref_get_var(ir_var_ref(obj)),
                        &layout("var", l.indent, l.last_node ~ [true]));
}

void
print_variable(gt_dispatcher *dispatcher, void *obj, void *data)
{
    layout *l = cast(layout*)data;
    IrVariable *var = ir_variable(obj);

    writefln("%sIrVariable '%s'",
             get_prefix(l),
             to!string(ir_variable_get_name(var)));
}

void
print_array_literal(gt_dispatcher *dispatcher, void *obj, void *data)
{
    layout *l = cast(layout*)data;
    writefln("%sIrArrayLiteral", get_prefix(l));

    layout cl = layout(null, l.indent, null);
    for (GSList *i = ir_array_literal_get_values(ir_array_literal(obj));
         i != null;
         i = i.next)
    {
        cl.last_node = l.last_node ~ [i.next == null];
        dispatcher.dispatch(i.data, &cl);
    }
}

void
print_basic_constant(gt_dispatcher *dispatcher, void *obj, void *data)
{
    string char_string(char c)
    {
        string[char] escape_chars =
            ['\a' : "\\a", '\b' : "\\b", '\f' : "\\f", '\n' : "\\n",
             '\r' : "\\r", '\t' : "\\t", '\v' : "\\v", '\0' : "\\0"];

        if (c in escape_chars)
        {
            return escape_chars[c];
        }
        return [c];
    }

    IrBasicConstant *bconst = ir_basic_constant(obj);
    DtBasic *type = dt_basic(ir_expression_get_data_type(ir_expression(obj)));

    string val;

    switch (dt_basic_get_data_type(type))
    {
        case basic_data_type.bool_type:
            val = to!string(ir_basic_constant_get_bool(bconst));
            break;
        case basic_data_type.char_type:
            val = char_string(ir_basic_constant_get_char(bconst));
            break;
        case basic_data_type.int_type:
            val = to!string(ir_basic_constant_get_int(bconst));
            break;
        case basic_data_type.uint_type:
            val = to!string(ir_basic_constant_get_uint(bconst));
            break;
        default:
            assert(false, "unexpected data type " ~
                   to!string(dt_basic_get_data_type(type)));
    }

    writefln("%sIrBasicConstant: %s", get_prefix(cast(layout*)data), val);
}
