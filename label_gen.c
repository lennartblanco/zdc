#include <stdbool.h>
#include <string.h>

#include "label_gen.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

void
label_gen_init(label_gen_t *label_gen)
{
    strcpy(label_gen->next_label, "A");
}

void
label_gen_next(label_gen_t *label_gen,
               char *label)
{
    int i;
    char *p;

    /* copy current next label */
    strcpy(label, label_gen->next_label);

    /*
     * generate new next label
     */
    p = &(label_gen->next_label[0]);

    i = 0;
    while (true)
    {
        assert(i < LABEL_MAX_LEN);
        p[i] += 1;
        if (p[i] > 'Z' && p[i] < 'a')
        {
            p[i] = 'a';
            break;
        }
        else if (p[i] > 'z')
        {
            p[i] = 'A';
            i += 1;
            if (p[i] == '\0')
            {
                p[i] = 'A';
                p[i+1] = '\0';
                break;
            }
        }
        else
        {
            break;
        }
    }

}


