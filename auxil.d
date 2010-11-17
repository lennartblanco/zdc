import std.c.stdio;
import GSList;
import ir_module;
import iml_func_frame;

/** see auxil.h for struct documentation */
struct arch_backend_s
{
    extern (C) {
      void function (GSList **scratch,
                     GSList **preserved)       get_registers;
      void function (iml_func_frame_t *frame)  assign_var_locations;
      void function (IrModule *ir_module,
                     FILE *out_stream,
                     const char *source_file)  gen_code;
    }
};
