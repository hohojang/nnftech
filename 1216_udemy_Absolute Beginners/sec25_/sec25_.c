/*
//구조정의
#include<stdint.h>
#include <stdio.h>

struct carModel
{
	uint32_t carNumber;
	uint32_t carPrice;
	uint16_t carMaxSpeed;
	float carWeight;
};
int main(void)
{
	struct carModel carBMW = {2021,15000,220,1330};
	printf("Sizeof of struct carModel is %I64u\n", sizeof(struct carModel));

	getchar();
	return 0;
}
//정렬 / 비 정렬 데이터 엑세스 및 구조 이해
*/
/*
#include<stdint.h>
#include<stdio.h>

struct DataSet
{
    char  data1;
    int   data2;
    char  data3;
    short data4;
};

int main(void)
{
    struct DataSet data;

    data.data1  = 0x11;
    data.data2  = 0xFFFFEEEE;
    data.data3  = 0x22;
    data.data4  = 0xABCD;

    uint8_t *ptr;

    ptr = (uint8_t*)&data;

    uint32_t totalSize = sizeof(struct DataSet);

    printf("Memory address      Content \n");
    printf("===================\n");
    for(uint32_t i = 0; i < totalSize; i++)
    {
        printf("%p       %X\n", ptr, *ptr);
        ptr++;
    }
    printf("Total memory consumed by this struct variable = %u\n", totalSize);

    getchar();
}
*/
/*
#include<stdio.h>
#include<stdint.h>


struct DataSet
{
	char  data1;
	int   data2;
	char  data3;
	short data4;
};

void displayMemberElements(struct DataSet *pdata);

int main(void)
{
	struct DataSet data;

	data.data1 = 0x11;
	data.data2 = 0xFFFFEEEE;
	data.data3 = 0x22;
	data.data4 = 0xABCD;

	displayMemberElements(&data);

	getchar();

	return 0;
}
void displayMemberElements(struct DataSet *pdata)
{
	printf("data1 = %X\n",pdata->data1);
	printf("data2 = %X\n",pdata->data2);
	printf("data3 = %X\n",pdata->data3);
	printf("data4 = %X\n",pdata->data4);
}
*/
/*
#include<stdio.h>
#include<stdint.h>

struct Packet
{
	uint8_t crc;
	uint8_t status;
	uint16_t payload;
	uint8_t bat;
	uint8_t sensor;
	uint8_t longAddr;
	uint8_t shortAddr;
	uint8_t addMode;

};

int main(void)
{
	uint32_t packetValue;
	printf("Enter the 32bit packet value:");
	scanf("%X", &packetValue);

	struct Packet packet;

	packet.crc       = (uint8_t)(packetValue & 0x3);
	packet.status    = (uint8_t)((packetValue >> 2) & 0x1);
	packet.payload   = (uint16_t)((packetValue >> 3) & 0xFFF);
	packet.bat       = (uint8_t)((packetValue >> 15) & 0x7);
	packet.sensor    = (uint8_t)((packetValue >> 18) & 0x7);
	packet.longAddr  = (uint8_t)((packetValue >> 21) & 0xFF);
	packet.shortAddr = (uint8_t)((packetValue >> 29) & 0x3);
	packet.addMode   = (uint8_t)((packetValue >> 31) & 0x1);

	printf("crc       :%#x\n",packet.crc);
	printf("status    :%#x\n",packet.status);
	printf("payload   :%#x\n",packet.payload);
	printf("bat   	  :%#x\n",packet.bat);
	printf("sensor    :%#x\n",packet.sensor);
	printf("longAddr  :%#x\n",packet.longAddr);
	printf("shortAddr :%#x\n",packet.shortAddr);
	printf("addMode   :%#x\n",packet.addMode);


	printf("Size of struct is %u\n", (unsigned int)sizeof(packet));

	while(getchar()!= '\n');
	getchar();

}
*/
//비트필드
#include<stdio.h>
#include<stdint.h>

struct Packet
{
	uint32_t crc			:2;
	uint32_t status			:1;
	uint32_t payload		:12;
	uint32_t bat			:3;
	uint32_t sensor			:3;
	uint32_t longAddr		:8;
	uint32_t shortAddr		:2;
	uint32_t addMode		:1;

};

int main(void)
{
	uint32_t packetValue;
	printf("Enter the 32bit packet value:");
	scanf("%X", &packetValue);

	struct Packet packet;

	packet.crc       = (uint8_t)(packetValue & 0x3);
	packet.status    = (uint8_t)((packetValue >> 2) & 0x1);
	packet.payload   = (uint16_t)((packetValue >> 3) & 0xFFF);
	packet.bat       = (uint8_t)((packetValue >> 15) & 0x7);
	packet.sensor    = (uint8_t)((packetValue >> 18) & 0x7);
	packet.longAddr  = (uint8_t)((packetValue >> 21) & 0xFF);
	packet.shortAddr = (uint8_t)((packetValue >> 29) & 0x3);
	packet.addMode   = (uint8_t)((packetValue >> 31) & 0x1);

	printf("crc       :%#x\n",packet.crc);
	printf("status    :%#x\n",packet.status);
	printf("payload   :%#x\n",packet.payload);
	printf("bat   	  :%#x\n",packet.bat);
	printf("sensor    :%#x\n",packet.sensor);
	printf("longAddr  :%#x\n",packet.longAddr);
	printf("shortAddr :%#x\n",packet.shortAddr);
	printf("addMode   :%#x\n",packet.addMode);


	printf("Size of struct is %u\n", (unsigned int)sizeof(packet));

	while(getchar()!= '\n');
	getchar();

}
