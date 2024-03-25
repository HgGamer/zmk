/*
 * Copyright (c) 2023 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <zmk/mouse/input_config.h>
#include <zmk/keymap.h>

#include <zephyr/device.h>
#include <zephyr/logging/log.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#define DT_DRV_COMPAT zmk_input_configs

#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)

#define CHILD_CONFIG(inst)                                                                         \
    {                                                                                              \
        .dev = DEVICE_DT_GET(DT_PHANDLE(inst, device)),                                            \
        .xy_swap = DT_PROP(inst, xy_swap),                                                         \
        .x_invert = DT_PROP(inst, x_invert),                                                       \
        .y_invert = DT_PROP(inst, y_invert),                                                       \
        .scale_multiplier = DT_PROP(inst, scale_multiplier),                                       \
        .scale_divisor = DT_PROP(inst, scale_divisor),                                             \
        .layer_toggle = DT_PROP(inst, layer_toggle),                                               \
        .layer_toggle_delay_ms = DT_PROP(inst, layer_toggle_delay_ms),                             \
        .layer_toggle_timeout_ms = DT_PROP(inst, layer_toggle_timeout_ms),                         \
    },

#define CHILD_DATA(inst)                                                                           \
    {                                                                                              \
        .dev = DEVICE_DT_GET(DT_PHANDLE(inst, device)),                                            \
        .layer_toggle_layer_enabled = false,                                                       \
        .layer_toggle_last_mouse_package_time = 0,                                                 \
    },

const struct zmk_input_config configs[] = {DT_INST_FOREACH_CHILD(0, CHILD_CONFIG)};
struct zmk_input_config_data datas[] = {DT_INST_FOREACH_CHILD(0, CHILD_DATA)};

const struct zmk_input_config *zmk_input_config_get_for_device(const struct device *dev) {
    for (int i = 0; i < ARRAY_SIZE(configs); i++) {
        if (configs[i].dev == dev) {
            return &configs[i];
        }
    }

    return NULL;
}

struct zmk_input_config_data *zmk_input_config_data_get_for_device(const struct device *dev) {
    for (int i = 0; i < ARRAY_SIZE(datas); i++) {
        if (datas[i].dev == dev) {
            return &datas[i];
        }
    }

    return NULL;
}

void zmk_input_config_layer_toggle_input_rel_received(const struct device *dev) {
    const struct zmk_input_config *config = zmk_input_config_get_for_device(dev);

    if (config->layer_toggle == -1) {
        LOG_INF("Ignoring layer toggle mouse movement for device %p", dev);
        return;
    }

    struct zmk_input_config_data *data = zmk_input_config_data_get_for_device(dev);

    data->layer_toggle_last_mouse_package_time = k_uptime_get();

    if (data->layer_toggle_layer_enabled == false) {
        k_work_schedule(&data->layer_toggle_activation_delay,
                        K_MSEC(config->layer_toggle_delay_ms));
    } else {
        // Deactivate the layer if no further movement within
        // layer_toggle_timeout_ms
        k_work_reschedule(&data->layer_toggle_deactivation_delay,
                          K_MSEC(config->layer_toggle_timeout_ms));
    }
}

void zmk_input_config_layer_toggle_activate_layer(struct k_work *item) {

    struct k_work_delayable *d_work = k_work_delayable_from_work(item);

    struct zmk_input_config_data *data =
        CONTAINER_OF(d_work, struct zmk_input_config_data, layer_toggle_activation_delay);
    const struct zmk_input_config *config = zmk_input_config_get_for_device(data->dev);

    int64_t current_time = k_uptime_get();
    int64_t last_mv_within_ms = current_time - data->layer_toggle_last_mouse_package_time;

    if (last_mv_within_ms <= config->layer_toggle_timeout_ms * 0.1) {
        LOG_INF("Activating layer %d due to mouse activity...", config->layer_toggle);

        zmk_keymap_layer_activate(config->layer_toggle, false);
        data->layer_toggle_layer_enabled = true;
    } else {
        LOG_INF("Not activating mouse layer %d, because last mouse activity was %lldms ago",
                config->layer_toggle, last_mv_within_ms);
    }
}

void zmk_input_config_layer_toggle_deactivate_layer(struct k_work *item) {

    struct k_work_delayable *d_work = k_work_delayable_from_work(item);

    struct zmk_input_config_data *data =
        CONTAINER_OF(d_work, struct zmk_input_config_data, layer_toggle_deactivation_delay);
    const struct zmk_input_config *config = zmk_input_config_get_for_device(data->dev);

    LOG_INF("Deactivating layer %d due to mouse activity...", config->layer_toggle);

    if (zmk_keymap_layer_active(config->layer_toggle)) {
        zmk_keymap_layer_deactivate(config->layer_toggle);
    }

    data->layer_toggle_layer_enabled = false;
}

void zmk_input_config_layer_toggle_init_device(const struct device *dev) {

    struct zmk_input_config_data *data = zmk_input_config_data_get_for_device(dev);

    k_work_init_delayable(&data->layer_toggle_activation_delay,
                          zmk_input_config_layer_toggle_activate_layer);
    k_work_init_delayable(&data->layer_toggle_deactivation_delay,
                          zmk_input_config_layer_toggle_deactivate_layer);
}

static int zmk_input_config_init(void) {

    for (int i = 0; i < ARRAY_SIZE(configs); i++) {
        if (configs[i].layer_toggle != -1) {
            zmk_input_config_layer_toggle_init_device(configs[i].dev);
        }
    }

    return 0;
}

SYS_INIT(zmk_input_config_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);

#else

const struct zmk_input_config *zmk_input_config_get_for_device(const struct device *dev) {
    return NULL;
}

#endif