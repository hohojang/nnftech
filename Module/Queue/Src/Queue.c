#include "../../Module/Queue/Inc/Queue.h"
#include <string.h>

/**
 * @brief 큐 초기화
 */
void InitQueue(CircularQueue *queue) {
    queue->front = 0;
    queue->rear = 0;
}

/**
 * @brief 데이터 삽입 (Enqueue)
 */
uint8_t Enqueue(CircularQueue *queue, uint8_t *data, uint8_t length) {
    if ((queue->rear + 1) % QUEUE_SIZE == queue->front) {
        return 0; // 큐가 가득 찬 경우
    }
    memcpy(queue->buffer[queue->rear], data, length);
    queue->rear = (queue->rear + 1) % QUEUE_SIZE;
    return 1;
}

/**
 * @brief 데이터 제거 (Dequeue)
 */
uint8_t Dequeue(CircularQueue *queue, uint8_t *data) {
    if (queue->front == queue->rear) {
        return 0; // 큐가 비어있는 경우
    }
    memcpy(data, queue->buffer[queue->front], MAX_PACKET_SIZE);
    queue->front = (queue->front + 1) % QUEUE_SIZE;
    return 1;
}
