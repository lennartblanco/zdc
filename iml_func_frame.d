public import GSList;
public import iml_operand;

struct iml_func_frame;

extern (C)
{
    GSList *
    iml_func_frame_get_parameters(iml_func_frame *self);

    GSList *
    iml_func_frame_get_locals(iml_func_frame *self);

    void
    iml_func_frame_set_size(iml_func_frame *self, uint stack_size);

    uint
    iml_func_frame_get_size(iml_func_frame *self);

    GSList *
    iml_func_frame_get_used_regs(iml_func_frame *self);
}
