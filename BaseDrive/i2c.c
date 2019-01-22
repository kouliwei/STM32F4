/******************** (C) COPYRIGHT  Դ�ع����� ********************************
 * �ļ���  ��i2c.c
 * ����    ����ʼ��IIC��һЩ�����Ĳ���
 * ����    ��zhuoyingxingyu
 * �Ա�    ��Դ�ع�����http://vcc-gnd.taobao.com/
 * ��̳��ַ������԰��-Ƕ��ʽ������̳http://vcc-gnd.com/
 * �汾����: 2015-02-20
 * Ӳ������: SCL--PB6,SDA--PB7 
 * ���Է�ʽ��J-Link-OB
 ******************************************************************************/	

//ͷ�ļ�
#include "i2c.h"
#include "stdio.h"
//#include "usmart.h"
//=============================================================================
//��������:I2C_Configuration
//���ܸ�Ҫ:EEPROM�ܽ�����
//����˵��:��
//��������:��
//=============================================================================
void I2C_Configuration(void)
{
        GPIO_InitTypeDef  GPIO_InitStructure; 
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��
        /* Configure I2C2 pins: PB6->SCL and PB7->SDA */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//
        GPIO_Init(GPIOB, &GPIO_InitStructure);//
        SDA_H;
        SCL_H;
}

//=============================================================================
//��������:I2C_delay
//���ܸ�Ҫ:�ӳ�ʱ��
//����˵��:��
//��������:��
//=============================================================================
void I2C_delay(u16 nCount)
{	
        while(nCount--)
        {
        }
}

//=============================================================================
//��������:I2C_delay
//���ܸ�Ҫ:�ӳ�ʱ��
//����˵��:��
//��������:��
//=============================================================================
void I2C_delayms(u16 nCount)
{	
        while(nCount--)
        {
                I2C_delay(10000);
        }

}

//=============================================================================
//��������:I2C_Start
//���ܸ�Ҫ:SCL�ߵ�ƽʱ,SDA�иߵ�ƽ��Ϊ�͵�ƽ,����IIC��ʼ�ź�
//����˵��:��
//��������:��
//=============================================================================
void I2C_Start(void)
{
        SDA_OUT();     //sda�����,scl=H,SDA  1-->0;
        SDA_H;
        SCL_H;
        I2C_delay(70);//START:when CLK is high,DATA change form high to low 
        SDA_L;
        I2C_delay(70);
        SCL_L;
        I2C_delay(70);
}

//=============================================================================
//��������:I2C_Stop
//���ܸ�Ҫ:SCL�ߵ�ƽʱ��SDA�е͵�ƽ��Ϊ�ߵ�ƽʱ��Ϊֹͣ�ź�
//����˵��:��
//��������:��
//=============================================================================
void I2C_Stop(void)
{
        SDA_OUT();//sda�����
        SCL_L;
        I2C_delay(70);//STOP:when CLK is high DATA change form low to high
        SDA_L;
        I2C_delay(70);
        SCL_H;
        I2C_delay(70);
        SDA_H;
        I2C_delay(70);
}

//=============================================================================
//��������:I2C_Ack
//���ܸ�Ҫ:����ACKӦ��
//����˵��:��
//��������:��
//=============================================================================
void I2C_Ack(void)
{	
        SCL_L;
        SDA_OUT();
        I2C_delay(70);
        SDA_L;
        I2C_delay(70);
        SCL_H;
        I2C_delay(70);
        SCL_L;
        I2C_delay(70);
}

//=============================================================================
//��������:I2C_NoAck	    
//���ܸ�Ҫ:������ACKӦ��		    
//����˵��:��
//��������:��
//=============================================================================
void I2C_NoAck(void)
{	
        SCL_L;
        SDA_OUT();
        I2C_delay(70);
        SDA_H;
        I2C_delay(70);
        SCL_H;
        I2C_delay(70);
        SCL_L;
        I2C_delay(70);
}

//=============================================================================
//��������:I2C_WaitAck
//���ܸ�Ҫ:�ȴ�Ӧ���źŵ���
//����˵��:��
//��������:0������Ӧ��ʧ��
//         1������Ӧ��ɹ�
//=============================================================================
uint8_t I2C_WaitAck(void) 	
{
        u8 ucErrTime=0;
        SDA_IN();      //SDA����Ϊ���� 
        SDA_H;
        I2C_delay(70);
        SCL_H;
        I2C_delay(10);
        while(SDA_read)
        {
                //printf("%d",SDA_read);
                ucErrTime++;
                if(ucErrTime>200)
                {
                        I2C_Stop();
                        return 0;
                }
        }
        SCL_L;
        I2C_delay(70);
        return 1;
}

//=============================================================================
//��������:I2C_SendByte		    
//���ܸ�Ҫ:�������������дһ���ֽڵ�����,���ݴӸ�λ����λ		    
//����˵��:���͵�����
//��������:��
//=============================================================================
void I2C_SendByte(uint8_t SendByte) 
{
        uint8_t i=8;
        SDA_OUT();
        SCL_L;
        while(i--)
        {
                SCL_L;
                I2C_delay(70);
                if(SendByte&0x80)
                        SDA_H;  
                else 
                        SDA_L;   
                SendByte<<=1;
                I2C_delay(70);
                SCL_H;
                I2C_delay(70);
        }
        SCL_L;
}

//=============================================================================
//��������:I2C_ReceiveByte	    
//���ܸ�Ҫ:�������Ӵ�������ȡһ���ֽڵ�����,���ݴӸ�λ����λ	    
//����˵��:��
//��������:I2C���߷��ص�����:receive
//=============================================================================	
uint8_t I2C_ReceiveByte(void)  
{ 
        uint8_t i=8;
        uint8_t ReceiveByte=0;
        SDA_IN();  //SDA����Ϊ����
        SDA_H;	   			
        while(i--)
        {
                ReceiveByte<<=1;      
                SCL_L;
                I2C_delay(70);
                SCL_H;
                I2C_delay(70);	
                if(SDA_read)
                {
                        ReceiveByte|=0x01;
                }
        }
        SCL_L;
        return ReceiveByte;
}

//=============================================================================
//��������:I2C_WriteOneByte    
//���ܸ�Ҫ:��ָ����ַд��һ������	    
//����˵��:- DataToWrite: ��д������
//         - WriteAddr: ��д���ַ
//��������:����Ϊ:=1�ɹ�д��,=0ʧ��
//=============================================================================	
uint8_t I2C_WriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite)
{		
        I2C_Start();
        I2C_SendByte(0xA0); 
        I2C_WaitAck();
        I2C_SendByte((uint8_t)(WriteAddr & 0x00FF));   /* ���õ���ʼ��ַ */      
        I2C_WaitAck();	
        I2C_SendByte(DataToWrite);
        I2C_WaitAck();   
        I2C_Stop(); 
        I2C_delayms(10);
        /* ע�⣺��Ϊ����Ҫ�ȴ�EEPROMд�꣬���Բ��ò�ѯ����ʱ��ʽ(10ms)	*/
        /* Systick_Delay_1ms(10); */
        return 1;
}									  

//=============================================================================
//��������:I2C_ReadOneByte    
//���ܸ�Ҫ:��ָ����ַ����һ������    
//����˵��: ReadAddress: �����ĵ�ַ 
//��������:����������
//=============================================================================	
uint8_t I2C_ReadOneByte(uint16_t ReadAddress)
{		
        uint8_t temp=0;
        I2C_Start();
        I2C_SendByte(0xA0); //����д����
        I2C_WaitAck();
        I2C_SendByte((uint8_t)(ReadAddress & 0x00FF));   /* ���õ���ʼ��ַ */      
        I2C_WaitAck();
        I2C_Start();
        I2C_SendByte(0xA1);//�������ģʽ	
        I2C_WaitAck();
        temp=I2C_ReceiveByte();
        I2C_Stop();
        return temp;
}


//=============================================================================
//��������:I2C_WriteLenByte    
//���ܸ�Ҫ:��ָ����ַ��ʼд��ָ������������    
//����˵��:
//         WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
//         pBuffer   :���������׵�ַ
//         NumToWrite:Ҫд�����ݵĸ���
//��������:��
//=============================================================================	
void I2C_WriteLenByte(uint16_t WriteAddr, uint8_t *pBuffer, uint16_t NumToWrite)
{
        uint16_t i;	
        for(i=WriteAddr;i<NumToWrite;i++)
        {
                I2C_WriteOneByte(i,pBuffer[i]);
        }
        //while(!I2C_WriteOneByte(i,pBuffer[i]));	
}									  

//=============================================================================
//��������:I2C_ReadLenByte    
//���ܸ�Ҫ:��ȡһ������	    
//����˵��: - ReadAddr: ��������ַ
//          - pBuffer: ��Ŷ������ݵ�������׵�ַ
//          - NumToRead:����������
//��������:��
//=============================================================================	
void I2C_ReadLenByte(uint16_t ReadAddr, uint8_t *pBuffer,uint16_t NumToRead)
{	
        while(NumToRead)
        {
                *pBuffer++=I2C_ReadOneByte(ReadAddr++);	
                NumToRead--;
        }
}

uint8_t I2C_Test(void)
{
        uint8_t WriteBuffer[256],ReadBuffer[256];
        uint16_t i;
        for(i=0; i<256; i++)
        {
                WriteBuffer[i]=i;	 
        }
        I2C_WriteLenByte(0, WriteBuffer, 256);
        for(i=0; i<1000; i++)
        {
                ;		
        }
        I2C_ReadLenByte(0, ReadBuffer, 256);
        if(  memcmp(WriteBuffer,ReadBuffer,sizeof(WriteBuffer)) == 0 ) /* ƥ������ */
                return 1;
        else
                return 0;

}
/*********************************************************************************************************
  END FILE
 *********************************************************************************************************/











