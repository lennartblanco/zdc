#ifndef IML_OPERAND_INC_X
#define IML_OPERAND_INC_X

#include <glib-object.h>
#include <stdio.h>

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

typedef enum iml_data_type_e
{
    iml_8b,
    iml_16b,
    iml_32b
} iml_data_type_t;

#define IML_TYPE_OPERAND iml_operand_get_type()

#define IML_OPERAND(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IML_TYPE_OPERAND, ImlOperand))

#define IML_OPERAND_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IML_TYPE_OPERAND, ImlOperandClass))

#define IML_IS_OPERAND(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IML_TYPE_OPERAND))

#define IML_IS_OPERAND_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IML_TYPE_OPERAND))

#define IML_OPERAND_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IML_TYPE_OPERAND, ImlOperandClass))

typedef struct 
{
    GObject parent;
} ImlOperand;

typedef struct 
{
    GObjectClass parent_class;
    /* virtual methods */
    void (*do_print) (ImlOperand *self, FILE *out, guint indention);
} ImlOperandClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
iml_operand_get_type(void);

void
iml_operand_print(ImlOperand *self, FILE *out, guint indention);

#endif /* IML_OPERAND_INC_X */
