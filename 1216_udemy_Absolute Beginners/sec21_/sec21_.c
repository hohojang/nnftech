//sec21
#include<stdint.h>
#include <stdio.h>

uint8_t const data = 10;

int main(void)
{

	uint8_t *ptr = (uint8_t*)&data;

	*ptr = 50;

	printf("Value = %u\n", data);

	getchar();
}
