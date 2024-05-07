/*
 * Copyright (C) 2024 Anastasiia Stepanova <asiiapine@gmail.com>
 * Distributed under the terms of the GPL v3 license
 */

#include <string.h>

#include "periphery/i2c/i2c.hpp"
#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef HAL_I2C_MODULE_ENABLED
extern I2C_HandleTypeDef hi2c1;
#endif

#define I2C_TIMEOUT_MS 100

int8_t i2cTransmit(uint8_t id, const uint8_t tx[], uint8_t len) {
#ifdef HAL_I2C_MODULE_ENABLED
    HAL_StatusTypeDef res =
        HAL_I2C_Master_Transmit(&hi2c1, id, (uint8_t *)tx, len, I2C_TIMEOUT_MS);
    if (res == HAL_OK) {
        return res;
    }
#else
    UNUSED(id);
    UNUSED(tx);
    UNUSED(len);
#endif
    return HAL_ERROR;
}

int8_t i2cReceive(uint8_t id, uint8_t *rx, uint8_t len) {
#ifdef HAL_I2C_MODULE_ENABLED
    HAL_StatusTypeDef res =
        HAL_I2C_Master_Receive(&hi2c1, id, rx, len, I2C_TIMEOUT_MS);
    if (res == HAL_OK) {
        return res;
    }
#else
    UNUSED(id);
    UNUSED(rx);
    UNUSED(len);
#endif
    return HAL_ERROR;
}

int8_t isDeviceReady(uint8_t id, uint8_t n_trials) {
    HAL_StatusTypeDef hal_status =
        HAL_I2C_IsDeviceReady(&hi2c1, id, n_trials, I2C_TIMEOUT_MS);
    return hal_status;
}

/**
 * @brief Reads 16 bits from specified register of the i2c device
 * @param id Target device address: The device 7 bits address value
 *         in datasheet must be shifted to the left before calling the interface
 * @param mem_addr Target device register
 * @param pData Pointer to data buffer
 * @retval i2c_error_t status
 */
i2c_error_t i2cReadTwoBytesRegister(uint8_t id, uint8_t mem_addr,
                                    uint16_t *const pData) {
    uint8_t tx_buf[I2C_MEMADD_SIZE_8BIT] = {mem_addr};
    uint8_t data[2] = {0x00};

    int8_t hal_status = i2cTransmit(id, tx_buf, 1);
    if (hal_status != 0) {
        return I2C_TRANSMIT_ERROR;
    }
    hal_status = i2cReceive(id, data, 2);
    if (hal_status != HAL_OK) {
        return I2C_RECEIVE_ERROR;
    }

    *pData = (((uint16_t)data[0] << 8) | (uint16_t)data[1]);
    return I2C_SUCCESS;
}

/**
 * @brief Reads 8 bits from specified register of the i2c device
 * @param id Target device address: The device 7 bits address value
 *         in datasheet must be shifted to the left before calling the interface
 * @param mem_addr Target device register
 * @param pData Pointer to data buffer
 * @retval i2c_error_t status
 */
i2c_error_t i2cReadByteRegister(uint8_t id, uint8_t mem_addr,
                                uint8_t *const pData) {
    uint8_t tx_buf[I2C_MEMADD_SIZE_8BIT] = {0x00};
    tx_buf[0] = mem_addr;
    int8_t hal_status = i2cTransmit(id, tx_buf, I2C_MEMADD_SIZE_8BIT);
    if (hal_status != 0) {
        return I2C_TRANSMIT_ERROR;
    }

    hal_status = i2cReceive(id, pData, I2C_MEMADD_SIZE_8BIT);
    if (hal_status != 0) {
        return I2C_RECEIVE_ERROR;
    }
    return I2C_SUCCESS;
}

i2c_error_t i2cWriteBytesToRegister(uint8_t id, uint8_t reg,
                                    uint8_t const *const p_tx, size_t n_bytes) {
    if (NULL == p_tx) {
        return I2C_BAD_PARAMETER;
    }

    uint8_t buffer[n_bytes + 1];
    buffer[0] = reg;

    memcpy(&buffer[1], p_tx, n_bytes);

    int8_t hal_status = i2cTransmit(id, buffer, sizeof(buffer));

    if (hal_status != 0) {
        return I2C_TRANSMIT_ERROR;
    }
    return I2C_SUCCESS;
}
i2c_error_t i2cWriteTwoBytesToRegister(uint8_t id, uint8_t const reg,
                                       uint16_t const tx_buffer) {
    uint16_t const first_byte_mask = 0x00FF;
    size_t const count = sizeof(uint16_t);
    i2c_error_t success;
    uint8_t buffer[2];

    buffer[0] = (uint8_t)((tx_buffer >> 8) & first_byte_mask);
    buffer[1] = (uint8_t)(tx_buffer & first_byte_mask);

    success = i2cWriteBytesToRegister(id, reg, buffer, count);

    return success;
}
#ifdef __cplusplus
}
#endif
