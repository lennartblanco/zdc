#ifndef IML_CONSTANT_INC_X
#define IML_CONSTANT_INC_X

#include "iml_operand.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IML_TYPE_CONSTANT iml_constant_get_type()

#define IML_CONSTANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IML_TYPE_CONSTANT, ImlConstantClass))

#define IML_IS_CONSTANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IML_TYPE_CONSTANT))

#define IML_CONSTANT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IML_TYPE_CONSTANT, ImlConstantClass))

typedef struct 
{
    ImlOperand parent;
    /* private */
    iml_data_type_t datatype;
    union
    {
      guint8 v8;
      guint16 v16;
      guint32 v32;
      const gchar *ptr_label;
    } value;
} ImlConstant;

typedef struct 
{
    ImlOperandClass parent_class;
} ImlConstantClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
iml_constant_get_type(void);

bool
iml_is_constant(void *obj);

ImlConstant *
iml_constant(void *obj);

ImlConstant *
iml_constant_new_8b(guint8 val);

ImlConstant *
iml_constant_new_16b(guint16 val);

ImlConstant *
iml_constant_new_32b(guint32 val);

ImlConstant *
iml_constant_new_ptr(const gchar *label);

/**
 * Get singleton object for a constant used to represent boolean true value.
 *
 * @return singleton 8-bit '1' constant.
 */
ImlConstant *
iml_constant_true();

/**
 * Get singleton object for a 32-bit '0' constant.
 */
ImlConstant *
iml_constant_zero_32b();

/**
 * Get singleton object for a 32-bit '1' constant.
 */
ImlConstant *
iml_constant_one_32b();

guint8
iml_constant_get_val_8b(ImlConstant *self);

guint32
iml_constant_get_val_32b(ImlConstant *self);

const gchar *
iml_constant_get_val_ptr(ImlConstant *self);

#endif /* IML_CONSTANT_INC_X */
