#include "x86_reg_location.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
x86_reg_location_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (X86RegLocationClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (X86RegLocation),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(G_TYPE_OBJECT,
                                    "X86RegLocationType",
                                    &info, 0);
    }
    return type;
}

X86RegLocation*
x86_reg_location_new(x86_register_t reg)
{
    X86RegLocation *obj;

    obj = g_object_new(X86_TYPE_REG_LOCATION, NULL);
    obj->reg = reg;

    return obj;
}

x86_register_t
x86_reg_location_get_reg(X86RegLocation *self)
{
    assert(self);
    assert(X86_IS_REG_LOCATION(self));

    return self->reg;
}

