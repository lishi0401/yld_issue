#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/i2c.h"
#include "mpu6050.h"

static const char *TAG = "MPU6050_TEST";

// I2C pins, adjust them according to your wiring.
#define I2C_MASTER_SCL_IO           GPIO_NUM_9
#define I2C_MASTER_SDA_IO           GPIO_NUM_8
#define I2C_MASTER_FREQ_HZ          100000

void app_main(void)
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &conf));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, conf.mode, 0, 0, 0));

    mpu6050_handle_t mpu6050 = mpu6050_create(I2C_NUM_0, MPU6050_I2C_ADDRESS);
    ESP_ERROR_CHECK(mpu6050_config(mpu6050, ACCE_FS_4G, GYRO_FS_500DPS));
    ESP_ERROR_CHECK(mpu6050_wake_up(mpu6050));

    ESP_LOGI(TAG, "MPU6050 initialized successfully!");

    while (1) {
        mpu6050_acce_value_t acce;
        mpu6050_gyro_value_t gyro;

        ESP_ERROR_CHECK(mpu6050_get_acce(mpu6050, &acce));
        ESP_ERROR_CHECK(mpu6050_get_gyro(mpu6050, &gyro));

        ESP_LOGI(TAG, "Accel: x=%.2f, y=%.2f, z=%.2f", acce.acce_x, acce.acce_y, acce.acce_z);
        ESP_LOGI(TAG, "Gyro:  x=%.2f, y=%.2f, z=%.2f", gyro.gyro_x, gyro.gyro_y, gyro.gyro_z);
        printf("---\n");

        vTaskDelay(pdMS_TO_TICKS(1));
    }
}
