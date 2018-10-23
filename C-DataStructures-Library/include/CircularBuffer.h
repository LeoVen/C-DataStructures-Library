/**
 * @file CircularBuffer.h
 * 
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * @date 08/10/2018
 */

#ifndef C_DATASTRUCTURES_LIBRARY_CIRCULARBUFFER_H
#define C_DATASTRUCTURES_LIBRARY_CIRCULARBUFFER_H

#include "Core.h"

typedef struct CircularBuffer_s
{
    int *buffer;        /*!< Buffer */
    index_t size;        /*!< Current Buffer size */
    index_t capacity;    /*!< Maximum Buffer size */
    index_t start;       /*!< Where elements are removed */
    index_t end;         /*!< Where elements are added */
} CircularBuffer_t, *CircularBuffer;

Status cbf_init(CircularBuffer *cbf, index_t length);

Status cbf_insert(CircularBuffer cbf, int value);

Status cbf_remove(CircularBuffer cbf, int *value);

Status cbf_display(CircularBuffer cbf);

Status cbf_display_array(CircularBuffer cbf);

Status cbf_display_raw(CircularBuffer cbf);

Status cbf_delete(CircularBuffer *cbf);

Status cbf_erase(CircularBuffer *cbf);

index_t cbf_size(CircularBuffer cbf);

index_t cbf_capacity(CircularBuffer cbf);

bool cbf_empty(CircularBuffer cbf);

bool cbf_full(CircularBuffer cbf);

Status cbf_copy(CircularBuffer cbf, CircularBuffer *result);

#endif //C_DATASTRUCTURES_LIBRARY_CIRCULARBUFFER_H
