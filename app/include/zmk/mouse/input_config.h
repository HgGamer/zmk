/*
 * Copyright (c) 2023 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <zephyr/kernel.h>
#include <zephyr/device.h>

struct zmk_input_config {
    const struct device *dev;
    bool xy_swap;
    bool x_invert;
    bool y_invert;
    uint16_t scale_multiplier;
    uint16_t scale_divisor;
    int layer_toggle;
    int layer_toggle_delay_ms;
    int layer_toggle_timeout_ms;
};

struct zmk_input_config_data {
    const struct device *dev;
    bool layer_toggle_layer_enabled;
    int64_t layer_toggle_last_mouse_package_time;
    struct k_work_delayable layer_toggle_activation_delay;
    struct k_work_delayable layer_toggle_deactivation_delay;
};

const struct zmk_input_config *zmk_input_config_get_for_device(const struct device *dev);
struct zmk_input_config_data *zmk_input_config_data_get_for_device(const struct device *dev);

void zmk_input_config_layer_toggle_input_rel_received(const struct device *dev);