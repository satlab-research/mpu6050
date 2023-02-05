#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "MPU6050.h"


MPU6050::MPU6050(i2c_inst_t *inst, int sda, int scl)
{
    this->inst = inst;
    this->sda = sda;
    this->scl = scl;

    // I2C Initialisation. Using it at 400 Khz.
    i2c_init(this->inst, 400*1000);
    // I2C Operating frequency for MPU6050: 400 KHz(All registers, Fast-mode), 100 KHz(All registers, Standard-mode)
    
    gpio_set_function(this->sda, GPIO_FUNC_I2C);
    gpio_set_function(this->scl, GPIO_FUNC_I2C);
    gpio_pull_up(this->sda); // To keep line HIGH when no data is online.
    gpio_pull_up(this->scl); // To keep line HIGH when no data is online.
}

MPU6050::~MPU6050()
{
}

void MPU6050::mpu_reset()
{
    uint8_t buf[] = {0x6B, 0x00};
    i2c_write_blocking(this->inst, ADDR1, buf, 2, false);
}

bool MPU6050::mpu_init()
{
    uint8_t val = 0x75;
    uint8_t buf[1];
    buf[0] = 0;
    i2c_write_blocking(this->inst, ADDR1, &val, 1, true); // true to keep master control of bus
    i2c_read_blocking(this->inst, ADDR1, buf, 1, false);

    if( buf[0] != 104 )
    {
        printf("Check MPU Connections!\n");
        return 0;
    }

    return 1;
}

void MPU6050::mpu_config()
{
    uint8_t buf1[] = {0x1A, 0x00};
    i2c_write_blocking(this->inst, ADDR1, buf1, 2, false);

    uint8_t buf2[] = {0x1B, 0x00};
    i2c_write_blocking(this->inst, ADDR1, buf2, 2, false);

    uint8_t buf3[] = {0x1C, 0x00};
    i2c_write_blocking(this->inst, ADDR1, buf3, 2, false);

}


void MPU6050::mpu_read(int16_t accel[3], int16_t gyro[3])
{
    uint8_t buffer[6];

    // Start reading acceleration registers from register 0x3B for 6 bytes
    
    uint8_t val = 0x3B;
    i2c_write_blocking(this->inst, ADDR1, &val, 1, true); // true to keep master control of bus
    i2c_read_blocking(this->inst, ADDR1, buffer, 6, false);

    for (int i = 0; i < 3; i++) {
        accel[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);
    }

    // Now gyro data from reg 0x43 for 6 bytes
    // The register is auto incrementing on each read
    
    val = 0x43;
    i2c_write_blocking(this->inst, ADDR1, &val, 1, true);
    i2c_read_blocking(this->inst, ADDR1, buffer, 6, false);  // False - finished with bus

    for (int i = 0; i < 3; i++) {
        gyro[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);
    }
}

void MPU6050::mpu_read_temp(float temp)
{
    uint8_t tem[2];    
    uint8_t val = 0x41;

    i2c_write_blocking(this->inst, ADDR1, &val, 1, true);
    i2c_read_blocking(this->inst, ADDR1, tem, 2, false);  // False - finished with bus
    
    temp = (tem[0]<<8|tem[1])/340 + 36.53;
    
}

