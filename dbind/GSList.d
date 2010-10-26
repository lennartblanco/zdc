module dbind.GSList;

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
}

