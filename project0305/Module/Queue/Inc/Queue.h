#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h>
#include <stdbool.h>

#define QUEUE_SIZE 10
#define MAX_PACKET_SIZE 4

typedef struct {
    uint8_t buffer[QUEUE_SIZE][MAX_PACKET_SIZE];
    uint8_t front;
    uint8_t rear;
} CircularQueue;

void InitQueue(CircularQueue *queue);
bool Enqueue(CircularQueue *queue, const uint8_t *data);
bool Dequeue(CircularQueue *queue, uint8_t *data);
bool IsQueueEmpty(CircularQueue *queue);

#endif
