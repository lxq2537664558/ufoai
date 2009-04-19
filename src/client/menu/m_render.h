/**
 * @file m_render.h
 */

/*
Copyright (C) 1997-2008 UFO:AI Team

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#ifndef CLIENT_MENU_M_RENDER_H
#define CLIENT_MENU_M_RENDER_H

#include "../cl_renderer.h"

const image_t *MN_LoadImage(const char *name);

void MN_DrawNormImage(float x, float y, float w, float h, float sh, float th, float sl, float tl, int align, qboolean blend, const image_t *image);
const image_t *MN_DrawNormImageByName(float x, float y, float w, float h, float sh, float th, float sl, float tl, int align, qboolean blend, const char *name);

void MN_DrawPanel(const vec2_t pos, const vec2_t size, const char *texture, qboolean blend, int texX, int texY, const int *panelDef);
void MN_DrawFill(int x, int y, int w, int h, int align, const vec4_t color);

#endif
