/**
 * @file Status.h
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 13/11/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_STATUS_H
#define C_DATASTRUCTURES_LIBRARY_STATUS_H

#include "Core.h"

#ifdef __cplusplus
extern "C" {
#endif

const char *status_string(Status st);

void status_print(Status st);

#ifdef __cplusplus
}
#endif

#endif //C_DATASTRUCTURES_LIBRARY_STATUS_H
