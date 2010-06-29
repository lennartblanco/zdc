module dbind.ir_function_def;

struct IrFunctionDef;

extern (C)
{
  char *
  ir_function_def_get_name(IrFunctionDef *self);

  char *
  ir_function_def_get_mangled_name(IrFunctionDef *self);
}

