/**
 * @file e_event_particleappear.c
 */

/*
Copyright (C) 2002-2009 UFO: Alien Invasion.

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

#include "../../../../client.h"
#include "../../../cl_localentity.h"
#include "../../../cl_particle.h"
#include "e_event_particleappear.h"

/**
 * @brief Let a particle appear for the client
 * @param[in] msg holds the network data
 * @sa CL_ParticleSpawn
 * @sa EV_SPAWN_PARTICLE
 */
void CL_ParticleAppear (const eventRegister_t *self, struct dbuffer *msg)
{
	char *particle;
	int entnum, levelflags;
	le_t* le;

	/* read data */
	NET_ReadFormat(msg, self->formatString, &entnum, &levelflags, &particle);

	le = LE_Get(entnum);
	if (!le)
		LE_NotFoundError(entnum);

	/* particles don't have a model to add to the scene - we mark them as invisible and
	 * only render the particle */
	le->invis = !cl_leshowinvis->integer;
	le->levelflags = levelflags;
	le->particleID = Mem_PoolStrDup(particle, cl_genericPool, 0);
	le->ptl = CL_ParticleSpawn(le->particleID, le->levelflags, le->origin, NULL, NULL);
	if (!le->ptl)
		Com_Printf("Could not spawn particle: '%s'\n", le->particleID);
}
