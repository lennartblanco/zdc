public import gpointer;

struct GSList
{
    gpointer data;
    GSList *_next;

    GSList *
    next()
    {
        return this._next;
    }
}

extern (C)
{
    GSList*
    g_slist_prepend(GSList *list, gpointer data);

    GSList*
    g_slist_reverse(GSList *list);

    uint
    g_slist_length(GSList *list);
}
