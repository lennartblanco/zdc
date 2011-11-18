import std.c.stdio;
import ir;
import iml;

/** see auxil.h for struct documentation */
struct arch_backend
{
    extern (C) {
      void function (GSList **scratch,
                     GSList **preserved)       get_registers;
      void function (iml_func_frame *frame,
                     ir_linkage_type linkage) assign_var_locations;
      void function (IrModule *ir_module,
                     FILE *out_stream,
                     const char *source_file)  gen_code;
    }
};
