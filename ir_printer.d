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
         ir_basic_constant_get_type(): &print_basic_constant
       ]);
    }

    dispatcher.dispatch(g_object(mod), null);
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
print_module(gt_dispatcher *dispatcher, GObject *obj, void *)
{
    IrModule *mod = ir_module(obj);

    writefln("IrModule: '%s'",
             to!string(ir_module_get_fqname(mod)));

    for (GSList *i = ir_module_get_function_defs(mod); i != null; i = i.next)
    {
        dispatcher.dispatch(g_object(i.data), null);
    }
}

void
print_function_def(gt_dispatcher *dispatcher, GObject *obj, void *)
{
    writefln("\nIrFunctionDef: '%s'",
             to!string(ir_function_get_mangled_name(ir_function(obj))));

    dispatcher.dispatch(
        g_object(ir_function_def_get_body(ir_function_def(obj))),
                 &layout("body", 2, [true]));
}

void
print_code_block(gt_dispatcher *dispatcher, GObject *obj, void *data)
{
    layout *l = cast(layout*)data;
    writefln("%sIrCodeBlock", get_prefix(l));

    layout child_layout = layout(null, l.indent);
    for (GSList *i = ir_code_block_get_statments(ir_code_block(obj));
         i != null;
         i = i.next)
    {
        child_layout.last_node = l.last_node ~ [i.next == null];
        dispatcher.dispatch(g_object(i.data), &child_layout);
    }
}

void
print_function_call(gt_dispatcher *dispatcher, GObject *obj, void *data)
{
    layout *l = cast(layout*)data;
    IrFunctionCall *fcall = ir_function_call(obj);

    writefln("%sIrFunctionCall", get_prefix(l));

    layout child_layout = layout(null, l.indent);
    child_layout.last_node = l.last_node ~ [false];
    writefln("%sname: %s",
             get_prefix(&child_layout),
             to!string(ir_function_call_get_name(fcall)));
}

void
print_return(gt_dispatcher *dispatcher, GObject *obj, void *data)
{
    layout *l = cast(layout*)data;
    writefln("%sIrReturn", get_prefix(l));

    IrExpression *ret_val =
        ir_return_get_return_value(ir_return(obj));

    if (ret_val != null)
    {
        dispatcher.dispatch(g_object(ret_val),
                            &layout(null, 2, l.last_node ~ [true]));
    }
}

void
print_foreach(gt_dispatcher *dispatcher, GObject *obj, void *data)
{
    layout *l = cast(layout*)data;
    writefln("%sIrForeach", get_prefix(l));
}

void
print_basic_constant(gt_dispatcher *dispatcher, GObject *obj, void *data)
{
    IrBasicConstant *bconst = ir_basic_constant(obj);
    DtBasic *type = dt_basic(ir_expression_get_data_type(ir_expression(obj)));

    string val;

    switch (dt_basic_get_data_type(type))
    {
        case basic_data_type.bool_type:
            val = to!string(ir_basic_constant_get_bool(bconst));
            break;
        case basic_data_type.char_type:
            val = to!string(ir_basic_constant_get_char(bconst));
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
