#include "x86_frame_offset.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
x86_frame_offset_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (X86FrameOffsetClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (X86FrameOffset),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(G_TYPE_OBJECT,
                                    "X86FrameOffsetType",
                                    &info, 0);
    }
    return type;
}

X86FrameOffset*
x86_frame_offset_new(int offset)
{
    X86FrameOffset *obj;

    obj = g_object_new(X86_TYPE_FRAME_OFFSET, NULL);
    obj->offset = offset;

    return obj;
}


int
x86_frame_offset_get_offset(X86FrameOffset *self)
{
    assert(self);
    assert(X86_IS_FRAME_OFFSET(self));

    return self->offset;
}
