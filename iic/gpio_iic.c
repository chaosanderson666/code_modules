#include "gpio_iic.h"

#define IIC_SCL(x)  GPIO_SET(x)
#define IIC_SDA(x)  GPIO_SET(x)
#define IIC_SDA_READ  GPIO_READ

#define IIC_DELAY(x) do { int t = x; t--} while(t);

char ack;

void gpio_iic_start(void)
{
        IIC_SDA(1);
        IIC_DELAY(1);
        IIC_SCL(1);
        IIC_DELAY(5);
        IIC_SDA(0);
        IIC_DELAY(5);
        IIC_SCL(0);
        IIC_DELAY(2);
}

void gpio_iic_stop()
{
        IIC_SDA(0);
        IIC_DELAY(1);
        IIC_SCL(1);
        IIC_DELAY(5);
        IIC_SDA(1); 
        IIC_DELAY(4);
}

void  gpio_iic_send_byte(unsigned char     c)
{
        unsigned char  i;

        for (i = 0; i < 8; i++) {
                if ((c << i) & 0x80) {
                        IIC_SDA(1);
                } else {
                        IIC_SDA(0);
                }
		
                IIC_DELAY(1);
                IIC_SCL(1);
                IIC_DELAY(5);
                IIC_SCL(0);
        }

        IIC_DELAY(2);
        IIC_SDA(1);
        IIC_DELAY(2);
        IIC_SCL(1);
        IIC_DELAY(3);
	
        if (IIC_SDA_READ()) {
                ack = 0;
        } else {
                ack = 1;
        }
	
        IIC_SCL(0);
        IIC_DELAY(2);
}

void gpio_iic_ack(char ack)
{

        if (ack == 1) {
                IIC_SDA(0);
        } else {
                IIC_SDA(1);
        }
        IIC_DELAY(3);
        IIC_SCL(1);
        IIC_DELAY(5);


        IIC_SCL(0);
        IIC_DELAY(2);
}

unsigned char gpio_iic_read_byte(char read_ack)
{
        unsigned char  retc;
        unsigned char  i;

        retc = 0;
        IIC_SDA(1);
	
        for (i = 0; i < 8; i++) {
                IIC_DELAY(1);
                IIC_SCL(0);
                IIC_DELAY(5);
                IIC_SCL(1);
                IIC_DELAY(2);
                retc = retc << 1;
		
                if (IIC_SDA_READ()) {
			retc = retc + 1;
		}
		
                IIC_DELAY(2);
        }
	
        IIC_SCL(0);
        IIC_DELAY(2);
        gpio_iic_ack(read_ack);
        return (retc);
}


