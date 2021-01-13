#include <stdio.h>
#include <stdlib.h>


int mask_modefail_gen_en 	= (1 << 17);
int mask_man_start_com		= (1 << 16);
int mask_man_start_en		= (1 << 15);
int mask_manual_cs			= (1 << 14);
int mask_CS					= (0B1111 << 10);
int mask_peri_sel			= (1 << 9);
int mask_ref_clk			= (1 << 8);
int mask_baud_rate_div		= (0B111 << 3);
int mask_cpha				= (1 << 2);
int mask_cpol				= (1 << 1);
int mask_mode_sel			= (1 << 0);

void printf_bin(int num);

int main(){
    int a = 0b10101010101010101010101010101010;
//    int a = 0b11111111111111111111111111111111;
    int b = 0b00101010101010101010101010101010;

    printf("a = %d\n", a);
    printf("b = %d\n", b);
    
/*    printf("a/b = %d\n", a/b);*/
    
    int tmp = 0b1;

/*    
    for( int i=0; i<32; i++ ){
//    	int mask = (tmp << i);
		int mask = (1 << i);
    	int val = a & mask;
//    	printf("I = %3d, val = %16d\n", i, val);

		printf("I = %3d\t",i);
    	printf_bin(val);
    }
*/  
    
    int tmp_mask = mask_modefail_gen_en | mask_man_start_com | mask_man_start_en;
    
    printf_bin(tmp_mask);
    
    return 0;
}


void printf_bin(int num)
{
    int i, j, k;
    unsigned char *p = (unsigned char*)&num + 3;//p��ָ��num�����3���ֽڵĵ�ַ����num�����λ�ֽڵ�ַ

    for (i = 0; i < 4; i++) //���δ���4���ֽ�(32λ��
    {
        j = *(p - i); //ȡÿ���ֽڵ��׵�ַ���Ӹ�λ�ֽڵ���λ�ֽڣ���p p-1 p-2 p-3��ַ��
        for (int k = 7; k >= 0; k--) //����ÿ���ֽڵ�8��λ��ע���ֽ��ڲ��Ķ��������ǰ����˵�ϰ�ߴ洢��
        {
            if (j & (1 << k))//1����kλ���뵥ǰ���ֽ�����j���л����㣬��k=7ʱ��00000000&10000000=0 ->���ֽڵ����λΪ0
                printf("1");
            else
                printf("0");
        }
        printf(" ");//ÿ8λ�Ӹ��ո񣬷���鿴
    }
    printf("\r\n");
}
