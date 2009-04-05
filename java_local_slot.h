#ifndef JAVA_LOCAL_SLOT_INC_X
#define JAVA_LOCAL_SLOT_INC_X

#include <glib-object.h>

/**
 * An object for storing java backend local variable's number in function
 * frame's local variables array.
 */

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define JAVA_TYPE_LOCAL_SLOT java_local_slot_get_type()

#define JAVA_LOCAL_SLOT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), JAVA_TYPE_LOCAL_SLOT, JavaLocalSlot))

#define JAVA_LOCAL_SLOT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), JAVA_TYPE_LOCAL_SLOT, JavaLocalSlotClass))

#define JAVA_IS_LOCAL_SLOT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAVA_TYPE_LOCAL_SLOT))

#define JAVA_IS_LOCAL_SLOT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), JAVA_TYPE_LOCAL_SLOT))

#define JAVA_LOCAL_SLOT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), JAVA_TYPE_LOCAL_SLOT, JavaLocalSlotClass))

typedef struct 
{
  GObject parent;
  /* private */
  int slot_number;
} JavaLocalSlot;

typedef struct 
{
  GObjectClass parent_class;
} JavaLocalSlotClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
java_local_slot_get_type(void);

JavaLocalSlot*
java_local_slot_new(int slot_numer);

int
java_local_slot_get_number(JavaLocalSlot *self);

#endif /* JAVA_LOCAL_SLOT_INC_X */
