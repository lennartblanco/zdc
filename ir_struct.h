#ifndef IR_STRUCT_INC_X
#define IR_STRUCT_INC_X

#include "ir_node.h"
#include "dt_struct.h"
#include "sym_table.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_STRUCT ir_struct_get_type()

#define IR_STRUCT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_STRUCT, IrStruct))

#define IR_STRUCT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_STRUCT, IrStructClass))

#define IR_IS_STRUCT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_STRUCT))

#define IR_IS_STRUCT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_STRUCT))

#define IR_STRUCT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_STRUCT, IrStructClass))

typedef struct
{
    IrNode parent;

    /* private */
    DtStruct *data_type;
    gchar *name;
    GSList *members;
    sym_table_t *symbols;
} IrStruct;

typedef struct
{
    IrNodeClass parent_class;
} IrStructClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_struct_get_type(void);

IrStruct *
ir_struct_new(gchar *name, GSList *members, sym_table_t *symbols);

void
ir_struct_set_members(IrStruct *self, GSList *members);

GSList *
ir_struct_get_members(IrStruct *self);

gchar *
ir_struct_get_name(IrStruct *self);

DtStruct *
ir_struct_get_data_type(IrStruct *self);

#endif /* IR_STRUCT_INC_X */
