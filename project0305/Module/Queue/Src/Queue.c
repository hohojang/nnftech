#include "../../Module/Queue/Inc/Queue.h"
#include <string.h>

void InitQueue(CircularQueue *queue) {
    queue->front = 0;
    queue->rear = 0;
}

bool Enqueue(CircularQueue *queue, const uint8_t *data) {
    if ((queue->rear + 1) % QUEUE_SIZE == queue->front) return false;
    memcpy(queue->buffer[queue->rear], data, MAX_PACKET_SIZE);
    queue->rear = (queue->rear + 1) % QUEUE_SIZE;
    return true;
}

bool Dequeue(CircularQueue *queue, uint8_t *data) {
    if (IsQueueEmpty(queue)) return false;
    memcpy(data, queue->buffer[queue->front], MAX_PACKET_SIZE);
    queue->front = (queue->front + 1) % QUEUE_SIZE;
    return true;
}

bool IsQueueEmpty(CircularQueue *queue) {
    return queue->front == queue->rear;
}
