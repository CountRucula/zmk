/*
 * Copyright (c) 2020 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <zephyr/sys/util.h>

#include <zephyr/usb/usb_device.h>
#include <zephyr/usb/class/usb_hid.h>

#include <zmk/keys.h>
#include <zmk/mouse.h>
#include <dt-bindings/zmk/hid_usage.h>
#include <dt-bindings/zmk/hid_usage_pages.h>

#if IS_ENABLED(CONFIG_ZMK_HID_KEYBOARD_NKRO_EXTENDED_REPORT)
#define ZMK_HID_KEYBOARD_NKRO_MAX_USAGE HID_USAGE_KEY_KEYBOARD_LANG8
#else
#define ZMK_HID_KEYBOARD_NKRO_MAX_USAGE HID_USAGE_KEY_KEYPAD_EQUAL
#endif

#if IS_ENABLED(CONFIG_ZMK_HID_CONSUMER_REPORT_USAGES_BASIC)
#define ZMK_HID_CONSUMER_MAX_USAGE 0xFF
#elif IS_ENABLED(CONFIG_ZMK_HID_CONSUMER_REPORT_USAGES_FULL)
#define ZMK_HID_CONSUMER_MAX_USAGE 0xFFF
#else
#error "Unknown consumer report usages configuration"
#endif

#if IS_ENABLED(CONFIG_ZMK_HID_REPORT_TYPE_NKRO)
#define ZMK_HID_KEYBOARD_MAX_USAGE ZMK_HID_KEYBOARD_NKRO_MAX_USAGE
#elif IS_ENABLED(CONFIG_ZMK_HID_REPORT_TYPE_HKRO)
#define ZMK_HID_KEYBOARD_MAX_USAGE 0xFF
#else
#error "Unknown keyboard report usages configuration"
#endif

#define ZMK_HID_MOUSE_NUM_BUTTONS 0x05

// See https://www.usb.org/sites/default/files/hid1_11.pdf section 6.2.2.4 Main Items

#define ZMK_HID_MAIN_VAL_DATA (0x00 << 0)
#define ZMK_HID_MAIN_VAL_CONST (0x01 << 0)

#define ZMK_HID_MAIN_VAL_ARRAY (0x00 << 1)
#define ZMK_HID_MAIN_VAL_VAR (0x01 << 1)

#define ZMK_HID_MAIN_VAL_ABS (0x00 << 2)
#define ZMK_HID_MAIN_VAL_REL (0x01 << 2)

#define ZMK_HID_MAIN_VAL_NO_WRAP (0x00 << 3)
#define ZMK_HID_MAIN_VAL_WRAP (0x01 << 3)

#define ZMK_HID_MAIN_VAL_LIN (0x00 << 4)
#define ZMK_HID_MAIN_VAL_NON_LIN (0x01 << 4)

#define ZMK_HID_MAIN_VAL_PREFERRED (0x00 << 5)
#define ZMK_HID_MAIN_VAL_NO_PREFERRED (0x01 << 5)

#define ZMK_HID_MAIN_VAL_NO_NULL (0x00 << 6)
#define ZMK_HID_MAIN_VAL_NULL (0x01 << 6)

#define ZMK_HID_MAIN_VAL_NON_VOL (0x00 << 7)
#define ZMK_HID_MAIN_VAL_VOL (0x01 << 7)

#define ZMK_HID_MAIN_VAL_BIT_FIELD (0x00 << 8)
#define ZMK_HID_MAIN_VAL_BUFFERED_BYTES (0x01 << 8)

#define ZMK_HID_REPORT_ID_KEYBOARD 0x01
#define ZMK_HID_REPORT_ID_LEDS 0x01
#define ZMK_HID_REPORT_ID_CONSUMER 0x02
#define ZMK_HID_REPORT_ID_MOUSE 0x03

static const uint8_t zmk_hid_report_desc[] = {
    HID_USAGE_PAGE(HID_USAGE_GEN_DESKTOP),
    HID_USAGE(HID_USAGE_GD_KEYBOARD),
    HID_COLLECTION(HID_COLLECTION_APPLICATION),
    HID_REPORT_ID(ZMK_HID_REPORT_ID_KEYBOARD),
    HID_USAGE_PAGE(HID_USAGE_KEY),
    HID_USAGE_MIN8(HID_USAGE_KEY_KEYBOARD_LEFTCONTROL),
    HID_USAGE_MAX8(HID_USAGE_KEY_KEYBOARD_RIGHT_GUI),
    HID_LOGICAL_MIN8(0x00),
    HID_LOGICAL_MAX8(0x01),

    HID_REPORT_SIZE(0x01),
    HID_REPORT_COUNT(0x08),
    HID_INPUT(ZMK_HID_MAIN_VAL_DATA | ZMK_HID_MAIN_VAL_VAR | ZMK_HID_MAIN_VAL_ABS),

    HID_USAGE_PAGE(HID_USAGE_KEY),
    HID_REPORT_SIZE(0x08),
    HID_REPORT_COUNT(0x01),
    HID_INPUT(ZMK_HID_MAIN_VAL_CONST | ZMK_HID_MAIN_VAL_VAR | ZMK_HID_MAIN_VAL_ABS),

#if IS_ENABLED(CONFIG_ZMK_HID_INDICATORS)

    HID_USAGE_PAGE(HID_USAGE_LED),
    HID_USAGE_MIN8(HID_USAGE_LED_NUM_LOCK),
    HID_USAGE_MAX8(HID_USAGE_LED_KANA),
    HID_REPORT_SIZE(0x01),
    HID_REPORT_COUNT(0x05),
    HID_OUTPUT(ZMK_HID_MAIN_VAL_DATA | ZMK_HID_MAIN_VAL_VAR | ZMK_HID_MAIN_VAL_ABS),

    HID_USAGE_PAGE(HID_USAGE_LED),
    HID_REPORT_SIZE(0x03),
    HID_REPORT_COUNT(0x01),
    HID_OUTPUT(ZMK_HID_MAIN_VAL_CONST | ZMK_HID_MAIN_VAL_VAR | ZMK_HID_MAIN_VAL_ABS),

#endif // IS_ENABLED(CONFIG_ZMK_HID_INDICATORS)

    HID_USAGE_PAGE(HID_USAGE_KEY),

#if IS_ENABLED(CONFIG_ZMK_HID_REPORT_TYPE_NKRO)
    HID_LOGICAL_MIN8(0x00),
    HID_LOGICAL_MAX8(0x01),
    HID_USAGE_MIN8(0x00),
    HID_USAGE_MAX8(ZMK_HID_KEYBOARD_NKRO_MAX_USAGE),
    HID_REPORT_SIZE(0x01),
    HID_REPORT_COUNT(ZMK_HID_KEYBOARD_NKRO_MAX_USAGE + 1),
    HID_INPUT(ZMK_HID_MAIN_VAL_DATA | ZMK_HID_MAIN_VAL_VAR | ZMK_HID_MAIN_VAL_ABS),
#elif IS_ENABLED(CONFIG_ZMK_HID_REPORT_TYPE_HKRO)
    HID_LOGICAL_MIN8(0x00),
    HID_LOGICAL_MAX16(0xFF, 0x00),
    HID_USAGE_MIN8(0x00),
    HID_USAGE_MAX8(0xFF),
    HID_REPORT_SIZE(0x08),
    HID_REPORT_COUNT(CONFIG_ZMK_HID_KEYBOARD_REPORT_SIZE),
    HID_INPUT(ZMK_HID_MAIN_VAL_DATA | ZMK_HID_MAIN_VAL_ARRAY | ZMK_HID_MAIN_VAL_ABS),
#else
#error "A proper HID report type must be selected"
#endif

    HID_END_COLLECTION,
    HID_USAGE_PAGE(HID_USAGE_CONSUMER),
    HID_USAGE(HID_USAGE_CONSUMER_CONSUMER_CONTROL),
    HID_COLLECTION(HID_COLLECTION_APPLICATION),
    HID_REPORT_ID(ZMK_HID_REPORT_ID_CONSUMER),
    HID_USAGE_PAGE(HID_USAGE_CONSUMER),

#if IS_ENABLED(CONFIG_ZMK_HID_CONSUMER_REPORT_USAGES_BASIC)
    HID_LOGICAL_MIN8(0x00),
    HID_LOGICAL_MAX16(0xFF, 0x00),
    HID_USAGE_MIN8(0x00),
    HID_USAGE_MAX8(0xFF),
    HID_REPORT_SIZE(0x08),
#elif IS_ENABLED(CONFIG_ZMK_HID_CONSUMER_REPORT_USAGES_FULL)
    HID_LOGICAL_MIN8(0x00),
    HID_LOGICAL_MAX16(0xFF, 0x0F),
    HID_USAGE_MIN8(0x00),
    HID_USAGE_MAX16(0xFF, 0x0F),
    HID_REPORT_SIZE(0x10),
#else
#error "A proper consumer HID report usage range must be selected"
#endif
    /* REPORT_COUNT (CONFIG_ZMK_HID_CONSUMER_REPORT_SIZE) */
    HID_REPORT_COUNT(CONFIG_ZMK_HID_CONSUMER_REPORT_SIZE),
    HID_INPUT(0x00),
    /* END COLLECTION */
    HID_END_COLLECTION,

#if IS_ENABLED(CONFIG_ZMK_MOUSE)
    /* USAGE_PAGE (Generic Desktop) */
    HID_USAGE_PAGE(HID_USAGE_GD),
    /* USAGE (Mouse) */
    HID_USAGE(HID_USAGE_GD_MOUSE),
    /* COLLECTION (Application) */
    HID_COLLECTION(HID_COLLECTION_APPLICATION),
    /* REPORT ID (4) */
    HID_REPORT_ID(0x04),
    /* USAGE (Pointer) */
    HID_USAGE(HID_USAGE_GD_POINTER),
    /* COLLECTION (Physical) */
    HID_COLLECTION(HID_COLLECTION_PHYSICAL),
    /* USAGE_PAGE (Button) */
    HID_USAGE_PAGE(HID_USAGE_BUTTON),
    /* USAGE_MINIMUM (0x1) (button 1?) */
    HID_USAGE_MIN8(0x01),
    /* USAGE_MAXIMUM (0x10) (button 5? Buttons up to 8 still work) */
    HID_USAGE_MAX8(0x10),
    /* LOGICAL_MINIMUM (0) */
    HID_LOGICAL_MIN8(0x00),
    /* LOGICAL_MAXIMUM (1) */
    HID_LOGICAL_MAX8(0x01),
    /* REPORT_SIZE (1) */
    HID_REPORT_SIZE(0x01),
    /* REPORT_COUNT (16) */
    HID_REPORT_COUNT(0x10),
    /* INPUT (Data,Var,Abs) */
    HID_INPUT(0x02),
    /* USAGE_PAGE (Generic Desktop) */
    HID_USAGE_PAGE(HID_USAGE_GD),
    /* LOGICAL_MINIMUM (-32767) */
    HID_LOGICAL_MIN16(0x01, 0x80),
    /* LOGICAL_MAXIMUM (32767) */
    HID_LOGICAL_MAX16(0xFF, 0x7F),
    /* REPORT_SIZE (16) */
    HID_REPORT_SIZE(0x10),
    /* REPORT_COUNT (2) */
    HID_REPORT_COUNT(0x02),
    /* USAGE (X) */ // Vertical scroll
    HID_USAGE(HID_USAGE_GD_X),
    /* USAGE (Y) */
    HID_USAGE(HID_USAGE_GD_Y),
    /* Input (Data,Var,Rel) */
    HID_INPUT(0x06),
    /* LOGICAL_MINIMUM (-127) */
    HID_LOGICAL_MIN8(0x81),
    /* LOGICAL_MAXIMUM (127) */
    HID_LOGICAL_MAX8(0x7F),
    /* REPORT_SIZE (8) */
    HID_REPORT_SIZE(0x08),
    /* REPORT_COUNT (1) */
    HID_REPORT_COUNT(0x01),
    /* USAGE (Wheel) */
    HID_USAGE(HID_USAGE_GD_WHEEL),
    /* Input (Data,Var,Rel) */
    HID_INPUT(0x06),
    /* USAGE_PAGE (Consumer) */ // Horizontal scroll
    HID_USAGE_PAGE(HID_USAGE_CONSUMER),
    /* USAGE (AC Pan) */
    0x0A,
    0x38,
    0x02,
    /* LOGICAL_MINIMUM (-127) */
    HID_LOGICAL_MIN8(0x81),
    /* LOGICAL_MAXIMUM (127) */
    HID_LOGICAL_MAX8(0x7F),
    /* REPORT_COUNT (1) */
    HID_REPORT_COUNT(0x01),
    /* Input (Data,Var,Rel) */
    HID_INPUT(0x06),
    /* END COLLECTION */
    HID_END_COLLECTION,
    /* END COLLECTION */
    HID_END_COLLECTION,
#endif // IS_ENABLED(CONFIG_ZMK_MOUSE)
};

#if IS_ENABLED(CONFIG_ZMK_USB_BOOT)

#define HID_ERROR_ROLLOVER 0x1
#define HID_BOOT_KEY_LEN 6

#if IS_ENABLED(CONFIG_ZMK_HID_REPORT_TYPE_HKRO) &&                                                 \
    CONFIG_ZMK_HID_KEYBOARD_REPORT_SIZE == HID_BOOT_KEY_LEN
typedef struct zmk_hid_keyboard_report_body zmk_hid_boot_report_t;
#else
struct zmk_hid_boot_report {
    zmk_mod_flags_t modifiers;
    uint8_t _reserved;
    uint8_t keys[HID_BOOT_KEY_LEN];
} __packed;

typedef struct zmk_hid_boot_report zmk_hid_boot_report_t;
#endif
#endif

struct zmk_hid_keyboard_report_body {
    zmk_mod_flags_t modifiers;
    uint8_t _reserved;
#if IS_ENABLED(CONFIG_ZMK_HID_REPORT_TYPE_NKRO)
    uint8_t keys[DIV_ROUND_UP(ZMK_HID_KEYBOARD_NKRO_MAX_USAGE + 1, 8)];
#elif IS_ENABLED(CONFIG_ZMK_HID_REPORT_TYPE_HKRO)
    uint8_t keys[CONFIG_ZMK_HID_KEYBOARD_REPORT_SIZE];
#endif
} __packed;

struct zmk_hid_keyboard_report {
    uint8_t report_id;
    struct zmk_hid_keyboard_report_body body;
} __packed;

#if IS_ENABLED(CONFIG_ZMK_HID_INDICATORS)

struct zmk_hid_led_report_body {
    uint8_t leds;
} __packed;

struct zmk_hid_led_report {
    uint8_t report_id;
    struct zmk_hid_led_report_body body;
} __packed;

#endif // IS_ENABLED(CONFIG_ZMK_HID_INDICATORS)

struct zmk_hid_consumer_report_body {
#if IS_ENABLED(CONFIG_ZMK_HID_CONSUMER_REPORT_USAGES_BASIC)
    uint8_t keys[CONFIG_ZMK_HID_CONSUMER_REPORT_SIZE];
#elif IS_ENABLED(CONFIG_ZMK_HID_CONSUMER_REPORT_USAGES_FULL)
    uint16_t keys[CONFIG_ZMK_HID_CONSUMER_REPORT_SIZE];
#endif
} __packed;

struct zmk_hid_consumer_report {
    uint8_t report_id;
    struct zmk_hid_consumer_report_body body;
} __packed;

struct zmk_hid_mouse_report_body {
    zmk_mouse_button_flags_t buttons;
    int16_t x;
    int16_t y;
    int8_t scroll_y;
    int8_t scroll_x;
} __packed;

struct zmk_hid_mouse_report {
    uint8_t report_id;
    struct zmk_hid_mouse_report_body body;
} __packed;

zmk_mod_flags_t zmk_hid_get_explicit_mods();
int zmk_hid_register_mod(zmk_mod_t modifier);
int zmk_hid_unregister_mod(zmk_mod_t modifier);
bool zmk_hid_mod_is_pressed(zmk_mod_t modifier);

int zmk_hid_register_mods(zmk_mod_flags_t explicit_modifiers);
int zmk_hid_unregister_mods(zmk_mod_flags_t explicit_modifiers);
int zmk_hid_implicit_modifiers_press(zmk_mod_flags_t implicit_modifiers);
int zmk_hid_implicit_modifiers_release(void);
int zmk_hid_masked_modifiers_set(zmk_mod_flags_t masked_modifiers);
int zmk_hid_masked_modifiers_clear(void);

int zmk_hid_keyboard_press(zmk_key_t key);
int zmk_hid_keyboard_release(zmk_key_t key);
void zmk_hid_keyboard_clear(void);
bool zmk_hid_keyboard_is_pressed(zmk_key_t key);

int zmk_hid_consumer_press(zmk_key_t key);
int zmk_hid_consumer_release(zmk_key_t key);
void zmk_hid_consumer_clear(void);
bool zmk_hid_consumer_is_pressed(zmk_key_t key);

int zmk_hid_press(uint32_t usage);
int zmk_hid_release(uint32_t usage);
bool zmk_hid_is_pressed(uint32_t usage);

int zmk_hid_mouse_button_press(zmk_mouse_button_t button);
int zmk_hid_mouse_button_release(zmk_mouse_button_t button);
int zmk_hid_mouse_buttons_press(zmk_mouse_button_flags_t buttons);
int zmk_hid_mouse_buttons_release(zmk_mouse_button_flags_t buttons);
void zmk_hid_mouse_movement_set(int16_t x, int16_t y);
void zmk_hid_mouse_scroll_set(int8_t x, int8_t y);
void zmk_hid_mouse_movement_update(int16_t x, int16_t y);
void zmk_hid_mouse_scroll_update(int8_t x, int8_t y);
void zmk_hid_mouse_clear();

struct zmk_hid_keyboard_report *zmk_hid_get_keyboard_report();
struct zmk_hid_consumer_report *zmk_hid_get_consumer_report();
struct zmk_hid_mouse_report *zmk_hid_get_mouse_report();
