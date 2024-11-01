/*
 * Copyright (c) 2020 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#define DT_DRV_COMPAT zmk_behavior_to_layer

#include <zephyr/device.h>
#include <drivers/behavior.h>
#include <zephyr/logging/log.h>

#include <zmk/keymap.h>
#include <zmk/behavior.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)

struct behavior_to_layer_config {
    int32_t ignored_layers_len;
    int8_t ignored_layers[];
};

static int behavior_to_init(const struct device *dev) { return 0; };

static bool check_if_layer_is_ignored(const struct behavior_to_layer_config *cfg, int8_t layer) {
    for (int k = 0; k < cfg->ignored_layers_len; k++) {
        if (layer == cfg->ignored_layers[k])
            return true;
    }

    return false;
}

static int to_keymap_binding_pressed(struct zmk_behavior_binding *binding,
                                     struct zmk_behavior_binding_event event) {
    const struct device *dev = zmk_behavior_get_binding(binding->behavior_dev);
    const struct behavior_to_layer_config *cfg = dev->config;
    int8_t layer = binding->param1;

    LOG_DBG("position %d layer %d", event.position, binding->param1);

    for (int i = 0; i < ZMK_KEYMAP_LAYERS_LEN; i++) {
        if (!check_if_layer_is_ignored(cfg, i))
            zmk_keymap_layer_deactivate(i);
        else
            LOG_DBG("ignored layer %d (%s)", i, zmk_keymap_layer_name(i));
    }

    zmk_keymap_layer_activate(layer);

    // zmk_keymap_layer_to(binding->param1);
    return ZMK_BEHAVIOR_OPAQUE;
}

static int to_keymap_binding_released(struct zmk_behavior_binding *binding,
                                      struct zmk_behavior_binding_event event) {
    LOG_DBG("position %d layer %d", event.position, binding->param1);
    return ZMK_BEHAVIOR_OPAQUE;
}

#if IS_ENABLED(CONFIG_ZMK_BEHAVIOR_METADATA)

static const struct behavior_parameter_value_metadata param_values[] = {
    {
        .display_name = "Layer",
        .type = BEHAVIOR_PARAMETER_VALUE_TYPE_LAYER_ID,
    },
};

static const struct behavior_parameter_metadata_set param_metadata_set[] = {{
    .param1_values = param_values,
    .param1_values_len = ARRAY_SIZE(param_values),
}};

static const struct behavior_parameter_metadata metadata = {
    .sets_len = ARRAY_SIZE(param_metadata_set),
    .sets = param_metadata_set,
};

#endif

static const struct behavior_driver_api behavior_to_driver_api = {
    .binding_pressed = to_keymap_binding_pressed,
    .binding_released = to_keymap_binding_released,
#if IS_ENABLED(CONFIG_ZMK_BEHAVIOR_METADATA)
    .parameter_metadata = &metadata,
#endif // IS_ENABLED(CONFIG_ZMK_BEHAVIOR_METADATA)
};

#define KP_INST(n)                                                                                 \
    static struct behavior_to_layer_config behavior_to_layer_config_##n = {                        \
        .ignored_layers = DT_INST_PROP_OR(n, ignore_layers, {0}),                                  \
        .ignored_layers_len = DT_INST_PROP_LEN_OR(n, ignore_layers, 0),                            \
    };                                                                                             \
    BEHAVIOR_DT_INST_DEFINE(n, behavior_to_init, NULL, NULL, &behavior_to_layer_config_##n,        \
                            POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,                      \
                            &behavior_to_driver_api);

DT_INST_FOREACH_STATUS_OKAY(KP_INST)
// BEHAVIOR_DT_INST_DEFINE(0, behavior_to_init, NULL, NULL, NULL, POST_KERNEL,
// CONFIG_KERNEL_INIT_PRIORITY_DEFAULT, &behavior_to_driver_api);

#endif /* DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT) */
