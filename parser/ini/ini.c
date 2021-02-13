/* ini.h - simple, lightweight, easy to use ini parser
 *
 * Copyright (c) 2021 Fossbay
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
*/

#include "ini.h"

int ini_count(char *buf) {
	int i, j = 0, count = 0;
	for(i = 0; buf[i] != 0; i++) {
		switch(buf[i]) {
			case '#':
			case ';':
				if(j == 0) j = 1;
				break;
			case '\n':
				if(j == 1) j = 0;
				break;
			case '=':
			case '[': {
				if(j == 0) count++;
				break;
			}
		}
	}
	
	return count;
}

int ini_parse(char *buf, struct ini_value *values) {
	int i = 0, j = 0, k = 0;
	while(i != -1) {
		switch(buf[i]) {
			case '#':
			case ';':
				if(j == 0) j = -1;
				break;
			case 0:
				i = -2;
				if(j == 1) {
					j = 0;
					values->value = buf + k;
				}
				break;
			case '\n':
				switch(j) {
					case -1:
						j = 0;
						break;
					case 0:
						k = i + 1;
						break;
					case 1:
						j = 0;

						values->value = buf + k;
						buf[i] = 0;
						k = i + 1;
						values++;
						break;
					default: return i + 1;
				}
				break;
			case '=':
				if(j == 0) {
					j = 1;

					values->name = buf + k;
					buf[i] = 0;
					k = i + 1;
				} else if(j != -1) return i + 1;
				break;
			case '[':
				if(j == 0) {
					j = 2;
					k = i + 1;
				} else if(j != -1) return i + 1;
				break;
			case ']':
				if(j == 2) {
					j = 0;
					
					values->name = buf + k;
					values->value = 0;
					buf[i] = 0;
					k = i + 1;
					values++;
				} else if(j != -1) return i + 1;
				break;
		}

		i++;
	}

	return 0;
}
