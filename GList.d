public import gpointer;

struct GList
{
    gpointer data;
    GList *_next;
    GList *_prev;

    GList *
    next()
    {
        return this._next;
    }
}

extern (C)
{
    void
    g_list_free(GList *list);
}