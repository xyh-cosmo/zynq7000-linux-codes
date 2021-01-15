///////////////////////////////////////////////////////////////////////
// =======================
// A0、A1解码器
// A0  A1  Y0  Y1  Y2  Y3
// 0   0   0    1   1   1
// 0   1   1    0   1   1
// 1   0   1    1   0   1
// 1   1   1    1   1   0
// =======================

// 参考资料：
// UG585
// https://www.cnblogs.com/yhpbook/p/11041197.html
// https://blog.csdn.net/little_soldier/article/details/102698926
// https://bestfpga.blog.csdn.net/article/details/88932544
// https://blog.csdn.net/obanaganastar/article/details/83781646

// 待解决问题：
// 1）如何实现A0、A1的配置？需要对什么寄存器进行配置？
///////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// #include <string.h>
// #include <unistd.h>
// #include <termios.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "xparameters.h"
#include "xspips.h"
#include "xil_printf.h"
#include "sleep.h"

#include "xyh.h"

#define SpiPs_RecvByte(BaseAddress) (u8)XSpiPs_In32((BaseAddress) + XSPIPS_RXD_OFFSET)

#define SpiPs_SendByte(BaseAddress, Data) XSpiPs_Out32((BaseAddress) + XSPIPS_TXD_OFFSET, (Data))

int spi0_init();
void spi0_one_write();
void SpiRead(int ByteCount);
void SpiWrite(u8 *Sendbuffer, int ByteCount);

// 定义SPI设备对应的结构体变量
XSpiPs Spi0, Spi1;

unsigned char ReadBuffer[1024];
unsigned char WriteBuffer[1024]={1,2,3,4,5,6,7,8,9,0};


int main(){

	int Status;
	
	// 通过设置A0、A0实现chip selection:
	// 见原理图U27A。
	


	// 初始化SPI设备，并进行自检。
	Status = spi0_init();
	if (Status != XST_SUCCESS) {
		xil_printf("SPI Selftest Example Failed\r\n");
		return XST_FAILURE;
	}

	// 准备SPI“数据=COMMADN+ADDR+DATA”
	// "X X X X C3 C2 C1 C0 A3 A2 A1 A0 D11 D10 D9 D8 D7 D6 D5 D4 D3 D2 D1 D0 X X X X X X X X"
	u32 buffer = 0x0;
	
	// 准备 Command bits
	u32 cmd_tmp = 0b0000;
/*	cmd_tmp = (cmd_tmp << 24);*/
/*	buffer |= cmd_tmp;*/
	
	// 准备 Address bits
	u32 add_tmp = 0b0000;
/*	add_tmp = (add_tmp << 20);*/
/*	buffer |= add_tmp;*/
	
	// 准备 Data bits
	u32 dat_tmp = pow(2,6);
/*	dat_tmp = (dat_tmp << 8);*/
/*	buffer |= dat_tmp;*/
	
	buffer = (cmd_tmp << 24) + (add_tmp << 20 ) + (dat_tmp << 8 );
	
	// 发送数据至SPI Slave Device
/*	XSpiPs_Transfer(XSpiPs *InstancePtr, u8 *SendBufPtr, u8 *RecvBufPtr, u32 ByteCount)*/

}

int spi0_init() {
	int Status;
	XSpiPs_Config *SpiConfig;

	/*
	 * Initialize the SPI device.
	 */

	SpiConfig = XSpiPs_LookupConfig(XPAR_XSPIPS_0_DEVICE_ID);
	if (NULL == SpiConfig) {
		return XST_FAILURE;
	}

	// 此处需要修改BaseAddress
	u32 BaseAddress = SpiConfig->BaseAddress;
	u32 fd = open("/dev/mem", O_RDWR | O_SYNC);
	u32 size = XPAR_PS7_SPI_0_HIGHADDR - XPAR_PS7_SPI_0_BASEADDR;
	u32* ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, BaseAddress);
	// SpiConfig->BaseAddress = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, BaseAddress);
	SpiConfig->BaseAddress = ptr;
	
	printf("--> BaseAddress = 0x%08x\r\n", BaseAddress);
	printf("--> size = %d\r\n", size);


	printf("--> debug -- 1\r\n");
	printf("SpiConfig->BaseAddress = 0x%08x\r\n", SpiConfig->BaseAddress);
	
	Status = XSpiPs_CfgInitialize(&Spi0, SpiConfig, SpiConfig->BaseAddress);

	if (Status != XST_SUCCESS) {
		printf("DEBUG:: %s ==> Status == XST_SUCCESS! exit now ...\r\n", __func__);
		return XST_FAILURE;
	}

	printf("@debug in %s: SpiConfig->BaseAddress = 0x%08x\r\n", __func__, SpiConfig->BaseAddress);

	/*
	 * Perform a self-test to check hardware build.
	 */

	printf("--> debug -- 2\r\n");
	Status = XSpiPs_SelfTest(&Spi0);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	printf("%s self test succ\r\n", __func__);
	return 0;

	printf("--> debug -- 3\r\n");
	Status = XSpiPs_SetOptions(&Spi0, XSPIPS_MASTER_OPTION);
	if (Status != XST_SUCCESS) {
		xil_printf("%s XSpiPs_SetOptions fail\n", __func__);
		return XST_FAILURE;
	}

	printf("--> debug -- 4\r\n");
	Status = XSpiPs_SetClkPrescaler(&Spi0, XSPIPS_CLK_PRESCALE_64);
	if (Status != XST_SUCCESS) {
		xil_printf("%s XSpiPs_SetClkPrescaler fail\n", __func__);
		return XST_FAILURE;
	}

	printf("--> debug -- 5\r\n");
	XSpiPs_Enable(&Spi0);
	debug_info("spi 0 config finish\n");

	munmap(ptr, size);
	return XST_SUCCESS;
}

void SpiRead(int ByteCount)
{
	int Count;
	u32 StatusReg;

	StatusReg = XSpiPs_ReadReg( Spi0.Config.BaseAddress, XSPIPS_SR_OFFSET );

	/*
	 * Polling the Rx Buffer for Data
	 */
	do{
		StatusReg = XSpiPs_ReadReg( Spi0.Config.BaseAddress, XSPIPS_SR_OFFSET );
	}while(!(StatusReg & XSPIPS_IXR_RXNEMPTY_MASK));

	/*
	 * Reading the Rx Buffer
	 */
	for(Count = 0; Count < ByteCount; Count++){
		ReadBuffer[Count] = SpiPs_RecvByte( Spi0.Config.BaseAddress);
	}

}

void SpiWrite(u8 *Sendbuffer, int ByteCount)
{
	u32 StatusReg;
	int TransCount = 0;

	StatusReg = XSpiPs_ReadReg( Spi0.Config.BaseAddress, XSPIPS_SR_OFFSET );

	while ((ByteCount > 0) &&
		(TransCount < XSPIPS_FIFO_DEPTH)) {
		SpiPs_SendByte( Spi0.Config.BaseAddress, *Sendbuffer );
		Sendbuffer++;
		++TransCount;
		ByteCount--;
	}

	/*
	 * Wait for the transfer to finish by polling Tx fifo status.
	 */
	do {
		StatusReg = XSpiPs_ReadReg( Spi0.Config.BaseAddress, XSPIPS_SR_OFFSET );
	} while ((StatusReg & XSPIPS_IXR_TXOW_MASK) == 0);

}
