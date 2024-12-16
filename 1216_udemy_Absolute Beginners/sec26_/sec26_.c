//union
/*
#include<stdint.h>
#include<stdio.h>

union Address
{
	uint16_t shortAddr;
	uint32_t longAddr;
};

int main(void)
{
	union Address addr;

	addr.shortAddr = 0xABCD;
	addr.longAddr = 0xEEEECCCC;

	printf("short addr = %#X\n",addr.shortAddr);
	printf("long addr = %#X\n",addr.longAddr);

	getchar();

	return 0;
}
*/
//임베디드 시스템 코드에서 공용체의 적용 가능성 이해
#include<stdio.h>
#include<stdint.h>

union Packet
{
	uint32_t packetValue;

	struct
	{
		uint32_t crc			:2;
		uint32_t status			:1;
		uint32_t payload		:12;
		uint32_t bat			:3;
		uint32_t sensor			:3;
		uint32_t longAddr		:8;
		uint32_t shortAddr		:2;
		uint32_t addMode		:1;
	}packetFields;
};

int main(void)
{
	union Packet packet;
	printf("Enter the 32bit packet value:");
	scanf("%X", &packet.packetValue);

	printf("crc       :%#x\n",packet.packetFields.crc);
	printf("status    :%#x\n",packet.packetFields.status);
	printf("payload   :%#x\n",packet.packetFields.payload);
	printf("bat   	  :%#x\n",packet.packetFields.bat);
	printf("sensor    :%#x\n",packet.packetFields.sensor);
	printf("longAddr  :%#x\n",packet.packetFields.longAddr);
	printf("shortAddr :%#x\n",packet.packetFields.shortAddr);
	printf("addMode   :%#x\n",packet.packetFields.addMode);

	printf("Size of union is %u\n", (unsigned int)sizeof(packet));


	while(getchar()!= '\n');
	getchar();

}
