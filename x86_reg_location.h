/**
 * An object for storing in which register an x86 backend variable is stored.
 */

#ifndef X86_REG_LOCATION_INC_X
#define X86_REG_LOCATION_INC_X

#include <glib-object.h>


/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define X86_TYPE_REG_LOCATION x86_reg_location_get_type()

#define X86_REG_LOCATION(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), X86_TYPE_REG_LOCATION, X86RegLocation))

#define X86_REG_LOCATION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), X86_TYPE_REG_LOCATION, X86RegLocationClass))

#define X86_IS_REG_LOCATION(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), X86_TYPE_REG_LOCATION))

#define X86_IS_REG_LOCATION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), X86_TYPE_REG_LOCATION))

#define X86_REG_LOCATION_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), X86_TYPE_REG_LOCATION, X86RegLocationClass))

/*
 * x86 register used for storing variables
 */
typedef enum x86_register_e
{
    eax_reg,
    ebx_reg,
    ecx_reg,
    edx_reg
} x86_register_t;


typedef struct 
{
  GObject parent;
  /* private */
  x86_register_t reg;
} X86RegLocation;

typedef struct 
{
  GObjectClass parent_class;
} X86RegLocationClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
x86_reg_location_get_type(void);

X86RegLocation*
x86_reg_location_new(x86_register_t reg);

x86_register_t
x86_reg_location_get_reg(X86RegLocation *self);

#endif /* X86_REG_LOCATION_INC_X */
