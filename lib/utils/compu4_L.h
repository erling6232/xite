#ifndef COMPUTE

#define COMPUTE 1

#include <xite/compu4.h>

#define TY_UN -1
#define TY_BF 0
#define TY_BT 1
#define TY_CI 2
#define TY_CD 3
#define TY_CS 4
#define TY_BO 5
#define TY_CP 6
#define TY_VR 7
#define TY_CF 8
#define TY_CL 9
#define TY_CU 10


#define E_OK  0
#define E_TOK 1
#define E_ARG 2
#define E_OP  3
#define E_LP  4
#define E_EOL 5
#define E_RP  6
#define E_TYP 7
#define E_LEN 9
#define E_EQ  10
#define E_VAR 11

typedef char *argarr[];
typedef func funcarr[8];

#define INT_0 (INTEG) (0)
#define INT_1 (INTEG) (1)

#define i_get(arg)      ((arg) -> i_value)
#define d_get(arg)      ((arg) -> d_value)
#define f_get(arg)      ((arg) -> f_value)
#define p_get(arg)      ((arg) -> p_value)
#define t_get(arg)      ((arg) -> u_type)
#define i_set(arg,val)  ((arg) -> i_value = (val))
#define d_set(arg,val)  ((arg) -> d_value = (val))
#define f_set(arg,val)  ((arg) -> f_value = (val))
#define p_set(arg,val)  ((arg) -> p_value = (val))
#define t_set(arg,val)  ((arg) -> u_type = (val))

#endif
