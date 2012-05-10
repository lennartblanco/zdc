/**
 * GType dispatcher
 */

public import GType;

import std.stdio;
import std.conv;

struct gt_dispatcher(DATA)
{
    alias void function(gt_dispatcher *, void *obj, ref DATA data) type_method;

    this(type_method[GType] type_methods)
    {
        this.type_methods = type_methods;
    }

    void
    dispatch(void *obj, ref DATA data)
    {
        assert(g_is_object(obj));
        GType obj_type = g_type_from_instance(obj);

        assert(obj_type in type_methods,
               "GType '" ~ to!string(g_type_name(obj_type)) ~ "' not listed");

        type_methods[obj_type](&this, obj, data);
    }

    private:
    type_method[GType] type_methods;
}
