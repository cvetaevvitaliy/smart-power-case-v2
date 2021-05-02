/**
 * @file imu.c
 * This is part of the project - Smart Power case V2 
 * Copyright (c) Vitaliy Nimych <vitaliy.nimych@gmail.com>
 * Created 28.03.2021
 * SPDX-License-Identifier: GNU General Public License v3.0
 * */

#include "imu.h"
#include "lis3dh_reg.h"
#include "stm32_init.h"
#include "cli.h"
#include "cmsis_os.h"

#define DEG_TO_RAD(angleInDegrees)      ((double)(angleInDegrees) * M_PI / 180.0f)
#define RAD_TO_DEG(angleInRadians)      ((double)(angleInRadians) * 180.0f / M_PI)
#define ACC_TO_G(acceleration)          (((double)(acceleration) / 100.0f) / 9.80665f) // convert acceleration m/s² to gravity 9,80665 m/s²


extern I2C_HandleTypeDef hi2c1;

static stmdev_ctx_t dev_ctx;

static int32_t platform_write(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len)
{
    /* Write multiple command */
    reg |= 0x80;
    HAL_I2C_Mem_Write(handle, LIS3DH_I2C_ADD_H, reg,
                      I2C_MEMADD_SIZE_8BIT, bufp, len, 1000);
    return 0;
}

static int32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len)
{
    /* Read multiple command */
    reg |= 0x80;
    HAL_I2C_Mem_Read(handle, LIS3DH_I2C_ADD_H, reg,
                     I2C_MEMADD_SIZE_8BIT, bufp, len, 1000);
    return 0;
}

int imu_init(void)
{
    lis3dh_int1_cfg_t _6d_cfg;
    lis3dh_ctrl_reg3_t ctrl_reg3;
    dev_ctx.write_reg = platform_write;
    dev_ctx.read_reg = platform_read;
    dev_ctx.handle = &hi2c1;
    uint8_t whoamI;

    lis3dh_device_id_get(&dev_ctx, &whoamI);

    if (whoamI != LIS3DH_ID) {
        return -1;
    }

    /* Set Output Data Rate in Hz */
    lis3dh_data_rate_set(&dev_ctx, LIS3DH_ODR_400Hz);

    /* Set full scale to 2 g */
    lis3dh_full_scale_set(&dev_ctx, LIS3DH_2g);

    return 0;
}

void imu_loop(void)
{
    int16_t data_raw_acceleration[3] = {0};
    float acceleration_mg[3];
    double degree_x;
    double degree_y;

    lis3dh_reg_t reg;

    /* Read output only if new value available */
    lis3dh_xl_data_ready_get(&dev_ctx, &reg.byte);

    if (reg.byte) {
        /* Read accelerometer data */
        lis3dh_acceleration_raw_get(&dev_ctx, data_raw_acceleration);

        acceleration_mg[0] = lis3dh_from_fs2_hr_to_mg(data_raw_acceleration[0]);
        acceleration_mg[1] = lis3dh_from_fs2_hr_to_mg(data_raw_acceleration[1]);
        acceleration_mg[2] = lis3dh_from_fs2_hr_to_mg(data_raw_acceleration[2]);

        ULOG_TRACE("Acc X: %.0f\r\n", acceleration_mg[0]);
        ULOG_TRACE("Acc Y: %.0f\r\n", acceleration_mg[1]);
        ULOG_TRACE("Acc Z: %.0f\r\n", acceleration_mg[2]);

        double X = ACC_TO_G(acceleration_mg[0]);
        double Y = ACC_TO_G(acceleration_mg[1]);

        double x_rms = sqrtf((float )(X * X));
        double y_rms = sqrtf((float )(Y * Y));

        if (x_rms < 1.0f)
            degree_x = RAD_TO_DEG(asinf(x_rms));
        else
            degree_x = 90.0f;

        if (y_rms < 1.0f)
            degree_y = RAD_TO_DEG(asinf(y_rms));
        else
            degree_y = 90.0f;

        ULOG_TRACE("Degree X: %.1f\r\n", degree_x);
        ULOG_TRACE("Degree Y: %.1f\r\n", degree_y);
    }

}
