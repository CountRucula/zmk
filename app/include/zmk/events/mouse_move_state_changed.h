
/*
 * Copyright (c) 2020 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <zephyr/kernel.h>
#include <zmk/event_manager.h>
#include <zmk/mouse.h>

struct zmk_mouse_move_state_changed {
    struct vector2d max_speed;
    struct mouse_config config;
    bool state;
    int64_t timestamp;
};

ZMK_EVENT_DECLARE(zmk_mouse_move_state_changed);

static inline int raise_zmk_mouse_move_state_changed_from_encoded(uint32_t encoded,
                                                                  struct mouse_config config,
                                                                  bool pressed, int64_t timestamp) {
    struct vector2d max_speed = (struct vector2d){
        .x = MOVE_HOR_DECODE(encoded),
        .y = MOVE_VERT_DECODE(encoded),
    };

    return raise_zmk_mouse_move_state_changed((struct zmk_mouse_move_state_changed){
        .max_speed = max_speed, .config = config, .state = pressed, .timestamp = timestamp});
}
