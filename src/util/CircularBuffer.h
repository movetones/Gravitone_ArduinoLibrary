/*
 CircularBuffer.h - Circular buffer library for Arduino.
 Copyright (c) 2017 Roberto Lo Giacco.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef __CIRCULAR_BUFFER__
#define __CIRCULAR_BUFFER__
#include <inttypes.h>

template<typename T, uint16_t S>
class CircularBuffer {
public:

	CircularBuffer() : head(buffer), tail(buffer), count(0) {
	}

	~CircularBuffer() {
	}

	/**
	 * Adds an element to the beginning of buffer returning `false` if the addition caused overwriting an existing element.
	 */
	bool push(T value) {
		if (head == buffer) {
			head = buffer + S;
		}
		*--head = value;
		if (count == S) {
			if (tail-- == buffer) {
				tail = buffer + S - 1;
			}
			return false;
		} else {
			if (count++ == 0) {
				tail = head;
			}
			return true;
		}
	}

	/**
	 * Adds an element to the end of buffer returning `false` if the addition caused overwriting an existing element.
	 */
	bool append(T value) {
		if (++tail == buffer + S) {
			tail = buffer;
		}
		*tail = value;
		if (count == S) {
			if (++head == buffer + S) {
				head = buffer;
			}
			return false;
		} else {
			if (count++ == 0) {
				head = tail;
			}
			return true;
		}
	}

	/**
	 * Removes an element from the beginning of the buffer.
	 */
	T pop() {
		T result = *head++;
		if (head == buffer + S) {
			head = buffer;
		}
		count--;
		return result;
	}

	/**
	 * Removes an element from the end of the buffer.
	 */
	T pull() {
		T result = *tail--;
		if (tail == buffer) {
			tail = buffer + S - 1;
		}
		count--;
		return result;
	}

	/**
	 * Returns the element at the beginning of the buffer.
	 * Like pop(), but it doesn't remove the element.
	 */
	T inline first() { return *head; }

	/**
	 * Returns the element at the end of the buffer.
	 * Like poll(), but it doesn't remove the element.
	 */
	T inline last() { return *tail; }

	/**
	 * Array-like access to buffer
	 */
	T operator [] (uint16_t index) { return *(buffer + ((head - buffer + index) % S)); }

	/**
	 * Returns how many elements are actually stored in the buffer.
	 */
	int inline size() { return count; }

	/**
	 * Returns how many elements can be safely pushed into the buffer.
	 */
	int inline available() { return S - count; }

	/**
	 * Returns `true` if no elements can be removed from the buffer.
	 */
	bool inline isEmpty() { return count == 0; }

	/**
	 * Returns `true` if no elements can be added to the buffer without overwriting existing elements.
	 */
	bool inline isFull() { return count == S; }

	/**
	 * Resets the buffer to a clean status, dropping any reference to current elements
	 * and making all buffer positions available again.
	 */
	void inline clear() {
		head = tail = buffer;
		count = 0;
	}

private:
	T buffer[S];
	T *head;
	T *tail;
	uint16_t count;
};
#endif
