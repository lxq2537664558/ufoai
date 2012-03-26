/**
 * @file ui_node_zone.h
 */

/*
Copyright (C) 2002-2011 UFO: Alien Invasion.

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

#ifndef CLIENT_UI_UI_NODE_ZONE_H
#define CLIENT_UI_UI_NODE_ZONE_H

class uiZoneNode : public uiLocatedNode {
	/**
	 * @override
	 */
	void loading(struct uiNode_s *node);
	/**
	 * @override
	 */
	void mouseDown(struct uiNode_s *node, int x, int y, int button);
	/**
	 * @override
	 */
	void mouseUp(struct uiNode_s *node, int x, int y, int button);
	/**
	 * @override
	 */
	void capturedMouseLost(struct uiNode_s *node);
};

typedef struct {
	qboolean repeat;			/**< repeat action when "click" is held */
	int clickDelay;				/**< delay for the next "click" */
} zoneExtraData_t;

struct uiBehaviour_s;

void UI_RegisterZoneNode(struct uiBehaviour_s *behaviour);

#endif
