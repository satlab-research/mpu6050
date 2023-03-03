#ifndef MPU6050_H
#define MPU6050_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define Default_ADDR 0x68
#define ADDR1 0x69


class MPU6050
{

    public:

        i2c_inst_t* inst;
        int sda;
        int scl;

        void mpu_reset();
        bool mpu_init();
        void mpu_config();
        void mpu_read(int16_t accel[3], int16_t gyro[3]);
        void mpu_read_temp(float temp);

        MPU6050(i2c_inst_t* inst, int sda, int scl);
        ~MPU6050();

};

#endif
