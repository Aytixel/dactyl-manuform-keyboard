/*
Copyright (c) 2015 NicoHood
See the readme for credit to other people.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

// Include guard
#pragma once


size_t NKROKeyboardAPI::set(uint16_t k, bool s) 
{
	// Press key
	if (k & 0x00FF){
		uint8_t key = k;
		// Invalid scancode with 104 nkro
		if ((key > 0x65 && 0xE0 > key) || key > 0xE7) {
			return 0;
		}
		// Skip gap in scancode with 104 nkro
		if (key > 0x65) {
			key -= 0x7A;
		}
		uint8_t bit = 1 << (uint8_t(key) % 8);
		if (s) {
			_keyReport.keys[key / 8] |= bit;
		} else {
			_keyReport.keys[key / 8] &= ~bit;
		}
	}

	// Press modifier
	if (k & 0xFF00) {
		uint8_t modifier = k >> 8;
		if (s) {
			_keyReport.modifiers |= modifier;
		} else {
			_keyReport.modifiers &= ~modifier;
		}
	}
	
	return 1;
}

size_t NKROKeyboardAPI::removeAll(void)
{
	// Release all keys
	size_t ret = 0;
	for (uint8_t i = 0; i < sizeof(_keyReport.allkeys); i++)
	{
		// Is a key in the list or did we found an empty slot?
		auto bits = _keyReport.allkeys[i];
		do {
			if (bits & 0x01) {
				ret++;
			}
			bits >>=1;
		} while(bits);
		_keyReport.allkeys[i] = 0x00;
	}
	return ret;
}

