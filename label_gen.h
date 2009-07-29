#ifndef LABEL_GEN_INC_X
#define LABEL_GEN_INC_X

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define LABEL_MAX_LEN 16

typedef struct label_gen_s 
{
    char next_label[LABEL_MAX_LEN];
} label_gen_t;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

void
label_gen_init(label_gen_t *label_gen);

/**
 * Get next unique label
 *
 * @param label the generated label is into this buffer,
                this buffer must be at least LABEL_MAX_LEN bytes long
 */
void
label_gen_next(label_gen_t *label_gen,
               char *label);


#endif /* LABEL_GEN_INC_X */

