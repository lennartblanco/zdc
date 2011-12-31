/**
 * Operation types enumerations.
 */

#ifndef OPERATIONS_INC_X
#define OPERATIONS_INC_X

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

/*
 * binary operations types
 */
typedef enum binary_op_type_e
{
    op_or,             /* || */
    op_and,            /* && */
    op_less,           /*  <  */
    op_greater,        /*  >  */
    op_less_or_eq,     /* <=  */
    op_greater_or_eq,  /* >=  */
    op_equal,          /* ==  */
    op_not_equal,      /* !=  */
    op_plus,           /*  +  */
    op_minus,          /*  -  */
    op_mult,           /*  *  */
    op_division,       /*  /  */
    op_modulo          /*  %  */
} binary_op_type_t;

/*
 * unary operations types
 */
typedef enum unary_op_type_e
{
    op_arithm_neg,     /*  -(exp)  */
    op_bool_neg,       /*  !(exp)  */
    op_pre_inc,        /*  ++(exp) */
    op_pre_dec,        /*  --(exp) */
    op_post_inc,       /* (exp)++ */
    op_post_dec        /* (exp)-- */
} unary_op_type_t;


#endif /* OPERATIONS_INC_X */
