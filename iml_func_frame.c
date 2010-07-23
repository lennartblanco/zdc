#include <glib.h>

#include "iml_func_frame.h"
#include "utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

struct iml_func_frame_s
{
  GSList *parameters;
};

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

iml_func_frame_t *
iml_func_frame_new(void)
{
    iml_func_frame_t *frame;

    frame = g_malloc0(sizeof(*frame));

    return frame;
}

void
iml_func_frame_add_local(iml_func_frame_t *self, iml_variable_t *variable)
{
    /* not imlemented */
    assert(FALSE);
}

void
iml_func_frame_add_parameter(iml_func_frame_t *self, iml_variable_t *variable)
{
    assert(self);
    assert(variable);

    self->parameters = g_slist_append(self->parameters, variable);
}

void
iml_func_frame_print(iml_func_frame_t *self, FILE *out, int indention)
{
    GSList *i;

    if (self->parameters != NULL)
    {
        fprintf_indent(out, indention + 2, "parameters\n");
        for (i = self->parameters; i != NULL; i = g_slist_next(i))
        {
            iml_variable_print(i->data, out, indention + 4);
        }
    }
}
