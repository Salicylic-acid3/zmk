/*
 * Copyright (c) 2021 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <zephyr/sys/util.h>

#define DEBOUNCE_COUNTER_BITS 14
#define DEBOUNCE_COUNTER_MAX BIT_MASK(DEBOUNCE_COUNTER_BITS)

struct zmk_debounce_state {
    bool pressed : 1;
    bool changed : 1;
    uint16_t counter : DEBOUNCE_COUNTER_BITS;
    /** How long the switch has read as pressed since it latched as pressed,
     * saturating. Decides which release window applies (see config). */
    uint16_t held_ms;
};

struct zmk_debounce_config {
    /** Duration a switch must be pressed to latch as pressed. */
    uint32_t debounce_press_ms;
    /** Duration a switch must be released to latch as released. */
    uint32_t debounce_release_ms;
    /**
     * A second, longer release window for a switch that has been held.
     *
     * A snap-action (mouse) switch rests near its actuation point while a
     * finger holds it, and its contact can open for tens of milliseconds
     * before closing again; with one release window, that is a release and
     * a second press. A window long enough to absorb it would also merge a
     * quick double tap of the same key. The two cases differ in what came
     * before: a double tap starts from a short press, the chatter from a
     * long one. So once a switch has been pressed for
     * debounce_held_threshold_ms, debounce_release_held_ms is the window
     * instead of debounce_release_ms. 0 keeps a single window.
     */
    uint32_t debounce_release_held_ms;
    /** How long a switch must have been pressed for the held window to apply. */
    uint32_t debounce_held_threshold_ms;
};

/**
 * Debounces one switch.
 *
 * @param state The state for the switch to debounce.
 * @param active Is the switch currently pressed?
 * @param elapsed_ms Time elapsed since the previous update in milliseconds.
 * @param config Debounce settings.
 */
void zmk_debounce_update(struct zmk_debounce_state *state, const bool active, const int elapsed_ms,
                         const struct zmk_debounce_config *config);

/**
 * @returns whether the switch is either latched as pressed or it is potentially
 * pressed but the debouncer has not yet made a decision. If this returns true,
 * the kscan driver should continue to poll quickly.
 */
bool zmk_debounce_is_active(const struct zmk_debounce_state *state);

/**
 * @returns whether the switch is latched as pressed.
 */
bool zmk_debounce_is_pressed(const struct zmk_debounce_state *state);

/**
 * @returns whether the pressed state of the switch changed in the last call to
 * debounce_update.
 */
bool zmk_debounce_get_changed(const struct zmk_debounce_state *state);
