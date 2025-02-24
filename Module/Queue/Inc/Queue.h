#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h>

#define QUEUE_SIZE 10       // 큐 크기 설정
#define MAX_PACKET_SIZE 12  // LoRa 패킷 크기 정의

typedef struct {
    uint8_t buffer[QUEUE_SIZE][MAX_PACKET_SIZE];
    int front;
    int rear;
} CircularQueue;

// 큐 초기화
void InitQueue(CircularQueue *queue);

// 큐 데이터 삽입 (Enqueue)
uint8_t Enqueue(CircularQueue *queue, uint8_t *data, uint8_t length);

// 큐 데이터 제거 (Dequeue)
uint8_t Dequeue(CircularQueue *queue, uint8_t *data);

#endif // QUEUE_H
