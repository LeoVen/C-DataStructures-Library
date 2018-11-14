/**
 * @file Status.c
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 13/11/2018
 */

#include "Core.h"

const char *status_string(Status st)
{
    switch (st)
    {
        case DS_OK:
            return "DS_OK";
        case DS_ERR_OUT_OF_RANGE:
            return "DS_ERR_OUT_OF_RANGE";
        case DS_ERR_INVALID_OPERATION:
            return "DS_ERR_INVALID_OPERATION";
        case DS_ERR_NEGATIVE_VALUE:
            return "DS_ERR_NEGATIVE_VALUE";
        case DS_ERR_ALLOC:
            return "DS_ERR_ALLOC";
        case DS_ERR_UNEXPECTED_RESULT:
            return "DS_ERR_UNEXPECTED_RESULT";
        case DS_ERR_NULL_POINTER:
            return "DS_ERR_NULL_POINTER";
        case DS_ERR_FULL:
            return "DS_ERR_FULL";
        case DS_ERR_INVALID_ARGUMENT:
            return "DS_ERR_INVALID_ARGUMENT";
        case DS_ERR_NOT_FOUND:
            return "DS_ERR_NOT_FOUND";
        case DS_ERR_INCOMPLETE_TYPE:
            return "DS_ERR_INCOMPLETE_TYPE";
        case DS_ERR_ITER:
            return "DS_ERR_ITER";
        case DS_ERR_ITER_MODIFICATION:
            return "DS_ERR_ITER_MODIFICATION";
        case DS_ERR_ITER_STATE:
            return "DS_ERR_ITER_STATE";
        default:
            return "UNKNOWN";
    }
}

void status_print(Status st)
{
    printf("%s\n", status_string(st));
}