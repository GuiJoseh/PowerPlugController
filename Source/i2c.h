#ifndef I2C_PROTOCOL
#define I2C_PROTOCOL

void i2c_start(void);
void i2c_stop(void);
void i2c_write(unsigned char dat);
unsigned char i2c_read(unsigned char ack);

#endif /* I2C_PROTOCOL */