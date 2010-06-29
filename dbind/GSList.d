module dbind.GSList;

alias void* gpointer;

struct GSList
{
  gpointer data;
  GSList *next;
}

