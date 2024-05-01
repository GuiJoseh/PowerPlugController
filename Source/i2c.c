#include <8051.h>
#include "i2c.h"
#include "time.h"

#define SCL P1_0 // Pino de rel�gio (SCL)
#define SDA P1_1 // Pino de dados (SDA)

void i2c_start(void) {
    SDA = 1;
    SCL = 1;
    delay(5);
    SDA = 0;
    delay(5);
    SCL = 0;
}

void i2c_stop(void) {
    SDA = 0;
    SCL = 1;
    delay(5);
    SDA = 1;
    delay(5);
}

void i2c_write(unsigned char dat) {
    unsigned char i;
    for (i = 0; i < 8; i++) {
        SDA = (dat & 0x80) ? 1 : 0;
        SCL = 1;
        delay(5);
        SCL = 0;
				delay(5);
        dat <<= 1;
    }
    SDA = 1;  // Libera o pino SDA
    SCL = 1;
    delay(5);
    SCL = 0;
		delay(5);
}

unsigned char i2c_read(unsigned char ack) {
    unsigned char i, dat = 0;
    SDA = 1;
    for (i = 0; i < 8; i++) {
        dat <<= 1;
        SCL = 1;
        delay(5);
        dat |= SDA;
        SCL = 0;
				delay(5);
    }
    SDA = ack ? 0 : 1;  // Envia ACK ou NACK
    SCL = 1;
    delay(5);
    SCL = 0;
		delay(5);
    SDA = 1;  // Libera o pino SDA
    return dat;
}