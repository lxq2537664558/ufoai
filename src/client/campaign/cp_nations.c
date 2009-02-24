/**
 * @file cp_nations.c
 * @brief Campaign nations code
 */

/*
Copyright (C) 2002-2007 UFO: Alien Invasion team.

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

#include "../client.h"
#include "cl_campaign.h"
#include "cp_nations.h"
#include "cp_time.h"

/**
 * @brief Update the nation data from all parsed nation each month
 * @note give us nation support by:
 * * credits
 * * new soldiers
 * * new scientists
 * @note Called from CL_CampaignRun
 * @sa CL_CampaignRun
 * @sa B_CreateEmployee
 */
void CP_NationHandleBudget (void)
{
	int i, j;
	char message[1024];
	int cost;
	int initial_credits = ccs.credits;

	/* Refreshes the pilot global list.  Pilots who are already hired are unchanged, but all other
	 * pilots are replaced.  The new pilots is evenly distributed between the nations that are happy (happiness > 0). */
	E_RefreshUnhiredEmployeeGlobalList(EMPL_PILOT, qtrue);

	for (i = 0; i < ccs.numNations; i++) {
		nation_t *nation = &ccs.nations[i];
		const int funding = NAT_GetFunding(nation, 0);
		int new_scientists = 0, new_soldiers = 0, new_workers = 0;
		CL_UpdateCredits(ccs.credits + funding);

		for (j = 0; 0.25 + j < (float) nation->maxScientists * nation->stats[0].happiness * nation->stats[0].happiness; j++) {
			/* Create a scientist, but don't auto-hire her. */
			E_CreateEmployee(EMPL_SCIENTIST, nation, NULL);
			new_scientists++;
		}


		if (nation->stats[0].happiness > 0) {
			for (j = 0; 0.25 + j < (float) nation->maxSoldiers * nation->stats[0].happiness * nation->stats[0].happiness * nation->stats[0].happiness; j++) {
				/* Create a soldier. */
				E_CreateEmployee(EMPL_SOLDIER, nation, NULL);
				new_soldiers++;
			}
		}

		for (j = 0; 0.25 + j * 2 < (float) nation->maxSoldiers * nation->stats[0].happiness; j++) {
			/* Create a worker. */
			E_CreateEmployee(EMPL_WORKER, nation, NULL);
			new_workers++;
		}

		Com_sprintf(message, sizeof(message), _("Gained %i %s, %i %s, %i %s, and %i %s from nation %s (%s)"),
					funding, ngettext("credit", "credits", funding),
					new_scientists, ngettext("scientist", "scientists", new_scientists),
					new_soldiers, ngettext("soldier", "soldiers", new_soldiers),
					new_workers, ngettext("worker", "workers", new_workers),
					_(nation->name), NAT_GetHappinessString(nation));
		MS_AddNewMessageSound(_("Notice"), message, qfalse, MSG_STANDARD, NULL, qfalse);
	}

	cost = 0;
	for (i = 0; i < ccs.numEmployees[EMPL_SOLDIER]; i++) {
		if (ccs.employees[EMPL_SOLDIER][i].hired)
			cost += SALARY_SOLDIER_BASE + ccs.employees[EMPL_SOLDIER][i].chr.score.rank * SALARY_SOLDIER_RANKBONUS;
	}

	Com_sprintf(message, sizeof(message), _("Paid %i credits to soldiers"), cost);
	CL_UpdateCredits(ccs.credits - cost);
	/* only this message is played with sound as previous are added for every nation */
	MS_AddNewMessageSound(_("Notice"), message, qfalse, MSG_STANDARD, NULL, qtrue);

	cost = 0;
	for (i = 0; i < ccs.numEmployees[EMPL_WORKER]; i++) {
		if (ccs.employees[EMPL_WORKER][i].hired)
			cost += SALARY_WORKER_BASE + ccs.employees[EMPL_WORKER][i].chr.score.rank * SALARY_WORKER_RANKBONUS;
	}

	Com_sprintf(message, sizeof(message), _("Paid %i credits to workers"), cost);
	CL_UpdateCredits(ccs.credits - cost);
	MS_AddNewMessageSound(_("Notice"), message, qfalse, MSG_STANDARD, NULL, qfalse);

	cost = 0;
	for (i = 0; i < ccs.numEmployees[EMPL_SCIENTIST]; i++) {
		if (ccs.employees[EMPL_SCIENTIST][i].hired)
			cost += SALARY_SCIENTIST_BASE + ccs.employees[EMPL_SCIENTIST][i].chr.score.rank * SALARY_SCIENTIST_RANKBONUS;
	}

	Com_sprintf(message, sizeof(message), _("Paid %i credits to scientists"), cost);
	CL_UpdateCredits(ccs.credits - cost);
	MS_AddNewMessageSound(_("Notice"), message, qfalse, MSG_STANDARD, NULL, qfalse);

	cost = 0;
	for (i = 0; i < ccs.numEmployees[EMPL_PILOT]; i++) {
		if (ccs.employees[EMPL_PILOT][i].hired)
			cost += SALARY_PILOT_BASE + ccs.employees[EMPL_PILOT][i].chr.score.rank * SALARY_PILOT_RANKBONUS;
	}

	Com_sprintf(message, sizeof(message), _("Paid %i credits to pilots"), cost);
	CL_UpdateCredits(ccs.credits - cost);
	MS_AddNewMessageSound(_("Notice"), message, qfalse, MSG_STANDARD, NULL, qfalse);

	cost = 0;
	for (i = 0; i < ccs.numEmployees[EMPL_ROBOT]; i++) {
		if (ccs.employees[EMPL_ROBOT][i].hired)
			cost += SALARY_ROBOT_BASE + ccs.employees[EMPL_ROBOT][i].chr.score.rank * SALARY_ROBOT_RANKBONUS;
	}

	if (cost != 0) {
		Com_sprintf(message, sizeof(message), _("Paid %i credits for robots"), cost);
		CL_UpdateCredits(ccs.credits - cost);
		MS_AddNewMessageSound(_("Notice"), message, qfalse, MSG_STANDARD, NULL, qfalse);
	}

	cost = 0;
	for (i = 0; i < MAX_BASES; i++) {
		const base_t const *base = B_GetFoundedBaseByIDX(i);
		if (!base)
			continue;
		for (j = 0; j < base->numAircraftInBase; j++) {
			cost += base->aircraft[j].price * SALARY_AIRCRAFT_FACTOR / SALARY_AIRCRAFT_DIVISOR;
		}
	}

	if (cost != 0) {
		Com_sprintf(message, sizeof(message), _("Paid %i credits for aircraft"), cost);
		CL_UpdateCredits(ccs.credits - cost);
		MS_AddNewMessageSound(_("Notice"), message, qfalse, MSG_STANDARD, NULL, qfalse);
	}

	for (i = 0; i < MAX_BASES; i++) {
		const base_t const *base = B_GetFoundedBaseByIDX(i);
		if (!base)
			continue;
		cost = SALARY_BASE_UPKEEP;	/* base cost */
		for (j = 0; j < ccs.numBuildings[i]; j++) {
			cost += ccs.buildings[i][j].varCosts;
		}

		Com_sprintf(message, sizeof(message), _("Paid %i credits for upkeep of base %s"), cost, base->name);
		MS_AddNewMessageSound(_("Notice"), message, qfalse, MSG_STANDARD, NULL, qfalse);
		CL_UpdateCredits(ccs.credits - cost);
	}

	cost = SALARY_ADMIN_INITIAL + ccs.numEmployees[EMPL_SOLDIER] * SALARY_ADMIN_SOLDIER + ccs.numEmployees[EMPL_WORKER] * SALARY_ADMIN_WORKER + ccs.numEmployees[EMPL_SCIENTIST] * SALARY_ADMIN_SCIENTIST + ccs.numEmployees[EMPL_PILOT] * SALARY_ADMIN_PILOT + ccs.numEmployees[EMPL_ROBOT] * SALARY_ADMIN_ROBOT;
	Com_sprintf(message, sizeof(message), _("Paid %i credits for administrative overhead."), cost);
	CL_UpdateCredits(ccs.credits - cost);
	MS_AddNewMessageSound(_("Notice"), message, qfalse, MSG_STANDARD, NULL, qfalse);

	if (initial_credits < 0) {
		float interest = initial_credits * SALARY_DEBT_INTEREST;

		cost = (int)ceil(interest);
		Com_sprintf(message, sizeof(message), _("Paid %i credits in interest on your debt."), cost);
		CL_UpdateCredits(ccs.credits - cost);
		MS_AddNewMessageSound(_("Notice"), message, qfalse, MSG_STANDARD, NULL, qfalse);
	}
	CL_GameTimeStop();
}

/**
 * @brief Backs up each nation's relationship values.
 * @note Right after the copy the stats for the current month are the same as the ones from the (end of the) previous month.
 * They will change while the curent month is running of course :)
 * @todo other stuff to back up?
 */
void CP_NationBackupMonthlyData (void)
{
	int i, nat;

	/**
	 * Back up nation relationship .
	 * "inuse" is copied as well so we do not need to set it anywhere.
	 */
	for (nat = 0; nat < ccs.numNations; nat++) {
		nation_t *nation = &ccs.nations[nat];

		for (i = MONTHS_PER_YEAR - 1; i > 0; i--) {	/* Reverse copy to not overwrite with wrong data */
			nation->stats[i] = nation->stats[i - 1];
		}
	}
}
