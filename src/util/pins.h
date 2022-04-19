/* Gravitone Library
 * Copyright (c) 2022, Matt Ruffner <matthew.ruffner@movetones.com>
 * MoveTones, LLC
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice, development funding notice, and this permission
 * notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


// PIN DEFINITIONS FOR GRAVITONE V1.5 PCB
// CONNECTED TO A TEENSY 3.2
#define MCP23017_INTA	8  // MCP -> ARDUINO
#define MCP23017_INTB	7  // MCP -> ARDUINO
#define MAX98357_MODE	11 // LOW DISABLES, HIGH-Z ENABLES AUDIO AMP
#define OLED_CONTROL	12 // ACTIVE HIGH
#define BATTERY_SENSE	21 // ANALOG
#define BATTERY_STAT	20 // HIGH = CHARGING
#define ICM20948_INT	13 // ICM -> ARDUINO
#define ICM20948_SYNC	14 // ARDUINO -> ICM

#define ESP_GPIO0 2
#define ESP_RST   3
#define ESP_EN    4
