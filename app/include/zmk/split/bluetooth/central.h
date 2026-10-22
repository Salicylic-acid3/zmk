/*
 * Copyright (c) 2025 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

/*
 * Something reached the host or arrived from a peripheral: a key, a sensor
 * turn, a pointer report. With CONFIG_ZMK_SPLIT_BLE_PACED the split central
 * keeps the link at its fast interval for CONFIG_ZMK_SPLIT_BLE_IDLE_TIMEOUT_MS
 * after each call and slows it once they stop. Safe from any context.
 */
void zmk_split_bt_central_note_activity(void);
