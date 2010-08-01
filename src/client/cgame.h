/**
 * @file cgame.h
 * @brief Client game mode interface
 */

/*
Copyright (C) 2002-2010 UFO: Alien Invasion.

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

#ifndef CL_CGAME_H
#define CL_CGAME_H

#include "mxml/mxml.h"

typedef struct {
	const char *name;
	const char *menu;
	int gametype;
	void (EXPORT *Init) (void);
	void (EXPORT *Shutdown) (void);
	/** soldier spawn functions may differ between the different gametypes */
	qboolean (EXPORT *Spawn) (void);
	/** each gametype can handle the current team in a different way */
	int (EXPORT *GetTeam) (void);
	/** some gametypes only support special maps */
	const mapDef_t* (EXPORT *MapInfo) (int step);
	/** some gametypes require extra data in the results parsing (like e.g. campaign mode) */
	void (EXPORT *Results) (struct dbuffer *msg, int, int*, int*, int[][MAX_TEAMS], int[][MAX_TEAMS]);
	/** check whether the given item is usable in the current game mode */
	qboolean (EXPORT *IsItemUseable) (const objDef_t *od);
	/** shows item info if not resolvable via objDef_t */
	void (EXPORT *DisplayItemInfo) (uiNode_t *node, const char *string);
	/** returns the equipment definition the game mode is using */
	equipDef_t* (EXPORT *GetEquipmentDefinition) (void);
	/** update character display values for game type dependent stuff */
	void (EXPORT *UpdateCharacterValues) (const character_t *chr);
	/** checks whether the given team is known in the particular gamemode */
	qboolean (EXPORT *IsTeamKnown) (const teamDef_t *teamDef);
	/** called on errors */
	void (EXPORT *Drop) (void);
	/** called after the team spawn messages where send, can e.g. be used to set initial actor states */
	void (EXPORT *InitializeBattlescape) (const chrList_t *team);
	/** callback that is executed every frame */
	void (EXPORT *RunFrame) (void);
	/** if you want to display a different model for the given object in your game mode, implement this function */
	const char* (EXPORT *GetModelForItem) (const objDef_t*od);
	void (EXPORT *EndRoundAnnounce) (int playerNum, int team);
	void (EXPORT *StartBattlescape) (qboolean isTeamPlay);
} cgame_export_t;

/** @todo define the import interface */
typedef struct {
	csi_t *csi;
	client_static_t *cls;
	client_state_t *cl;

	/* menu functions */
	void (IMPORT *MN_ExecuteConfunc) (const char *fmt, ...) __attribute__((format(printf, 1, 2)));
	void (IMPORT *MN_PopWindow) (qboolean all);
	uiNode_t* (IMPORT *MN_PushWindow) (const char *name, const char *parentName);
	void (IMPORT *MN_InitStack) (const char* activeMenu, const char* mainMenu, qboolean popAll, qboolean pushActive);
	void (IMPORT *MN_Popup) (const char *title, const char *text);
	uiNode_t* (IMPORT *MN_AddOption) (uiNode_t** tree, const char* name, const char* label, const char* value);
	void (IMPORT *MN_RegisterOption) (int dataId, uiNode_t *option);
	void (IMPORT *MN_RegisterText) (int textId, const char *text);
	void (IMPORT *MN_ResetData) (int dataId);
	void (IMPORT *MN_RegisterLinkedListText) (int textId, linkedList_t *text);
	void (IMPORT *MN_TextNodeSelectLine) (uiNode_t *node, int num);
	uiNode_t *(IMPORT *MN_PopupList) (const char *title, const char *headline, linkedList_t* entries, const char *clickAction);

	/* xml functions */
	mxml_node_t * (IMPORT *mxml_AddNode) (mxml_node_t *parent, const char *name);
	void (IMPORT *mxml_AddString) (mxml_node_t *parent, const char *name, const char *value);
	void (IMPORT *mxml_AddBool) (mxml_node_t *parent, const char *name, qboolean value);
	void (IMPORT *mxml_AddFloat) (mxml_node_t *parent, const char *name, float value);
	void (IMPORT *mxml_AddDouble) (mxml_node_t *parent, const char *name, double value);
	void (IMPORT *mxml_AddByte) (mxml_node_t *parent, const char *name, byte value);
	void (IMPORT *mxml_AddShort) (mxml_node_t *parent, const char *name, short value);
	void (IMPORT *mxml_AddInt) (mxml_node_t *parent, const char *name, int value);
	void (IMPORT *mxml_AddLong) (mxml_node_t *parent, const char *name, long value);
	void (IMPORT *mxml_AddPos3) (mxml_node_t *parent, const char *name, const vec3_t pos);
	void (IMPORT *mxml_AddPos2) (mxml_node_t *parent, const char *name, const vec2_t pos);
	void (IMPORT *mxml_AddDate) (mxml_node_t *parent, const char *name, const int day, const int sec);
	void (IMPORT *mxml_AddStringValue) (mxml_node_t *parent, const char *name, const char *value);
	void (IMPORT *mxml_AddBoolValue) (mxml_node_t *parent, const char *name, qboolean value);
	void (IMPORT *mxml_AddFloatValue) (mxml_node_t *parent, const char *name, float value);
	void (IMPORT *mxml_AddDoubleValue) (mxml_node_t *parent, const char *name, double value);
	void (IMPORT *mxml_AddByteValue) (mxml_node_t *parent, const char *name, byte value);
	void (IMPORT *mxml_AddShortValue) (mxml_node_t *parent, const char *name, short value);
	void (IMPORT *mxml_AddIntValue) (mxml_node_t *parent, const char *name, int value);
	void (IMPORT *mxml_AddLongValue) (mxml_node_t *parent, const char *name, long value);

	/* filesystem functions */
	int (IMPORT *FS_LoadFile) (const char *path, byte **buffer);
	void (IMPORT *FS_FreeFile) (void *buffer);

	/* console variable interaction */
	cvar_t *(IMPORT *Cvar_Get) (const char *varName, const char *value, int flags, const char* desc);
	cvar_t *(IMPORT *Cvar_Set) (const char *varName, const char *value);
	void (IMPORT *Cvar_SetValue) (const char *varName, float value);
	const char *(IMPORT *Cvar_String) (const char *varName);
	int (IMPORT *Cvar_Integer) (const char *varName);
	qboolean (IMPORT *Cvar_Delete) (const char *varName);

	/* sound functions */
	void (IMPORT *S_StartLocalSample) (const char *s, float volume);
	void (IMPORT *S_SetSampleRepeatRate) (int sampleRepeatRate);

	/* renderer functions */
	void (IMPORT *R_SoftenTexture) (byte *in, int width, int height, int bpp);
	void (IMPORT *R_UploadAlpha) (const image_t *image, const byte *alphaData);
	void (IMPORT *R_LoadImage) (const char *name, byte **pic, int *width, int *height);
	image_t *(IMPORT *R_LoadImageData) (const char *name, byte * pic, int width, int height, imagetype_t type);

	/* ClientCommand and ServerCommand parameter access */
	int (IMPORT *Cmd_Argc) (void);
	const char *(IMPORT *Cmd_Argv) (int n);
	const char *(IMPORT *Cmd_Args) (void);		/**< concatenation of all argv >= 1 */
	void (IMPORT *Cmd_AddCommand) (const char *cmdName, xcommand_t function, const char *desc);
	void (IMPORT *Cmd_RemoveCommand) (const char *cmdName);
	void (IMPORT *Cmd_ExecuteString) (const char *text);
} cgame_import_t;

cgame_export_t *GetCGameAPI(cgame_import_t *import);

#endif
