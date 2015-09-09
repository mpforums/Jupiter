/**
 * Copyright (C) 2014-2015 Justin James.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Written by Justin James <justin.aj@hotmail.com>
 */

#include <ctime>
#include "Jupiter/IRC_Client.h"
#include "Jupiter/INIFile.h"
#include "ExcessiveHeadshots.h"
#include "RenX_Server.h"
#include "RenX_PlayerInfo.h"
#include "RenX_Functions.h"

RenX_ExcessiveHeadshotsPlugin::RenX_ExcessiveHeadshotsPlugin()
{
	RenX_ExcessiveHeadshotsPlugin::OnRehash();
}

int RenX_ExcessiveHeadshotsPlugin::OnRehash()
{
	RenX_ExcessiveHeadshotsPlugin::ratio = Jupiter::IRC::Client::Config->getDouble(RenX_ExcessiveHeadshotsPlugin::getName(), STRING_LITERAL_AS_REFERENCE("HeadshotKillRatio"), 0.5);
	RenX_ExcessiveHeadshotsPlugin::minKills = Jupiter::IRC::Client::Config->getInt(RenX_ExcessiveHeadshotsPlugin::getName(), STRING_LITERAL_AS_REFERENCE("Kills"), 10);
	RenX_ExcessiveHeadshotsPlugin::minKD = Jupiter::IRC::Client::Config->getDouble(RenX_ExcessiveHeadshotsPlugin::getName(), STRING_LITERAL_AS_REFERENCE("KillDeathRatio"), 5.0);
	RenX_ExcessiveHeadshotsPlugin::minKPS = Jupiter::IRC::Client::Config->getDouble(RenX_ExcessiveHeadshotsPlugin::getName(), STRING_LITERAL_AS_REFERENCE("KillsPerSecond"), 0.5);
	RenX_ExcessiveHeadshotsPlugin::minFlags = Jupiter::IRC::Client::Config->getInt(RenX_ExcessiveHeadshotsPlugin::getName(), STRING_LITERAL_AS_REFERENCE("Flags"), 4);
	return 0;
}

void RenX_ExcessiveHeadshotsPlugin::RenX_OnKill(RenX::Server *server, const RenX::PlayerInfo *player, const RenX::PlayerInfo *victim, const Jupiter::ReadableString &damageType)
{
	if (player->kills < 3) return;
	if (damageType.equals("Rx_DmgType_Headshot"))
	{
		unsigned int flags = 0;
		std::chrono::milliseconds game_time = server->getGameTime(player);
		double kps = game_time == std::chrono::milliseconds(0) ? static_cast<double>(player->kills) : static_cast<double>(player->kills) / static_cast<double>(game_time.count());
		if (player->kills >= RenX_ExcessiveHeadshotsPlugin::minKills) flags++;
		if (RenX::getHeadshotKillRatio(player) >= RenX_ExcessiveHeadshotsPlugin::ratio) flags++;
		if (RenX::getKillDeathRatio(player) >= RenX_ExcessiveHeadshotsPlugin::minKD) flags++;
		if (kps >= RenX_ExcessiveHeadshotsPlugin::minKPS) flags++;
		if (kps >= RenX_ExcessiveHeadshotsPlugin::minKPS * 2) flags++;
		if (game_time <= RenX_ExcessiveHeadshotsPlugin::maxGameTime) flags++;

		if (flags >= RenX_ExcessiveHeadshotsPlugin::minFlags)
		{
			server->banPlayer(player, STRING_LITERAL_AS_REFERENCE("Aimbot detected"));
			server->sendPubChan(IRCCOLOR "13[Aimbot]" IRCCOLOR " %.*s was banned from the server! Kills: %u - Deaths: %u - Headshots: %u", player->name.size(), player->name.ptr(), player->kills, player->deaths, player->headshots);
			const Jupiter::ReadableString &steamid = server->formatSteamID(player);
			server->sendAdmChan(IRCCOLOR "13[Aimbot]" IRCCOLOR " %.*s was banned from the server! Kills: %u - Deaths: %u - Headshots: %u - IP: " IRCBOLD "%.*s" IRCBOLD " - Steam ID: " IRCBOLD "%.*s" IRCBOLD, player->name.size(), player->name.ptr(), player->kills, player->deaths, player->headshots, player->ip.size(), player->ip.ptr(), steamid.size(), steamid.ptr());
		}
	}
}


// Plugin instantiation and entry point.
RenX_ExcessiveHeadshotsPlugin pluginInstance;

extern "C" __declspec(dllexport) Jupiter::Plugin *getPlugin()
{
	return &pluginInstance;
}
