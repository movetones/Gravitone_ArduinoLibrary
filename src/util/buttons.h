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


#ifndef GRAVITONE_BUTTON_INTERFACE
#define GRAVITONE_BUTTON_INTERFACE

// BUTTON CONNECTIONS w.r.t. TO MCP23017
enum ButtonMap {
    GB1,
    GB2,
    GB3,
    GB4,
    GB5,
    GB6,
    GB7,
    GB8,
    GB9,
    GB10,
    GB11,
    GB12
};

#define NUM_BUTTONS   12


typedef uint8_t butevent_t;

enum ButtonEvent {
  BUTTON_OPEN=8,
  BUTTON_PRESSED=9,  // this is a user pressing the button
  BUTTON_RELEASED=10, // this is the user releasing the button
  BUTTON_HELD=11      // the button has been held down
};

#endif
