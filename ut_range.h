#ifndef UT_RANGE_INC_X
#define UT_RANGE_INC_X

#include <glib-object.h>

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define UT_TYPE_RANGE ut_range_get_type()

#define UT_RANGE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), UT_TYPE_RANGE, UtRange))

#define UT_RANGE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), UT_TYPE_RANGE, UtRangeClass))

#define UT_IS_RANGE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), UT_TYPE_RANGE))

#define UT_IS_RANGE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), UT_TYPE_RANGE))

#define UT_RANGE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), UT_TYPE_RANGE, UtRangeClass))

typedef struct
{
    GObject parent;

    /* private */
    gint64 min;
    gint64 max;
} UtRange;

typedef struct
{
    GObjectClass parent_class;
} UtRangeClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ut_range_get_type(void);

UtRange *
ut_range_new(gint64 min, gint64 max);

/**
 * @return true if self range includes completely the 'range' range
 */
bool
ut_range_includes(UtRange *self, UtRange *range);


#endif /* UT_VALUE_RANGE_INC_X */
