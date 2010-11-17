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
}
