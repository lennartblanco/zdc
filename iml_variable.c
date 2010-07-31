#include <glib.h>

#include "iml_variable.h"
#include "utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

struct iml_variable_s
{
  iml_data_type_t datatype;
};


/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

iml_variable_t *
iml_variable_new(iml_data_type_t data_type)
{
  iml_variable_t *var = g_malloc(sizeof(*var));

  var->datatype = data_type;

  return var;
}

iml_data_type_t
iml_variable_get_data_type(iml_variable_t *self)
{
    assert(self);

    return self->datatype;
}

void
iml_variable_print(iml_variable_t *self, FILE *out, int indention)
{
    char *type_str;

    switch (self->datatype)
    {
        case iml_8b:
            type_str = " 8b";
            break;
        case iml_16b:
            type_str = "16b";
            break;
        case iml_32b:
            type_str = "32b";
            break;
        default:
            /* unexpected data type */
            assert(FALSE);
    }

    fprintf_indent(out, indention, "%s [%p]\n", type_str, self);
}
