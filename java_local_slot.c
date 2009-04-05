#include "java_local_slot.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
java_local_slot_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (JavaLocalSlotClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (JavaLocalSlot),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(G_TYPE_OBJECT,
                                    "JavaLocalSlotType",
                                    &info, 0);
    }
    return type;
}

JavaLocalSlot*
java_local_slot_new(int slot_number)
{
    JavaLocalSlot *obj;

    obj = g_object_new(JAVA_TYPE_LOCAL_SLOT, NULL);
    obj->slot_number = slot_number;

    return obj;
}


int
java_local_slot_get_number(JavaLocalSlot *self)
{
    assert(self);
    assert(JAVA_IS_LOCAL_SLOT(self));

    return self->slot_number;
}
