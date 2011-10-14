#include <stdbool.h>

#include "ut_range.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ut_range_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (UtRangeClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (UtRange),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(G_TYPE_OBJECT,
                                    "UtRangeType",
                                    &info, 0);
    }
    return type;
}

UtRange *
ut_range_new(gint64 min, gint64 max)
{
    UtRange *obj;

    obj = g_object_new(UT_TYPE_RANGE, NULL);

    obj->min = min;
    obj->max = max;

    return obj;
}

bool
ut_range_includes(UtRange *self, UtRange *range)
{
    assert(UT_IS_RANGE(self));
    assert(UT_IS_RANGE(range));

    return self->min <= range->min && range->max <= self->max;
}
