public import GSList;
public import iml_operand;

struct iml_func_frame_t;

extern (C)
{
    GSList *
    iml_func_frame_get_parameters(iml_func_frame_t *self);

    GSList *
    iml_func_frame_get_locals(iml_func_frame_t *self,
                              iml_data_type_t vars_type);

    void
    iml_func_frame_set_size(iml_func_frame_t *self, uint stack_size);

    uint
    iml_func_frame_get_size(iml_func_frame_t *self);

    GSList *
    iml_func_frame_get_used_regs(iml_func_frame_t *self);
}
