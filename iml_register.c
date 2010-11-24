#include <glib.h>
#include <stdbool.h>

#include "iml_register.h"
#include "utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

struct iml_register_s
{
    guint id;
    gchar *name;
};


/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

iml_register_t *
iml_register_new(guint id, const gchar *name)
{
    iml_register_t *reg;

    reg = g_malloc(sizeof(*reg));
    reg->id = id;
    reg->name = g_strdup(name);

    return reg;
}

guint
iml_register_get_id(iml_register_t *self)
{
    assert(self);

    return self->id;
}

const gchar *
iml_register_get_name(iml_register_t *self)
{
    assert(self);

    return self->name;
}

void
iml_register_print(iml_register_t *self, FILE *out, int indention)
{
    assert(self);

    fprintf_indent(out, indention, "%s", self->name);
}
