alias void* gpointer;

struct GSList
{
    gpointer data;
    GSList *next;
}

extern (C)
{
    GSList*
    g_slist_prepend(GSList *list, gpointer data);

    uint
    g_slist_length(GSList *list);
}

GSList *
g_slist_next(GSList *l)
{
    return l.next;
}
