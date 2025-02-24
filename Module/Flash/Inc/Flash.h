#ifndef FLASH_H
#define FLASH_H

#include <stdint.h>

void Flash_Write(uint32_t address, uint8_t *data, uint16_t length);
void Flash_Read(uint32_t address, uint8_t *data, uint16_t length);

#endif
