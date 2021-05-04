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
#include <stdint.h>

#define DEG_TO_RAD(angleInDegrees)      ((double)(angleInDegrees) * M_PI / 180.0f)
#define RAD_TO_DEG(angleInRadians)      ((double)(angleInRadians) * 180.0f / M_PI)
#define ACC_TO_G(acceleration)          (((double)(acceleration) / 100.0f) / 9.80665f) // convert acceleration m/s² to gravity 9,80665 m/s²


extern I2C_HandleTypeDef hi2c1;

static stmdev_ctx_t dev_ctx;
static lis3dh_click_src_t src;

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

int imu_Init(void)
{
    lis3dh_ctrl_reg3_t ctrl_reg3;
    lis3dh_click_cfg_t click_cfg;

    dev_ctx.write_reg = platform_write;
    dev_ctx.read_reg = platform_read;
    dev_ctx.handle = &hi2c1;
    uint8_t whoamI;

    lis3dh_device_id_get(&dev_ctx, &whoamI);

    if (whoamI != LIS3DH_ID) {
        return -1;
    }

    /* Enable and Set full scale to 4 g. */
    lis3dh_full_scale_set(&dev_ctx, LIS3DH_4g);
    /* Set click threshold to 12h -> 0.281 g
     * 1 LSB = full scale/128 */
     lis3dh_tap_threshold_set(&dev_ctx, 0x66);

    /* Set TIME_LIMIT to 33h -> 127 ms
     * Set TIME_LATENCY to 20h -> 80 ms
     * Set TIME_WINDOW to 64h -> 250 ms
     * 1 LSB = 1/ODR ( LIS3DH_ODR_400Hz )*/
    lis3dh_shock_dur_set(&dev_ctx, 0x33);
    lis3dh_quiet_dur_set(&dev_ctx, 0x33);
    lis3dh_double_tap_timeout_set(&dev_ctx, 0x64);


    /* Enable Click interrupt on INT pin 1 */
    lis3dh_pin_int1_config_get(&dev_ctx, &ctrl_reg3);
    ctrl_reg3.i1_click = PROPERTY_ENABLE;
    lis3dh_pin_int1_config_set(&dev_ctx, &ctrl_reg3);
    lis3dh_int1_gen_duration_set(&dev_ctx, 0);

    /* Enable double click on all axis */
    lis3dh_tap_conf_get(&dev_ctx, &click_cfg);
    click_cfg.xd = PROPERTY_ENABLE;
    click_cfg.yd = PROPERTY_ENABLE;
    click_cfg.zd = PROPERTY_ENABLE;
    lis3dh_tap_conf_set(&dev_ctx, &click_cfg);

    lis3dh_tap_notification_mode_set(&dev_ctx, LIS3DH_TAP_PULSED);

    /* Set device in HR mode. */
    lis3dh_operating_mode_set(&dev_ctx, LIS3DH_HR_12bit);

    /* Set Output Data Rate.
     * The recommended accelerometer ODR for single and
     * double-click recognition is 400 Hz or higher. */
    lis3dh_data_rate_set(&dev_ctx, LIS3DH_ODR_400Hz);


    return 0;
}

void imu_Loop(void)
{
#if 0 // for debug
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
#if 0
        ULOG_TRACE("Acc X: %.0f\r\n", acceleration_mg[0]);
        ULOG_TRACE("Acc Y: %.0f\r\n", acceleration_mg[1]);
        ULOG_TRACE("Acc Z: %.0f\r\n", acceleration_mg[2]);
#endif

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
#if 0
        ULOG_TRACE("Degree X: %.1f\r\n", degree_x);
        ULOG_TRACE("Degree Y: %.1f\r\n", degree_y);
#endif
    }
#endif

    static uint32_t time = 0;

    lis3dh_tap_source_get(&dev_ctx, &src);

    if ( (src.dclick) && (HAL_GetTick() - time > 1000)) {
        ULOG_DEBUG("d-click detected : x %d, y %d, z %d, sign %d\r\n", src.x, src.y, src.z, src.sign);
        time = HAL_GetTick();
    }

}

void imu_PowerOff(void)
{
    lis3dh_data_rate_set(&dev_ctx, LIS3DH_POWER_DOWN);
}

void imu_InitWakeUp(void)
{

    lis3dh_int1_cfg_t int1_cfg;
    lis3dh_ctrl_reg3_t ctrl_reg3;
    uint8_t dummy;

    /* High-pass filter enabled on interrupt activity 1 */
    lis3dh_high_pass_int_conf_set(&dev_ctx, LIS3DH_ON_INT1_GEN);
    /* Enable HP filter for wake-up event detection
     *
     * Use this setting to remove gravity on data output */
    lis3dh_high_pass_on_outputs_set(&dev_ctx, PROPERTY_ENABLE);
    /* Enable AOI1 on int1 pin */
    lis3dh_pin_int1_config_get(&dev_ctx, &ctrl_reg3);
    ctrl_reg3.i1_ia1 = PROPERTY_ENABLE;
    lis3dh_pin_int1_config_set(&dev_ctx, &ctrl_reg3);
    /* Interrupt 1 pin latched */
    lis3dh_int1_pin_notification_mode_set(&dev_ctx,LIS3DH_INT1_PULSED);
    /* Set full scale to 2 g */
    lis3dh_full_scale_set(&dev_ctx, LIS3DH_2g);
    /* Set interrupt threshold to 0x10 -> 250 mg */
    lis3dh_int1_gen_threshold_set(&dev_ctx, 0x10);
    /* Set no time duration */
    lis3dh_int1_gen_duration_set(&dev_ctx, 0);
    /* Dummy read to force the HP filter to current acceleration value. */
    lis3dh_filter_reference_get(&dev_ctx, &dummy);
    /* Configure wake-up interrupt event on all axis */
    lis3dh_int1_gen_conf_get(&dev_ctx, &int1_cfg);
    int1_cfg.zhie = PROPERTY_ENABLE;
    int1_cfg.yhie = PROPERTY_ENABLE;
    int1_cfg.xhie = PROPERTY_ENABLE;
    int1_cfg.aoi = PROPERTY_DISABLE;
    lis3dh_int1_gen_conf_set(&dev_ctx, &int1_cfg);
    /* Set device in HR mode */
    lis3dh_operating_mode_set(&dev_ctx, LIS3DH_HR_12bit);
    /* Set Output Data Rate to 100 Hz */
    lis3dh_data_rate_set(&dev_ctx, LIS3DH_ODR_100Hz);

}
