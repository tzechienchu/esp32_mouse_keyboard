/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * Copyright 2017 Benjamin Aigner <beni@asterics-foundation.org>
 */

/** @file
 * @brief This file is a C compatible wrapper for Neil Kolbans CPP utils
 * 
 * It initializes 3 queues for sending mouse, keyboard and joystick data
 * from within the C-side. C++ classes are instantiated from here.
 * If you want to have a different BLE HID device, you need to adapt this file.
 * 
 * @note Thank you very much Neil Kolban for this impressive work!
*/

#ifndef _HID_kbdmousejoystick_H_
#define _HID_kbdmousejoystick_H_


#ifdef __cplusplus
extern "C" {
#endif

#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>
#include <freertos/queue.h>
#include <esp_log.h>

/** @brief Queue for sending mouse reports
 * @see mouse_command_t */
extern QueueHandle_t mouse_q;

/** @brief Queue for sending keyboard reports
 * @see keyboard_command_t */
extern QueueHandle_t keyboard_q;

/** @brief Queue for sending joystick reports
 * @see joystick_command_t */
extern QueueHandle_t joystick_q;

/** @brief Main init function to start HID interface
 * 
 * @note After init, just use the queues! */
esp_err_t HID_kbdmousejoystick_init(void);

/** @brief One mouse command (report) to be sent via BLE mouse profile
 * @see mouse_q */
typedef struct mouse_command {
  int8_t x;
  int8_t y;
  int8_t wheel;
  uint8_t buttons;
} mouse_command_t;

/** @brief One command (report) to be issued via BLE joystick profile
 * @see joystick_q */
typedef struct joystick_command {
  /** @brief Button mask, allows up to 32 different buttons */
  uint32_t buttonmask;
  /** @brief X-axis value, 0-1023 */
  uint16_t Xaxis;
  /** @brief Y-axis value, 0-1023 */
  uint16_t Yaxis;
  /** @brief Z-axis value, 0-1023 */
  uint16_t Zaxis;
  /** @brief Z-rotate value, 0-1023 */
  uint16_t Zrotate;
  /** @brief Slider left value, 0-1023 */
  uint16_t sliderLeft;
  /** @brief Slider right value, 0-1023 */
  uint16_t sliderRight;
  /** @brief Hat position (0-360), mapped to 8 directions. Use <0 for no pressing*/
  int16_t hat;
} joystick_command_t;

/** @brief Type of keycode action.
 * 
 * * Use press to add the keycode/modifier to the report.
 * * Use release to remove the keycode/modifier from the report.
 * * Use press_release to send 2 reports: one with the added keycode, one without it.
 * */
typedef enum {
  PRESS,
  RELEASE,
  PRESS_RELEASE
} keyboard_action;

/** @brief One command (report) to be issued via BLE keyboard profile
 * @see keyboard_q */
typedef struct keyboard_command {
  /** @brief type of this keyboard action */
  keyboard_action type;
  /** list of keycodes+modfiers to be pressed/released.
   * @note Low byte contains the keycode, high byte any modifiers
   * */
  uint16_t keycodes;
} keyboard_command_t;

#ifdef __cplusplus
}
#endif

#endif /* _HID_kbdmousejoystick_H_ */