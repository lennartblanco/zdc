#ifndef X86_FRAME_OFFSET_INC_X
#define X86_FRAME_OFFSET_INC_X

#include <glib-object.h>

/**
 * An object for storing x86 local variable's offset relative to functions
 * frame offset pointer (%ebp register)
 */

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define X86_TYPE_FRAME_OFFSET x86_frame_offset_get_type()

#define X86_FRAME_OFFSET(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), X86_TYPE_FRAME_OFFSET, X86FrameOffset))

#define X86_FRAME_OFFSET_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), X86_TYPE_FRAME_OFFSET, X86FrameOffsetClass))

#define X86_IS_FRAME_OFFSET(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), X86_TYPE_FRAME_OFFSET))

#define X86_IS_FRAME_OFFSET_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), X86_TYPE_FRAME_OFFSET))

#define X86_FRAME_OFFSET_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), X86_TYPE_FRAME_OFFSET, X86FrameOffsetClass))

typedef struct 
{
  GObject parent;
  /* private */
  int offset;
} X86FrameOffset;

typedef struct 
{
  GObjectClass parent_class;
} X86FrameOffsetClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
x86_frame_offset_get_type(void);

X86FrameOffset*
x86_frame_offset_new(int offset);

int
x86_frame_offset_get_offset(X86FrameOffset *self);

#endif /* X86_FRAME_OFFSET_INC_X */
