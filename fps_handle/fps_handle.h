 /*
  * pixel_frame
  * Copyright (C) 2013  Julian Iseringhausen <julian.iseringhausen@gmail.com>
  *
  * This program is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
  */

/*
 * fps_handle.h
 *
 * Created: 30.10.2012 23:05:54
 *  Author: Julian Iseringhausen <julian.iseringhausen@gmail.com>
 *
 *  Description: Implements VSYNC.
 */ 


#ifndef FPS_HANDLE_H_
#define FPS_HANDLE_H_

#define FRAMES_MS(x) (((uint32_t)(FPS)*(x))/1000)

#define FPS_LOOP(x) { \
	if (!(FLAGS & (_BV(FLAG_GS_UPDATE) | _BV(FLAG_FPS_UPDATE)))) { \
	{x} \
	FLAGS |= _BV(FLAG_GS_UPDATE) | _BV(FLAG_FPS_UPDATE); \
	}}
	

extern void fps_handle_init();

#endif /* FPS_HANDLE_H_ */