/*  EQEMu:  Everquest Server Emulator
	Copyright (C) 2001-2006  EQEMu Development Team (http://eqemulator.net)

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; version 2 of the License.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY except by those people which sell it, which
	are required to give you total support for your newly bought product;
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR
	A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "../common/features.h"
#include "../common/content/world_content_service.h"

#ifdef EMBPERL
#ifdef EMBPERL_XS

#include "../common/global_define.h"
#include "../common/misc_functions.h"
#include "../common/eqemu_logsys.h"

#include "dialogue_window.h"
#include "embperl.h"
#include "embxs.h"
#include "entity.h"
#include "queryserv.h"
#include "questmgr.h"
#include "zone.h"
#include "data_bucket.h"

#include <cctype>

extern Zone      *zone;
extern QueryServ *QServ;

#ifdef EMBPERL_XS_CLASSES

//Any creation of new Client objects gets the current quest Client
Client* Perl__Client_new()
{
	return quest_manager.GetInitiator();
}

//Any creation of new NPC objects gets the current quest NPC
NPC* Perl__NPC_new()
{
	return quest_manager.GetNPC();
}

//Any creation of new NPC objects gets the current quest NPC
EntityList* Perl__EntityList_new()
{
	return &entity_list;
}

//Any creation of new inventory gets the curreny inventory
EQ::InventoryProfile* Perl__Inventory_new()
{
	return quest_manager.GetInventory();
}

//Any creation of new quest items gets the current quest item
EQ::ItemInstance* Perl__QuestItem_new()
{
	return quest_manager.GetQuestItem();
}

//Any creation of new Spells gets the current Spell
SPDat_Spell_Struct* Perl__Spell_new()
{
	// should be safe, it's read only in perl (could also use proxy lika lua)
	return const_cast<SPDat_Spell_Struct*>(quest_manager.GetQuestSpell());
}

#endif //EMBPERL_XS_CLASSES


void Perl__echo(int emote_color_id, const char* message)
{
	quest_manager.echo(emote_color_id, message);
}

void Perl__say(const char* message)
{
	Journal::Options opts;
	// we currently default to these
	opts.speak_mode   = Journal::SpeakMode::Say;
	opts.journal_mode = Journal::Mode::Log2;
	opts.language     = 0;
	opts.message_type = Chat::NPCQuestSay;

	quest_manager.say(message, opts);
}

void Perl__say(const char* message, int language_id)
{
	Journal::Options opts;
	opts.speak_mode   = Journal::SpeakMode::Say;
	opts.journal_mode = Journal::Mode::Log2;
	opts.language     = language_id;
	opts.message_type = Chat::NPCQuestSay;

	quest_manager.say(message, opts);
}

void Perl__say(const char* message, int language_id, int message_type)
{
	Journal::Options opts;
	opts.speak_mode   = Journal::SpeakMode::Say;
	opts.journal_mode = Journal::Mode::Log2;
	opts.language     = language_id;
	opts.message_type = message_type;

	quest_manager.say(message, opts);
}

void Perl__say(const char* message, int language_id, int message_type, int speak_mode)
{
	Journal::Options opts;
	opts.speak_mode   = static_cast<Journal::SpeakMode>(speak_mode);
	opts.journal_mode = Journal::Mode::Log2;
	opts.language     = language_id;
	opts.message_type = message_type;

	quest_manager.say(message, opts);
}

void Perl__say(const char* message, int language_id, int message_type, int speak_mode, int journal_mode)
{
	Journal::Options opts;
	opts.speak_mode   = static_cast<Journal::SpeakMode>(speak_mode);
	opts.journal_mode = static_cast<Journal::Mode>(journal_mode);
	opts.language     = language_id;
	opts.message_type = message_type;

	quest_manager.say(message, opts);
}

void Perl__me(const char* message)
{
	quest_manager.me(message);
}

void Perl__summonitem(int item_id)
{
	quest_manager.summonitem(item_id);
}

void Perl__summonitem(int item_id, int charges)
{
	quest_manager.summonitem(item_id, charges);
}

void Perl__write(const char* file, const char* message)
{
	quest_manager.write(file, message);
}

int Perl__spawn(int npc_type_id, int grid_id, int unused, float x, float y, float z)
{
	auto position = glm::vec4(x, y, z, 0.0f);
	Mob* mob = quest_manager.spawn2(npc_type_id, grid_id, unused, position);
	return mob ? mob->GetID() : 0;
}

int Perl__spawn2(int npc_type_id, int grid_id, int unused, float x, float y, float z, float heading)
{
	auto position = glm::vec4(x, y, z, heading);
	Mob* mob = quest_manager.spawn2(npc_type_id, grid_id, unused, position);
	return mob ? mob->GetID() : 0;
}

int Perl__unique_spawn(int npc_type_id, int grid_id, int unused, float x, float y, float z)
{
	auto position = glm::vec4(x, y, z, 0.0f);
	Mob* mob = quest_manager.unique_spawn(npc_type_id, grid_id, unused, position);
	return mob ? mob->GetID() : 0;
}

int Perl__unique_spawn(int npc_type_id, int grid_id, int unused, float x, float y, float z, float heading)
{
	auto position = glm::vec4(x, y, z, heading);
	Mob* mob = quest_manager.unique_spawn(npc_type_id, grid_id, unused, position);
	return mob ? mob->GetID() : 0;
}

int Perl__spawn_from_spawn2(uint32_t spawn2_id)
{
	Mob* mob = quest_manager.spawn_from_spawn2(spawn2_id);
	return mob ? mob->GetID() : 0;
}

void Perl__enable_spawn2(uint32_t spawn2_id)
{
	quest_manager.enable_spawn2(spawn2_id);
}

void Perl__disable_spawn2(uint32_t spawn2_id)
{
	quest_manager.disable_spawn2(spawn2_id);
}

void Perl__setstat(int stat_id, int int_value)
{
	quest_manager.setstat(stat_id, int_value);
}

void Perl__incstat(int stat_id, int int_value)
{
	quest_manager.incstat(stat_id, int_value);
}

int Perl__getinventoryslotid(std::string identifier)
{
	int result = EQ::invslot::SLOT_INVALID;

	for (std::string::size_type i = 0; i < identifier.size(); ++i)
		identifier[i] = std::tolower(identifier[i]);

	if (identifier.find('.') == std::string::npos) {
		if (identifier == "invalid")                    result = EQ::invslot::SLOT_INVALID;
		else if (identifier == "charm")                 result = EQ::invslot::slotCharm;
		else if (identifier == "ear1")                  result = EQ::invslot::slotEar1;
		else if (identifier == "head")                  result = EQ::invslot::slotHead;
		else if (identifier == "face")                  result = EQ::invslot::slotFace;
		else if (identifier == "ear2")                  result = EQ::invslot::slotEar2;
		else if (identifier == "neck")                  result = EQ::invslot::slotNeck;
		else if (identifier == "shoulders")             result = EQ::invslot::slotShoulders;
		else if (identifier == "arms")                  result = EQ::invslot::slotArms;
		else if (identifier == "back")                  result = EQ::invslot::slotBack;
		else if (identifier == "wrist1")                result = EQ::invslot::slotWrist1;
		else if (identifier == "wrist2")                result = EQ::invslot::slotWrist2;
		else if (identifier == "range")                 result = EQ::invslot::slotRange;
		else if (identifier == "hands")                 result = EQ::invslot::slotHands;
		else if (identifier == "primary")               result = EQ::invslot::slotPrimary;
		else if (identifier == "secondary")             result = EQ::invslot::slotSecondary;
		else if (identifier == "finger1")               result = EQ::invslot::slotFinger1;
		else if (identifier == "finger2")               result = EQ::invslot::slotFinger2;
		else if (identifier == "chest")                 result = EQ::invslot::slotChest;
		else if (identifier == "legs")                  result = EQ::invslot::slotLegs;
		else if (identifier == "feet")                  result = EQ::invslot::slotFeet;
		else if (identifier == "waist")                 result = EQ::invslot::slotWaist;
		else if (identifier == "powersource")           result = EQ::invslot::slotPowerSource;
		else if (identifier == "ammo")                  result = EQ::invslot::slotAmmo;
		else if (identifier == "general1")              result = EQ::invslot::slotGeneral1;
		else if (identifier == "general2")              result = EQ::invslot::slotGeneral2;
		else if (identifier == "general3")              result = EQ::invslot::slotGeneral3;
		else if (identifier == "general4")              result = EQ::invslot::slotGeneral4;
		else if (identifier == "general5")              result = EQ::invslot::slotGeneral5;
		else if (identifier == "general6")              result = EQ::invslot::slotGeneral6;
		else if (identifier == "general7")              result = EQ::invslot::slotGeneral7;
		else if (identifier == "general8")              result = EQ::invslot::slotGeneral8;
		else if (identifier == "general9")              result = EQ::invslot::slotGeneral9;
		else if (identifier == "general10")             result = EQ::invslot::slotGeneral10;
		else if (identifier == "cursor")                result = EQ::invslot::slotCursor;
		else if (identifier == "tradeskill")            result = EQ::invslot::SLOT_TRADESKILL_EXPERIMENT_COMBINE;
		else if (identifier == "augment")               result = EQ::invslot::SLOT_AUGMENT_GENERIC_RETURN;
	}
	else {
		if (identifier == "possessions.begin")          result = EQ::invslot::POSSESSIONS_BEGIN;
		else if (identifier == "possessions.end")       result = EQ::invslot::POSSESSIONS_END;
		else if (identifier == "equipment.begin")       result = EQ::invslot::EQUIPMENT_BEGIN;
		else if (identifier == "equipment.end")         result = EQ::invslot::EQUIPMENT_END;
		else if (identifier == "general.begin")         result = EQ::invslot::GENERAL_BEGIN;
		else if (identifier == "general.end")           result = EQ::invslot::GENERAL_END;
		else if (identifier == "possessionsbags.begin") result = EQ::invbag::GENERAL_BAGS_BEGIN;
		else if (identifier == "possessionsbags.end")   result = EQ::invbag::CURSOR_BAG_END;
		else if (identifier == "generalbags.begin")     result = EQ::invbag::GENERAL_BAGS_BEGIN;
		else if (identifier == "generalbags.end")       result = EQ::invbag::GENERAL_BAGS_END;
		else if (identifier == "general1bag.begin")     result = EQ::invbag::GENERAL_BAGS_BEGIN;
		else if (identifier == "general1bag.end")       result = EQ::invbag::GENERAL_BAGS_BEGIN + 9;
		else if (identifier == "general2bag.begin")     result = EQ::invbag::GENERAL_BAGS_BEGIN + 10;
		else if (identifier == "general2bag.end")       result = EQ::invbag::GENERAL_BAGS_BEGIN + 19;
		else if (identifier == "general3bag.begin")     result = EQ::invbag::GENERAL_BAGS_BEGIN + 20;
		else if (identifier == "general3bag.end")       result = EQ::invbag::GENERAL_BAGS_BEGIN + 29;
		else if (identifier == "general4bag.begin")     result = EQ::invbag::GENERAL_BAGS_BEGIN + 30;
		else if (identifier == "general4bag.end")       result = EQ::invbag::GENERAL_BAGS_BEGIN + 39;
		else if (identifier == "general5bag.begin")     result = EQ::invbag::GENERAL_BAGS_BEGIN + 40;
		else if (identifier == "general5bag.end")       result = EQ::invbag::GENERAL_BAGS_BEGIN + 49;
		else if (identifier == "general6bag.begin")     result = EQ::invbag::GENERAL_BAGS_BEGIN + 50;
		else if (identifier == "general6bag.end")       result = EQ::invbag::GENERAL_BAGS_BEGIN + 59;
		else if (identifier == "general7bag.begin")     result = EQ::invbag::GENERAL_BAGS_BEGIN + 60;
		else if (identifier == "general7bag.end")       result = EQ::invbag::GENERAL_BAGS_BEGIN + 69;
		else if (identifier == "general8bag.begin")     result = EQ::invbag::GENERAL_BAGS_BEGIN + 70;
		else if (identifier == "general8bag.end")       result = EQ::invbag::GENERAL_BAGS_BEGIN + 79;
		else if (identifier == "general9bag.begin")     result = EQ::invbag::GENERAL_BAGS_BEGIN + 80;
		else if (identifier == "general9bag.end")       result = EQ::invbag::GENERAL_BAGS_BEGIN + 89;
		else if (identifier == "general10bag.begin")    result = EQ::invbag::GENERAL_BAGS_BEGIN + 90;
		else if (identifier == "general10bag.end")      result = EQ::invbag::GENERAL_BAGS_BEGIN + 99;
		else if (identifier == "cursorbag.begin")       result = EQ::invbag::CURSOR_BAG_BEGIN;
		else if (identifier == "cursorbag.end")         result = EQ::invbag::CURSOR_BAG_END;
		else if (identifier == "bank.begin")            result = EQ::invslot::BANK_BEGIN;
		else if (identifier == "bank.end")              result = EQ::invslot::BANK_END;
		else if (identifier == "bankbags.begin")        result = EQ::invbag::BANK_BAGS_BEGIN;
		else if (identifier == "bankbags.end")          result = EQ::invbag::BANK_BAGS_END;
		else if (identifier == "sharedbank.begin")      result = EQ::invslot::SHARED_BANK_BEGIN;
		else if (identifier == "sharedbank.end")        result = EQ::invslot::SHARED_BANK_END;
		else if (identifier == "sharedbankbags.begin")  result = EQ::invbag::SHARED_BANK_BAGS_BEGIN;
		else if (identifier == "sharedbankbags.end")    result = EQ::invbag::SHARED_BANK_BAGS_END;
		else if (identifier == "bagslot.begin")         result = EQ::invbag::SLOT_BEGIN;
		else if (identifier == "bagslot.end")           result = EQ::invbag::SLOT_END;
		else if (identifier == "augsocket.begin")       result = EQ::invaug::SOCKET_BEGIN;
		else if (identifier == "augsocket.end")         result = EQ::invaug::SOCKET_END;
	}

	return result;
}

void Perl__castspell(int spell_id, int target_id)
{
	quest_manager.castspell(spell_id, target_id);
}

void Perl__selfcast(int spell_id)
{
	quest_manager.selfcast(spell_id);
}

void Perl__addloot(int item_id)
{
	quest_manager.addloot(item_id);
}

void Perl__addloot(int item_id, int charges)
{
	quest_manager.addloot(item_id, charges);
}

void Perl__addloot(int item_id, int charges, bool equip_item)
{
	quest_manager.addloot(item_id, charges, equip_item);
}

void Perl__zone(const char* zone_name)
{
	quest_manager.Zone(zone_name);
}

void Perl__zonegroup(const char* zone_name)
{
	quest_manager.ZoneGroup(zone_name);
}

void Perl__zoneraid(const char* zone_name)
{
	quest_manager.ZoneRaid(zone_name);
}

bool Perl__hastimer(const char* timer_name)
{
	return quest_manager.hastimer(timer_name);
}

uint32_t Perl__getremainingtimeMS(const char* timer_name)
{
	return quest_manager.getremainingtimeMS(timer_name);
}

uint32_t Perl__gettimerdurationMS(const char* timer_name)
{
	return quest_manager.gettimerdurationMS(timer_name);
}

void Perl__settimer(const char* timer_name, int seconds)
{
	quest_manager.settimer(timer_name, seconds);
}

void Perl__settimerMS(const char* timer_name, int milliseconds)
{
	quest_manager.settimerMS(timer_name, milliseconds);
}

void Perl__stoptimer(const char* timer_name)
{
	quest_manager.stoptimer(timer_name);
}

void Perl__stopalltimers()
{
	quest_manager.stopalltimers();
}

void Perl__emote(const char* message)
{
	quest_manager.emote(message);
}

void Perl__shout(const char* message)
{
	quest_manager.shout(message);
}

void Perl__shout2(const char* message)
{
	quest_manager.shout2(message);
}

void Perl__gmsay(const char* message)
{
	quest_manager.gmsay(message, Chat::LightGray, false, 0, AccountStatus::QuestTroupe);
}

void Perl__gmsay(const char* message, int color_id)
{
	quest_manager.gmsay(message, color_id, false, 0, AccountStatus::QuestTroupe);
}

void Perl__gmsay(const char* message, int color_id, bool send_to_world)
{
	quest_manager.gmsay(message, color_id, send_to_world, 0, AccountStatus::QuestTroupe);
}

void Perl__gmsay(const char* message, int color_id, bool send_to_world, int to_guilddbid)
{
	quest_manager.gmsay(message, color_id, send_to_world, to_guilddbid, AccountStatus::QuestTroupe);
}

void Perl__gmsay(const char* message, int color_id, bool send_to_world, int to_guilddbid, int to_minstatus)
{
	quest_manager.gmsay(message, color_id, send_to_world, to_guilddbid, to_minstatus);
}

void Perl__depop()
{
	quest_manager.depop();
}

void Perl__depop(int npc_type_id)
{
	quest_manager.depop(npc_type_id);
}

void Perl__depop_withtimer()
{
	quest_manager.depop_withtimer();
}

void Perl__depop_withtimer(int npc_type_id)
{
	quest_manager.depop_withtimer(npc_type_id);
}

void Perl__depopall()
{
	quest_manager.depopall();
}

void Perl__depopall(int npc_type_id)
{
	quest_manager.depopall(npc_type_id);
}

void Perl__settarget(const char* target_enum, int target_id)
{
	quest_manager.settarget(target_enum, target_id);
}

void Perl__follow(int entity_id)
{
	quest_manager.follow(entity_id, 10);
}

void Perl__follow(int entity_id, int distance)
{
	quest_manager.follow(entity_id, distance);
}

void Perl__sfollow()
{
	quest_manager.sfollow();
}

void Perl__changedeity(int deity_id)
{
	quest_manager.changedeity(deity_id);
}

void Perl__exp(int amount)
{
	quest_manager.exp(amount);
}

void Perl__level(int new_level)
{
	quest_manager.level(new_level);
}

void Perl__traindisc(int tome_item_id)
{
	quest_manager.traindisc(tome_item_id);
}

bool Perl__isdisctome(int item_id)
{
	return quest_manager.isdisctome(item_id);
}

std::string Perl__getracename(uint16 race_id)
{
	return quest_manager.getracename(race_id);
}

std::string Perl__getspellname(uint32 spell_id)
{
	return quest_manager.getspellname(spell_id);
}

int Perl__get_spell_level(uint32_t spell_id, int class_id)
{
	int spell_level = IsValidSpell(spell_id) ? GetSpellLevel(spell_id, class_id) : 0;
	return (spell_level > RuleI(Character, MaxLevel)) ? 0 : spell_level;
}

std::string Perl__getskillname(int skill_id)
{
	return quest_manager.getskillname(skill_id);
}

void Perl__safemove()
{
	quest_manager.safemove();
}

void Perl__rain(int weather)
{
	quest_manager.rain(weather);
}

void Perl__snow(int weather)
{
	quest_manager.snow(weather);
}

void Perl__surname(std::string last_name)
{
	quest_manager.surname(last_name);
}

void Perl__permaclass(int class_id)
{
	quest_manager.permaclass(class_id);
}

void Perl__permarace(int race_id)
{
	quest_manager.permarace(race_id);
}

void Perl__permagender(int gender_id)
{
	quest_manager.permagender(gender_id);
}

int Perl__scribespells(int max_level)
{
	return quest_manager.scribespells(max_level);
}

int Perl__scribespells(int max_level, int min_level)
{
	return quest_manager.scribespells(max_level, min_level);
}

int Perl__traindiscs(int max_level)
{
	return quest_manager.traindiscs(max_level);
}

int Perl__traindiscs(int max_level, int min_level)
{
	return quest_manager.traindiscs(max_level, min_level);
}

void Perl__unscribespells()
{
	quest_manager.unscribespells();
}

void Perl__untraindiscs()
{
	quest_manager.untraindiscs();
}

void Perl__givecash(uint32 copper)
{
	quest_manager.givecash(copper);
}

void Perl__givecash(uint32 copper, uint32 silver)
{
	quest_manager.givecash(copper, silver);
}

void Perl__givecash(uint32 copper, uint32 silver, uint32 gold)
{
	quest_manager.givecash(copper, silver, gold);
}

void Perl__givecash(uint32 copper, uint32 silver, uint32 gold, uint32 platinum)
{
	quest_manager.givecash(copper, silver, gold, platinum);
}

void Perl__pvp(const char* mode)
{
	quest_manager.pvp(mode);
}

void Perl__movepc(int zone_id, float x, float y, float z)
{
	quest_manager.movepc(zone_id, x, y, z, 0.0f);
}

void Perl__movepc(int zone_id, float x, float y, float z, float heading)
{
	quest_manager.movepc(zone_id, x, y, z, heading);
}

void Perl__gmmove(float x, float y, float z)
{
	quest_manager.gmmove(x, y, z);
}

void Perl__movegrp(int zone_id, float x, float y, float z)
{
	quest_manager.movegrp(zone_id, x, y, z);
}

void Perl__doanim(int animation_id)
{
	quest_manager.doanim(animation_id);
}

void Perl__addskill(int skill_id, int value)
{
	quest_manager.addskill(skill_id, value);
}

void Perl__setlanguage(int skill_id, int value)
{
	quest_manager.setlanguage(skill_id, value);
}

void Perl__setskill(int skill_id, int value)
{
	quest_manager.setskill(skill_id, value);
}

void Perl__setallskill(int value)
{
	quest_manager.setallskill(value);
}

void Perl__attack(const char* client_name)
{
	quest_manager.attack(client_name);
}

void Perl__attacknpc(int npc_entity_id)
{
	quest_manager.attacknpc(npc_entity_id);
}

void Perl__attacknpctype(int npc_type_id)
{
	quest_manager.attacknpctype(npc_type_id);
}

void Perl__save()
{
	quest_manager.save();
}

void Perl__faction(int faction_id, int value)
{
	quest_manager.faction(faction_id, value, 0);
}

void Perl__faction(int faction_id, int value, int temp)
{
	quest_manager.faction(faction_id, value, temp);
}

void Perl__rewardfaction(int faction_id, int value)
{
	quest_manager.rewardfaction(faction_id, value);
}

void Perl__setsky(uint8 new_sky)
{
	quest_manager.setsky(new_sky);
}

void Perl__setguild(uint32_t guild_id, uint8_t guild_rank_id)
{
	quest_manager.setguild(guild_id, guild_rank_id);
}

void Perl__createguild(const char* guild_name, const char* leader_name)
{
	quest_manager.CreateGuild(guild_name, leader_name);
}

void Perl__settime(int new_hour, int new_min)
{
	quest_manager.settime(new_hour, new_min);
}

void Perl__settime(int new_hour, int new_min, bool update_world)
{
	quest_manager.settime(new_hour, new_min, update_world);
}

void Perl__itemlink(int item_id)
{
	quest_manager.itemlink(item_id);
}

void Perl__signalwith(int npc_id, int signal_id)
{
	quest_manager.signalwith(npc_id, signal_id);
}

void Perl__signalwith(int npc_id, int signal_id, int wait_ms)
{
	quest_manager.signalwith(npc_id, signal_id, wait_ms);
}

void Perl__signal(int npc_id)
{
	quest_manager.signal(npc_id);
}

void Perl__signal(int npc_id, int wait_ms)
{
	quest_manager.signal(npc_id, wait_ms);
}

void Perl__setglobal(const char* key, const char* value, int options, const char* duration)
{
	quest_manager.setglobal(key, value, options, duration);
}

void Perl__targlobal(const char* key, const char* value, const char* duration, int npc_id, int char_id, int zone_id)
{
	quest_manager.targlobal(key, value, duration, npc_id, char_id, zone_id);
}

void Perl__delglobal(const char* key)
{
	quest_manager.delglobal(key);
}

void Perl__ding()
{
	quest_manager.ding();
}

void Perl__rebind(int zone_id, float x, float y, float z)
{
	quest_manager.rebind(zone_id, glm::vec3(x, y, z));
}

void Perl__rebind(int zone_id, float x, float y, float z, float heading)
{
	quest_manager.rebind(zone_id, glm::vec4(x, y, z, heading));
}

void Perl__start(int waypoint)
{
	quest_manager.start(waypoint);
}

void Perl__stop()
{
	quest_manager.stop();
}

void Perl__pause(int duration_ms)
{
	quest_manager.pause(duration_ms);
}

void Perl__moveto(float x, float y, float z)
{
	quest_manager.moveto(glm::vec4(x, y, z, 0.0f), false);
}

void Perl__moveto(float x, float y, float z, float h)
{
	quest_manager.moveto(glm::vec4(x, y, z, h), false);
}

void Perl__moveto(float x, float y, float z, float h, bool save_guard_location)
{
	quest_manager.moveto(glm::vec4(x, y, z, h), save_guard_location);
}

void Perl__resume()
{
	quest_manager.resume();
}

void Perl__setnexthpevent(int at_mob_percentage)
{
	quest_manager.setnexthpevent(at_mob_percentage);
}

void Perl__setnextinchpevent(int at_mob_percentage)
{
	quest_manager.setnextinchpevent(at_mob_percentage);
}

void Perl__sethp(int mob_health_percentage)
{
	quest_manager.sethp(mob_health_percentage);
}

void Perl__respawn(int npc_type_id, int grid_id)
{
	quest_manager.respawn(npc_type_id, grid_id);
}

perl::scalar Perl__ChooseRandom(perl::array options)
{
	int index = zone->random.Int(0, static_cast<int>(options.size()) - 1);
	return options[index];
}

void Perl__set_proximity(float min_x, float max_x, float min_y, float max_y)
{
	quest_manager.set_proximity(min_x, max_x, min_y, max_y);
}

void Perl__set_proximity(float min_x, float max_x, float min_y, float max_y, float min_z, float max_z)
{
	quest_manager.set_proximity(min_x, max_x, min_y, max_y, min_z, max_z);
}

void Perl__set_proximity(float min_x, float max_x, float min_y, float max_y, float min_z, float max_z, bool say)
{
	quest_manager.set_proximity(min_x, max_x, min_y, max_y, min_z, max_z, say);
}

void Perl__clear_proximity()
{
	quest_manager.clear_proximity();
}

void Perl__enable_proximity_say()
{
	quest_manager.enable_proximity_say();
}

void Perl__disable_proximity_say()
{
	quest_manager.disable_proximity_say();
}

void Perl__setanim(int npc_type_id, int appearance_number)
{
	quest_manager.setanim(npc_type_id, appearance_number);
}

void Perl__showgrid(int grid_id)
{
	quest_manager.showgrid(grid_id);
}

void Perl__spawn_condition(const char* zone_short, uint16 condition_id, int16 value)
{
	quest_manager.spawn_condition(zone_short, condition_id, value);
}

int Perl__get_spawn_condition(const char* zone_short, uint16 condition_id)
{
	return quest_manager.get_spawn_condition(zone_short, condition_id);
}

void Perl__toggle_spawn_event(int event_id, bool is_enabled, bool is_strict, bool reset_base)
{
	quest_manager.toggle_spawn_event(event_id, is_enabled, is_strict, reset_base);
}

bool Perl__has_zone_flag(int zone_id)
{
	return quest_manager.has_zone_flag(zone_id);
}

void Perl__set_zone_flag(int zone_id)
{
	quest_manager.set_zone_flag(zone_id);
}

void Perl__clear_zone_flag(int zone_id)
{
	quest_manager.clear_zone_flag(zone_id);
}

bool Perl__summonburiedplayercorpse(uint32 char_id, float dest_x, float dest_y, float dest_z, float dest_heading)
{
	auto position = glm::vec4(dest_x, dest_y, dest_z, dest_heading);
	return quest_manager.summonburiedplayercorpse(char_id, position);
}

bool Perl__summonallplayercorpses(uint32 char_id, float dest_x, float dest_y, float dest_z, float dest_heading)
{
	auto position = glm::vec4(dest_x, dest_y, dest_z, dest_heading);
	return quest_manager.summonallplayercorpses(char_id, position);
}

int Perl__getplayercorpsecount(uint32 char_id)
{
	return quest_manager.getplayercorpsecount(char_id);
}

int Perl__getplayercorpsecountbyzoneid(uint32 char_id, uint32 zone_id)
{
	return quest_manager.getplayercorpsecountbyzoneid(char_id, zone_id);
}

int Perl__getplayerburiedcorpsecount(uint32 char_id)
{
	return quest_manager.getplayerburiedcorpsecount(char_id);
}

bool Perl__buryplayercorpse(uint32 char_id)
{
	return quest_manager.buryplayercorpse(char_id);
}

void Perl__forcedooropen(uint32 door_id)
{
	quest_manager.forcedooropen(door_id, false);
}

void Perl__forcedooropen(uint32 door_id, bool alt_mode)
{
	quest_manager.forcedooropen(door_id, alt_mode);
}

void Perl__forcedoorclose(uint32 door_id)
{
	quest_manager.forcedoorclose(door_id, false);
}

void Perl__forcedoorclose(uint32 door_id, bool alt_mode)
{
	quest_manager.forcedoorclose(door_id, alt_mode);
}

void Perl__toggledoorstate(uint32 door_id)
{
	quest_manager.toggledoorstate(door_id);
}

bool Perl__isdooropen(uint32 door_id)
{
	return quest_manager.isdooropen(door_id);
}

void Perl__depopzone(bool start_spawn_status)
{
	quest_manager.depopzone(start_spawn_status);
}

void Perl__repopzone()
{
	quest_manager.repopzone();
}

void Perl__processmobswhilezoneempty(bool on)
{
	quest_manager.processmobswhilezoneempty(on);
}

void Perl__npcrace(int race_id)
{
	quest_manager.npcrace(race_id);
}

void Perl__npcgender(int gender_id)
{
	quest_manager.npcgender(gender_id);
}

void Perl__npcsize(int size)
{
	quest_manager.npcsize(size);
}

void Perl__npctexture(int texture_id)
{
	quest_manager.npctexture(texture_id);
}

void Perl__playerrace(int race_id)
{
	quest_manager.playerrace(race_id);
}

void Perl__playergender(int gender_id)
{
	quest_manager.playergender(gender_id);
}

void Perl__playersize(int newsize)
{
	quest_manager.playersize(newsize);
}

void Perl__playertexture(int texture_id)
{
	quest_manager.playertexture(texture_id);
}

void Perl__playerfeature(char* feature, int value)
{
	quest_manager.playerfeature(feature, value);
}

void Perl__npcfeature(char* feature, int value)
{
	quest_manager.npcfeature(feature, value);
}

#ifdef BOTS

int Perl__createbotcount()
{
	return quest_manager.createbotcount();
}

int Perl__createbotcount(uint8 class_id)
{
	return quest_manager.createbotcount(class_id);
}

int Perl__spawnbotcount()
{
	return quest_manager.spawnbotcount();
}

int Perl__spawnbotcount(uint8 class_id)
{
	return quest_manager.spawnbotcount(class_id);
}

bool Perl__botquest()
{
	return quest_manager.botquest();
}

bool Perl__createBot(const char* firstname, const char* lastname, int level, int race_id, int class_id, int gender_id)
{
	return quest_manager.createBot(firstname, lastname, level, race_id, class_id, gender_id);
}

#endif //BOTS

void Perl__popup(const char* window_title, const char* message)
{
	quest_manager.popup(window_title, message, 0, 0, 0);
}

void Perl__popup(const char* window_title, const char* message, int popup_id)
{
	quest_manager.popup(window_title, message, popup_id, 0, 0);
}

void Perl__popup(const char* window_title, const char* message, int popup_id, int buttons)
{
	quest_manager.popup(window_title, message, popup_id, buttons, 0);
}

void Perl__popup(const char* window_title, const char* message, int popup_id, int buttons, int duration)
{
	quest_manager.popup(window_title, message, popup_id, buttons, duration);
}

void Perl__clearspawntimers()
{
	quest_manager.clearspawntimers();
}

void Perl__ze(int emote_color_id, const char* message)
{
	return quest_manager.ze(emote_color_id, message);
}

void Perl__we(int emote_color_id, const char* message)
{
	quest_manager.we(emote_color_id, message);
}

void Perl__message(int color, const char* message)
{
	quest_manager.message(color, message);
}

void Perl__whisper(const char* message)
{
	quest_manager.whisper(message);
}

int Perl__getlevel(uint8 type)
{
	return quest_manager.getlevel(type);
}

int Perl__CreateGroundObject(uint32_t item_id, float x, float y, float z, float heading)
{
	return quest_manager.CreateGroundObject(item_id, glm::vec4(x, y, z, heading));
}

int Perl__CreateGroundObject(uint32_t item_id, float x, float y, float z, float heading, uint32_t decay_time_ms)
{
	return quest_manager.CreateGroundObject(item_id, glm::vec4(x, y, z, heading), decay_time_ms);
}

int Perl__CreateGroundObjectFromModel(const char* modelname, float x, float y, float z, float heading)
{
	return quest_manager.CreateGroundObjectFromModel(modelname, glm::vec4(x, y, z, heading));
}

int Perl__CreateGroundObjectFromModel(const char* modelname, float x, float y, float z, float heading, uint8_t object_type)
{
	return quest_manager.CreateGroundObjectFromModel(modelname, glm::vec4(x, y, z, heading), object_type);
}

int Perl__CreateGroundObjectFromModel(const char* modelname, float x, float y, float z, float heading, uint8_t object_type, uint32_t decay_time_ms)
{
	return quest_manager.CreateGroundObjectFromModel(modelname, glm::vec4(x, y, z, heading), object_type, decay_time_ms);
}

int Perl__CreateDoor(const char* modelname, float x, float y, float z, float heading)
{
	return quest_manager.CreateDoor(modelname, x, y, z, heading, 58, 100);
}

int Perl__CreateDoor(const char* modelname, float x, float y, float z, float heading, uint8_t object_type)
{
	return quest_manager.CreateDoor(modelname, x, y, z, heading, object_type, 100);
}

int Perl__CreateDoor(const char* modelname, float x, float y, float z, float heading, uint8_t object_type, uint16_t size)
{
	return quest_manager.CreateDoor(modelname, x, y, z, heading, object_type, size);
}

void Perl__ModifyNPCStat(const char* key, const char* value)
{
	quest_manager.ModifyNPCStat(key, value);
}

int Perl__collectitems(uint32_t item_id, bool remove_item)
{
	return quest_manager.collectitems(item_id, remove_item);
}

int Perl__countitem(uint32_t item_id)
{
	return quest_manager.countitem(item_id);
}

void Perl__removeitem(uint32_t item_id)
{
	quest_manager.removeitem(item_id);
}

void Perl__removeitem(uint32_t item_id, int quantity)
{
	quest_manager.removeitem(item_id, quantity);
}

std::string Perl__getitemname(uint32 item_id)
{
	return quest_manager.getitemname(item_id);
}

std::string Perl__getnpcnamebyid(uint32 npc_id)
{
	return quest_manager.getnpcnamebyid(npc_id);
}

void Perl__UpdateSpawnTimer(uint32 id, uint32 new_time)
{
	quest_manager.UpdateSpawnTimer(id, new_time);
}

void Perl__MerchantSetItem(uint32 npc_id, uint32 item_id)
{
	quest_manager.MerchantSetItem(npc_id, item_id);
}

void Perl__MerchantSetItem(uint32 npc_id, uint32 item_id, uint32 quantity)
{
	quest_manager.MerchantSetItem(npc_id, item_id, quantity);
}

uint32_t Perl__MerchantCountItem(uint32 npc_id, uint32 item_id)
{
	return quest_manager.MerchantCountItem(npc_id, item_id);
}

std::string Perl__varlink(int item_id)
{
	char text[250] = { 0 };
	return quest_manager.varlink(text, item_id);
}

std::string Perl__saylink(const char* text)
{
	// const cast is safe since, target api doesn't modify it
	return quest_manager.saylink(const_cast<char*>(text), false, text);
}

std::string Perl__saylink(const char* text, bool silent)
{
	return quest_manager.saylink(const_cast<char*>(text), silent, text);
}

std::string Perl__saylink(const char* text, bool silent, const char* link_name)
{
	return quest_manager.saylink(const_cast<char*>(text), silent, link_name);
}

std::string Perl__getcharnamebyid(uint32 char_id)
{
	return quest_manager.getcharnamebyid(char_id);
}

uint32_t Perl__getcharidbyname(const char* name)
{
	return quest_manager.getcharidbyname(name);
}

std::string Perl__getclassname(uint8 class_id)
{
	return quest_manager.getclassname(class_id);
}

std::string Perl__getclassname(uint8 class_id, uint8 level)
{
	return quest_manager.getclassname(class_id, level);
}

uint32 Perl__getcurrencyitemid(uint32 currency_id)
{
	return quest_manager.getcurrencyitemid(currency_id);
}

uint32 Perl__getcurrencyid(uint32 item_id)
{
	return quest_manager.getcurrencyid(item_id);
}

std::string Perl__getguildnamebyid(int guild_id)
{
	return quest_manager.getguildnamebyid(guild_id);
}

int Perl__getguildidbycharid(uint32 char_id)
{
	return quest_manager.getguildidbycharid(char_id);
}

int Perl__getgroupidbycharid(uint32 char_id)
{
	return quest_manager.getgroupidbycharid(char_id);
}

int Perl__getraididbycharid(uint32 char_id)
{
	return quest_manager.getraididbycharid(char_id);
}

void Perl__SetRunning(bool is_running)
{
	quest_manager.SetRunning(is_running);
}

bool Perl__IsRunning()
{
	return quest_manager.IsRunning();
}

bool Perl__IsEffectInSpell(uint32 spell_id, uint32 effect_id)
{
	return IsEffectInSpell(spell_id, effect_id);
}

bool Perl__IsBeneficialSpell(uint16 spell_id)
{
	return BeneficialSpell(spell_id);
}

int Perl__GetSpellResistType(uint16 spell_id)
{
	return GetSpellResistType(spell_id);
}

int Perl__GetSpellTargetType(uint16 spell_id)
{
	return GetSpellTargetType(spell_id);
}

void Perl__FlyMode(GravityBehavior flymode)
{
	quest_manager.FlyMode(flymode);
}

int Perl__FactionValue()
{
	return quest_manager.FactionValue();
}

void Perl__enabletitle(int title_set)
{
	quest_manager.enabletitle(title_set);
}

bool Perl__checktitle(int title_set)
{
	return quest_manager.checktitle(title_set);
}

void Perl__removetitle(int title_set)
{
	quest_manager.removetitle(title_set);
}

void Perl__wearchange(uint8 slot, uint16 texture_id)
{
	quest_manager.wearchange(slot, texture_id);
}

void Perl__wearchange(uint8 slot, uint16 texture_id, uint32 hero_forge_model_id)
{
	quest_manager.wearchange(slot, texture_id, hero_forge_model_id);
}

void Perl__wearchange(uint8 slot, uint16 texture_id, uint32 hero_forge_model_id, uint32 elite_material_id)
{
	quest_manager.wearchange(slot, texture_id, hero_forge_model_id, elite_material_id);
}

void Perl__voicetell(const char* client_name, int macro_id, int race_id, int gender_id)
{
	quest_manager.voicetell(client_name, macro_id, race_id, gender_id);
}

void Perl__LearnRecipe(int recipe_id)
{
	quest_manager.LearnRecipe(recipe_id);
}

void Perl__SendMail(const char* to, const char* from, const char* subject, const char* message)
{
	quest_manager.SendMail(to, from, subject, message);
}

int Perl__GetZoneID(const char* zone)
{
	return quest_manager.GetZoneID(zone);
}

std::string Perl__GetZoneLongName(std::string zone)
{
	return quest_manager.GetZoneLongName(zone);
}

std::string Perl__GetZoneLongNameByID(uint32 zone_id)
{
	return quest_manager.GetZoneLongNameByID(zone_id);
}

std::string Perl__GetZoneShortName(uint32 zone_id)
{
	return quest_manager.GetZoneShortName(zone_id);
}

uint32 Perl__GetTimeSeconds()
{
	return Timer::GetTimeSeconds();
}

bool Perl__enablerecipe(uint32 recipe_id)
{
	return quest_manager.EnableRecipe(recipe_id);
}

bool Perl__disablerecipe(uint32 recipe_id)
{
	return quest_manager.DisableRecipe(recipe_id);
}

void Perl__clear_npctype_cache(int npc_type_id)
{
	quest_manager.ClearNPCTypeCache(npc_type_id);
}

void Perl__reloadzonestaticdata()
{
	quest_manager.ReloadZoneStaticData();
}

void Perl__qs_send_query(std::string query)
{
	QServ->SendQuery(std::move(query));
}

void Perl__qs_player_event(int char_id, std::string message)
{
	QServ->PlayerLogEvent(Player_Log_Quest, char_id, message);
}

void Perl__log(int category, const char* message)
{
	if (category < Logs::None || category >= Logs::MaxCategoryID)
		return;

	Log(Logs::General, static_cast<Logs::LogCategory>(category), message);
}

void Perl__debug(const char* message)
{
	Log(Logs::General, Logs::QuestDebug, message);
}

void Perl__debug(const char* message, int level)
{
	if (level < Logs::General || level > Logs::Detail)
		return;

	Log(static_cast<Logs::DebugLevel>(level), Logs::QuestDebug, message);
}

void Perl__log_combat(const char* message)
{
	Log(Logs::General, Logs::Combat, message);
}

void Perl__UpdateZoneHeader(std::string key, std::string value)
{
	quest_manager.UpdateZoneHeader(key, value);
}

void Perl__set_rule(const char* rule_name, const char* rule_value)
{
	RuleManager::Instance()->SetRule(rule_name, rule_value);
}

std::string Perl__get_rule(const char* rule_name)
{
	std::string rule_value;
	RuleManager::Instance()->GetRule(rule_name, rule_value);
	return rule_value;
}

std::string Perl__get_data(std::string bucket_key)
{
	return DataBucket::GetData(bucket_key);
}

std::string Perl__get_data_expires(std::string bucket_key)
{
	return DataBucket::GetDataExpires(bucket_key);
}

void Perl__set_data(std::string key, std::string value)
{
	DataBucket::SetData(key, value);
}

void Perl__set_data(std::string key, std::string value, std::string expires_at)
{
	DataBucket::SetData(key, value, expires_at);
}

bool Perl__delete_data(std::string bucket_key)
{
	return DataBucket::DeleteData(bucket_key);
}

bool Perl__IsClassicEnabled()
{
	return content_service.IsClassicEnabled();
}

bool Perl__IsTheRuinsOfKunarkEnabled()
{
	return content_service.IsTheRuinsOfKunarkEnabled();
}

bool Perl__IsTheScarsOfVeliousEnabled()
{
	return content_service.IsTheScarsOfVeliousEnabled();
}

bool Perl__IsTheShadowsOfLuclinEnabled()
{
	return content_service.IsTheShadowsOfLuclinEnabled();
}

bool Perl__IsThePlanesOfPowerEnabled()
{
	return content_service.IsThePlanesOfPowerEnabled();
}

bool Perl__IsTheLegacyOfYkeshaEnabled()
{
	return content_service.IsTheLegacyOfYkeshaEnabled();
}

bool Perl__IsLostDungeonsOfNorrathEnabled()
{
	return content_service.IsLostDungeonsOfNorrathEnabled();
}

bool Perl__IsGatesOfDiscordEnabled()
{
	return content_service.IsGatesOfDiscordEnabled();
}

bool Perl__IsOmensOfWarEnabled()
{
	return content_service.IsOmensOfWarEnabled();
}

bool Perl__IsDragonsOfNorrathEnabled()
{
	return content_service.IsDragonsOfNorrathEnabled();
}

bool Perl__IsDepthsOfDarkhollowEnabled()
{
	return content_service.IsDepthsOfDarkhollowEnabled();
}

bool Perl__IsProphecyOfRoEnabled()
{
	return content_service.IsProphecyOfRoEnabled();
}

bool Perl__IsTheSerpentsSpineEnabled()
{
	return content_service.IsTheSerpentsSpineEnabled();
}

bool Perl__IsTheBuriedSeaEnabled()
{
	return content_service.IsTheBuriedSeaEnabled();
}

bool Perl__IsSecretsOfFaydwerEnabled()
{
	return content_service.IsSecretsOfFaydwerEnabled();
}

bool Perl__IsSeedsOfDestructionEnabled()
{
	return content_service.IsSeedsOfDestructionEnabled();
}

bool Perl__IsUnderfootEnabled()
{
	return content_service.IsUnderfootEnabled();
}

bool Perl__IsHouseOfThuleEnabled()
{
	return content_service.IsHouseOfThuleEnabled();
}

bool Perl__IsVeilOfAlarisEnabled()
{
	return content_service.IsVeilOfAlarisEnabled();
}

bool Perl__IsRainOfFearEnabled()
{
	return content_service.IsRainOfFearEnabled();
}

bool Perl__IsCallOfTheForsakenEnabled()
{
	return content_service.IsCallOfTheForsakenEnabled();
}

bool Perl__IsTheDarkenedSeaEnabled()
{
	return content_service.IsTheDarkenedSeaEnabled();
}

bool Perl__IsTheBrokenMirrorEnabled()
{
	return content_service.IsTheBrokenMirrorEnabled();
}

bool Perl__IsEmpiresOfKunarkEnabled()
{
	return content_service.IsEmpiresOfKunarkEnabled();
}

bool Perl__IsRingOfScaleEnabled()
{
	return content_service.IsRingOfScaleEnabled();
}

bool Perl__IsTheBurningLandsEnabled()
{
	return content_service.IsTheBurningLandsEnabled();
}

bool Perl__IsTormentOfVeliousEnabled()
{
	return content_service.IsTormentOfVeliousEnabled();
}

bool Perl__IsCurrentExpansionClassic()
{
	return content_service.IsCurrentExpansionClassic();
}

bool Perl__IsCurrentExpansionTheRuinsOfKunark()
{
	return content_service.IsCurrentExpansionTheRuinsOfKunark();
}

bool Perl__IsCurrentExpansionTheScarsOfVelious()
{
	return content_service.IsCurrentExpansionTheScarsOfVelious();
}

bool Perl__IsCurrentExpansionTheShadowsOfLuclin()
{
	return content_service.IsCurrentExpansionTheShadowsOfLuclin();
}

bool Perl__IsCurrentExpansionThePlanesOfPower()
{
	return content_service.IsCurrentExpansionThePlanesOfPower();
}

bool Perl__IsCurrentExpansionTheLegacyOfYkesha()
{
	return content_service.IsCurrentExpansionTheLegacyOfYkesha();
}

bool Perl__IsCurrentExpansionLostDungeonsOfNorrath()
{
	return content_service.IsCurrentExpansionLostDungeonsOfNorrath();
}

bool Perl__IsCurrentExpansionGatesOfDiscord()
{
	return content_service.IsCurrentExpansionGatesOfDiscord();
}

bool Perl__IsCurrentExpansionOmensOfWar()
{
	return content_service.IsCurrentExpansionOmensOfWar();
}

bool Perl__IsCurrentExpansionDragonsOfNorrath()
{
	return content_service.IsCurrentExpansionDragonsOfNorrath();
}

bool Perl__IsCurrentExpansionDepthsOfDarkhollow()
{
	return content_service.IsCurrentExpansionDepthsOfDarkhollow();
}

bool Perl__IsCurrentExpansionProphecyOfRo()
{
	return content_service.IsCurrentExpansionProphecyOfRo();
}

bool Perl__IsCurrentExpansionTheSerpentsSpine()
{
	return content_service.IsCurrentExpansionTheSerpentsSpine();
}

bool Perl__IsCurrentExpansionTheBuriedSea()
{
	return content_service.IsCurrentExpansionTheBuriedSea();
}

bool Perl__IsCurrentExpansionSecretsOfFaydwer()
{
	return content_service.IsCurrentExpansionSecretsOfFaydwer();
}

bool Perl__IsCurrentExpansionSeedsOfDestruction()
{
	return content_service.IsCurrentExpansionSeedsOfDestruction();
}

bool Perl__IsCurrentExpansionUnderfoot()
{
	return content_service.IsCurrentExpansionUnderfoot();
}

bool Perl__IsCurrentExpansionHouseOfThule()
{
	return content_service.IsCurrentExpansionHouseOfThule();
}

bool Perl__IsCurrentExpansionVeilOfAlaris()
{
	return content_service.IsCurrentExpansionVeilOfAlaris();
}

bool Perl__IsCurrentExpansionRainOfFear()
{
	return content_service.IsCurrentExpansionRainOfFear();
}

bool Perl__IsCurrentExpansionCallOfTheForsaken()
{
	return content_service.IsCurrentExpansionCallOfTheForsaken();
}

bool Perl__IsCurrentExpansionTheDarkenedSea()
{
	return content_service.IsCurrentExpansionTheDarkenedSea();
}

bool Perl__IsCurrentExpansionTheBrokenMirror()
{
	return content_service.IsCurrentExpansionTheBrokenMirror();
}

bool Perl__IsCurrentExpansionEmpiresOfKunark()
{
	return content_service.IsCurrentExpansionEmpiresOfKunark();
}

bool Perl__IsCurrentExpansionRingOfScale()
{
	return content_service.IsCurrentExpansionRingOfScale();
}

bool Perl__IsCurrentExpansionTheBurningLands()
{
	return content_service.IsCurrentExpansionTheBurningLands();
}

bool Perl__IsCurrentExpansionTormentOfVelious()
{
	return content_service.IsCurrentExpansionTormentOfVelious();
}

bool Perl__IsContentFlagEnabled(std::string flag_name)
{
	return content_service.IsContentFlagEnabled(flag_name);
}

void Perl__SetContentFlag(std::string flag_name, bool enabled)
{
	content_service.SetContentFlag(flag_name, enabled);
}

EQ::ItemInstance* Perl__createitem(uint32 item_id)
{
	return database.CreateItem(item_id);
}

EQ::ItemInstance* Perl__createitem(uint32 item_id, int16 charges)
{
	return database.CreateItem(item_id, charges);
}

EQ::ItemInstance* Perl__createitem(uint32 item_id, int16 charges, uint32 augment_one)
{
	return database.CreateItem(item_id, charges, augment_one);
}

EQ::ItemInstance* Perl__createitem(uint32 item_id, int16 charges, uint32 augment_one, uint32 augment_two)
{
	return database.CreateItem(item_id, charges, augment_one, augment_two);
}

EQ::ItemInstance* Perl__createitem(uint32 item_id, int16 charges, uint32 augment_one, uint32 augment_two, uint32 augment_three)
{
	return database.CreateItem(item_id, charges, augment_one, augment_two, augment_three);
}

EQ::ItemInstance* Perl__createitem(uint32 item_id, int16 charges, uint32 augment_one, uint32 augment_two, uint32 augment_three, uint32 augment_four)
{
	return database.CreateItem(item_id, charges, augment_one, augment_two, augment_three, augment_four);
}

EQ::ItemInstance* Perl__createitem(uint32 item_id, int16 charges, uint32 augment_one, uint32 augment_two, uint32 augment_three, uint32 augment_four, uint32 augment_five)
{
	return database.CreateItem(item_id, charges, augment_one, augment_two, augment_three, augment_four, augment_five);
}

EQ::ItemInstance* Perl__createitem(uint32 item_id, int16 charges, uint32 augment_one, uint32 augment_two, uint32 augment_three, uint32 augment_four, uint32 augment_five, uint32 augment_six)
{
	return database.CreateItem(item_id, charges, augment_one, augment_two, augment_three, augment_four, augment_five, augment_six);
}

EQ::ItemInstance* Perl__createitem(uint32 item_id, int16 charges, uint32 augment_one, uint32 augment_two, uint32 augment_three, uint32 augment_four, uint32 augment_five, uint32 augment_six, bool attuned)
{
	return database.CreateItem(item_id, charges, augment_one, augment_two, augment_three, augment_four, augment_five, augment_six, attuned);
}

std::string Perl__secondstotime(int duration)
{
	return quest_manager.secondstotime(duration);
}

std::string Perl__gethexcolorcode(std::string color_name)
{
	return quest_manager.gethexcolorcode(color_name);
}

double Perl__getaaexpmodifierbycharid(uint32 character_id, uint32 zone_id)
{
	return quest_manager.GetAAEXPModifierByCharID(character_id, zone_id);
}

double Perl__getexpmodifierbycharid(uint32 character_id, uint32 zone_id)
{
	return quest_manager.GetEXPModifierByCharID(character_id, zone_id);
}

void Perl__setaaexpmodifierbycharid(uint32 character_id, uint32 zone_id, double aa_modifier)
{
	quest_manager.SetAAEXPModifierByCharID(character_id, zone_id, aa_modifier);
}

void Perl__setexpmodifierbycharid(uint32 character_id, uint32 zone_id, double exp_modifier)
{
	quest_manager.SetEXPModifierByCharID(character_id, zone_id, exp_modifier);
}

std::string Perl__getcleannpcnamebyid(uint32 npc_id)
{
	return quest_manager.getcleannpcnamebyid(npc_id);
}

std::string Perl__getgendername(uint32 gender_id)
{
	return quest_manager.getgendername(gender_id);
}

std::string Perl__getdeityname(uint32 deity_id)
{
	return quest_manager.getdeityname(deity_id);
}

std::string Perl__getinventoryslotname(int16 slot_id)
{
	return quest_manager.getinventoryslotname(slot_id);
}

void Perl__rename(std::string name)
{
	quest_manager.rename(name);
}

std::string Perl__get_data_remaining(std::string bucket_name)
{
	return DataBucket::GetDataRemaining(bucket_name);
}

int Perl__getitemstat(uint32 item_id, std::string stat_identifier)
{
	return quest_manager.getitemstat(item_id, stat_identifier);
}

int Perl__getspellstat(uint32 spell_id, std::string stat_identifier)
{
	return quest_manager.getspellstat(spell_id, stat_identifier);
}

int Perl__getspellstat(uint32 spell_id, std::string stat_identifier, uint8 slot)
{
	return quest_manager.getspellstat(spell_id, stat_identifier, slot);
}

void Perl__crosszonecastspellbycharid(int character_id, uint32 spell_id)
{
	quest_manager.CrossZoneSpell(CZUpdateType_Character, CZSpellUpdateSubtype_Cast, character_id, spell_id);
}

void Perl__crosszonecastspellbygroupid(int group_id, uint32 spell_id)
{
	quest_manager.CrossZoneSpell(CZUpdateType_Group, CZSpellUpdateSubtype_Cast, group_id, spell_id);
}

void Perl__crosszonecastspellbyraidid(int raid_id, uint32 spell_id)
{
	quest_manager.CrossZoneSpell(CZUpdateType_Raid, CZSpellUpdateSubtype_Cast, raid_id, spell_id);
}

void Perl__crosszonecastspellbyguildid(int guild_id, uint32 spell_id)
{
	quest_manager.CrossZoneSpell(CZUpdateType_Guild, CZSpellUpdateSubtype_Cast, guild_id, spell_id);
}

void Perl__crosszonecastspellbyclientname(const char* client_name, uint32 spell_id)
{
	int update_identifier = 0;
	quest_manager.CrossZoneSpell(CZUpdateType_ClientName, CZSpellUpdateSubtype_Cast, update_identifier, spell_id, client_name);
}

void Perl__crosszonedialoguewindowbycharid(int character_id, const char* message)
{
	quest_manager.CrossZoneDialogueWindow(CZUpdateType_Character, character_id, message);
}

void Perl__crosszonedialoguewindowbygroupid(int group_id, const char* message)
{
	quest_manager.CrossZoneDialogueWindow(CZUpdateType_Group, group_id, message);
}

void Perl__crosszonedialoguewindowbyraidid(int raid_id, const char* message)
{
	quest_manager.CrossZoneDialogueWindow(CZUpdateType_Raid, raid_id, message);
}

void Perl__crosszonedialoguewindowbyguildid(int guild_id, const char* message)
{
	quest_manager.CrossZoneDialogueWindow(CZUpdateType_Guild, guild_id, message);
}

void Perl__crosszonedialoguewindowbyclientname(const char* client_name, const char* message)
{
	int update_identifier = 0;
	quest_manager.CrossZoneDialogueWindow(CZUpdateType_ClientName, update_identifier, message, client_name);
}

void Perl__crosszonemarqueebycharid(int character_id, uint32 type, uint32 priority, uint32 fade_in, uint32 fade_out, uint32 duration, const char* message)
{
	quest_manager.CrossZoneMarquee(CZUpdateType_Character, character_id, type, priority, fade_in, fade_out, duration, message);
}

void Perl__crosszonemarqueebygroupid(int group_id, uint32 type, uint32 priority, uint32 fade_in, uint32 fade_out, uint32 duration, const char* message)
{
	quest_manager.CrossZoneMarquee(CZUpdateType_Group, group_id, type, priority, fade_in, fade_out, duration, message);
}

void Perl__crosszonemarqueebyraidid(int raid_id, uint32 type, uint32 priority, uint32 fade_in, uint32 fade_out, uint32 duration, const char* message)
{
	quest_manager.CrossZoneMarquee(CZUpdateType_Raid, raid_id, type, priority, fade_in, fade_out, duration, message);
}

void Perl__crosszonemarqueebyguildid(int guild_id, uint32 type, uint32 priority, uint32 fade_in, uint32 fade_out, uint32 duration, const char* message)
{
	quest_manager.CrossZoneMarquee(CZUpdateType_Guild, guild_id, type, priority, fade_in, fade_out, duration, message);
}

void Perl__crosszonemarqueebyclientname(const char* client_name, uint32 type, uint32 priority, uint32 fade_in, uint32 fade_out, uint32 duration, const char* message)
{
	int update_identifier = 0;
	quest_manager.CrossZoneMarquee(CZUpdateType_ClientName, update_identifier, type, priority, fade_in, fade_out, duration, message, client_name);
}

void Perl__crosszonemessageplayerbycharid(int character_id, uint32 type, const char* message)
{
	quest_manager.CrossZoneMessage(CZUpdateType_Character, character_id, type, message);
}

void Perl__crosszonemessageplayerbygroupid(int group_id, uint32 type, const char* message)
{
	quest_manager.CrossZoneMessage(CZUpdateType_Group, group_id, type, message);
}

void Perl__crosszonemessageplayerbyraidid(int raid_id, uint32 type, const char* message)
{
	quest_manager.CrossZoneMessage(CZUpdateType_Raid, raid_id, type, message);
}

void Perl__crosszonemessageplayerbyguildid(int guild_id, uint32 type, const char* message)
{
	quest_manager.CrossZoneMessage(CZUpdateType_Guild, guild_id, type, message);
}

void Perl__crosszonemessageplayerbyname(const char* client_name, uint32 type, const char* message)
{
	int update_identifier = 0;
	quest_manager.CrossZoneMessage(CZUpdateType_ClientName, update_identifier, type, message, client_name);
}

void Perl__crosszonemoveplayerbycharid(int character_id, const char* zone_short_name)
{
	quest_manager.CrossZoneMove(CZUpdateType_Character, CZMoveUpdateSubtype_MoveZone, character_id, zone_short_name);
}

void Perl__crosszonemoveplayerbygroupid(int group_id, const char* zone_short_name)
{
	quest_manager.CrossZoneMove(CZUpdateType_Group, CZMoveUpdateSubtype_MoveZone, group_id, zone_short_name);
}

void Perl__crosszonemoveplayerbyraidid(int raid_id, const char* zone_short_name)
{
	quest_manager.CrossZoneMove(CZUpdateType_Raid, CZMoveUpdateSubtype_MoveZone, raid_id, zone_short_name);
}

void Perl__crosszonemoveplayerbyguildid(int guild_id, const char* zone_short_name)
{
	quest_manager.CrossZoneMove(CZUpdateType_Guild, CZMoveUpdateSubtype_MoveZone, guild_id, zone_short_name);
}

void Perl__crosszonemoveplayerbyname(const char* client_name, const char* zone_short_name)
{
	int update_identifier = 0;
	quest_manager.CrossZoneMove(CZUpdateType_ClientName, CZMoveUpdateSubtype_MoveZone, update_identifier, zone_short_name, client_name);
}

void Perl__crosszoneremovespellbycharid(int character_id, uint32 spell_id)
{
	quest_manager.CrossZoneSpell(CZUpdateType_Character, CZSpellUpdateSubtype_Remove, character_id, spell_id);
}

void Perl__crosszoneremovespellbygroupid(int group_id, uint32 spell_id)
{
	quest_manager.CrossZoneSpell(CZUpdateType_Group, CZSpellUpdateSubtype_Remove, group_id, spell_id);
}

void Perl__crosszoneremovespellbyraidid(int raid_id, uint32 spell_id)
{
	quest_manager.CrossZoneSpell(CZUpdateType_Raid, CZSpellUpdateSubtype_Remove, raid_id, spell_id);
}

void Perl__crosszoneremovespellbyguildid(int guild_id, uint32 spell_id)
{
	quest_manager.CrossZoneSpell(CZUpdateType_Guild, CZSpellUpdateSubtype_Remove, guild_id, spell_id);
}

void Perl__crosszoneremovespellbyclientname(const char* client_name, uint32 spell_id)
{
	int update_identifier = 0;
	quest_manager.CrossZoneSpell(CZUpdateType_ClientName, CZSpellUpdateSubtype_Remove, update_identifier, spell_id, client_name);
}

void Perl__crosszonesetentityvariablebycharid(int character_id, const char* variable_name, const char* variable_value)
{
	quest_manager.CrossZoneSetEntityVariable(CZUpdateType_Character, character_id, variable_name, variable_value);
}

void Perl__crosszonesetentityvariablebygroupid(int group_id, const char* variable_name, const char* variable_value)
{
	quest_manager.CrossZoneSetEntityVariable(CZUpdateType_Group, group_id, variable_name, variable_value);
}

void Perl__crosszonesetentityvariablebyraidid(int raid_id, const char* variable_name, const char* variable_value)
{
	quest_manager.CrossZoneSetEntityVariable(CZUpdateType_Raid, raid_id, variable_name, variable_value);
}

void Perl__crosszonesetentityvariablebyguildid(int guild_id, const char* variable_name, const char* variable_value)
{
	quest_manager.CrossZoneSetEntityVariable(CZUpdateType_Guild, guild_id, variable_name, variable_value);
}

void Perl__crosszonesetentityvariablebyclientname(const char* client_name, const char* variable_name, const char* variable_value)
{
	int update_identifier = 0;
	quest_manager.CrossZoneSetEntityVariable(CZUpdateType_ClientName, update_identifier, variable_name, variable_value, client_name);
}

void Perl__crosszonesetentityvariablebynpctypeid(int npc_id, const char* variable_name, const char* variable_value)
{
	quest_manager.CrossZoneSetEntityVariable(CZUpdateType_NPC, npc_id, variable_name, variable_value);
}

void Perl__crosszonesignalclientbycharid(int character_id, int signal)
{
	quest_manager.CrossZoneSignal(CZUpdateType_Character, character_id, signal);
}

void Perl__crosszonesignalclientbygroupid(int group_id, int signal)
{
	quest_manager.CrossZoneSignal(CZUpdateType_Group, group_id, signal);
}

void Perl__crosszonesignalclientbyraidid(int raid_id, int signal)
{
	quest_manager.CrossZoneSignal(CZUpdateType_Raid, raid_id, signal);
}

void Perl__crosszonesignalclientbyguildid(int guild_id, int signal)
{
	quest_manager.CrossZoneSignal(CZUpdateType_Guild, guild_id, signal);
}

void Perl__crosszonesignalclientbyname(const char* client_name, int signal)
{
	int update_identifier = 0;
	quest_manager.CrossZoneSignal(CZUpdateType_Expedition, update_identifier, signal, client_name);
}

void Perl__crosszonesignalnpcbynpctypeid(uint32 npc_id, int signal)
{
	quest_manager.CrossZoneSignal(CZUpdateType_NPC, npc_id, signal);
}

void Perl__worldwidecastspell(uint32 spell_id)
{
	quest_manager.WorldWideSpell(WWSpellUpdateType_Cast, spell_id);
}

void Perl__worldwidecastspell(uint32 spell_id, uint8 min_status)
{
	quest_manager.WorldWideSpell(WWSpellUpdateType_Cast, spell_id, min_status);
}

void Perl__worldwidecastspell(uint32 spell_id, uint8 min_status, uint8 max_status)
{
	quest_manager.WorldWideSpell(WWSpellUpdateType_Cast, spell_id, min_status, max_status);
}

void Perl__worldwidedialoguewindow(const char* message)
{
	quest_manager.WorldWideDialogueWindow(message);
}

void Perl__worldwidedialoguewindow(const char* message, uint8 min_status)
{
	quest_manager.WorldWideDialogueWindow(message, min_status);
}

void Perl__worldwidedialoguewindow(const char* message, uint8 min_status, uint8 max_status)
{
	quest_manager.WorldWideDialogueWindow(message, min_status, max_status);
}

void Perl__worldwidemarquee(uint32 type, uint32 priority, uint32 fade_in, uint32 fade_out, uint32 duration, const char* message)
{
	quest_manager.WorldWideMarquee(type, priority, fade_in, fade_out, duration, message);
}

void Perl__worldwidemarquee(uint32 type, uint32 priority, uint32 fade_in, uint32 fade_out, uint32 duration, const char* message, uint8 min_status)
{
	quest_manager.WorldWideMarquee(type, priority, fade_in, fade_out, duration, message, min_status);
}

void Perl__worldwidemarquee(uint32 type, uint32 priority, uint32 fade_in, uint32 fade_out, uint32 duration, const char* message, uint8 min_status, uint8 max_status)
{
	quest_manager.WorldWideMarquee(type, priority, fade_in, fade_out, duration, message, min_status, max_status);
}

void Perl__worldwidemessage(uint32 type, const char* message)
{
	quest_manager.WorldWideMessage(type, message);
}

void Perl__worldwidemessage(uint32 type, const char* message, uint8 min_status)
{
	quest_manager.WorldWideMessage(type, message, min_status);
}

void Perl__worldwidemessage(uint32 type, const char* message, uint8 min_status, uint8 max_status)
{
	quest_manager.WorldWideMessage(type, message, min_status, max_status);
}

void Perl__worldwidemove(const char* zone_short_name)
{
	quest_manager.WorldWideMove(WWMoveUpdateType_MoveZone, zone_short_name);
}

void Perl__worldwidemove(const char* zone_short_name, uint8 min_status)
{
	quest_manager.WorldWideMove(WWMoveUpdateType_MoveZone, zone_short_name, min_status);
}

void Perl__worldwidemove(const char* zone_short_name, uint8 min_status, uint8 max_status)
{
	quest_manager.WorldWideMove(WWMoveUpdateType_MoveZone, zone_short_name, min_status, max_status);
}

void Perl__worldwideremovespell(uint32 spell_id)
{
	quest_manager.WorldWideSpell(WWSpellUpdateType_Remove, spell_id);
}

void Perl__worldwideremovespell(uint32 spell_id, uint8 min_status)
{
	quest_manager.WorldWideSpell(WWSpellUpdateType_Remove, spell_id, min_status);
}

void Perl__worldwideremovespell(uint32 spell_id, uint8 min_status, uint8 max_status)
{
	quest_manager.WorldWideSpell(WWSpellUpdateType_Remove, spell_id, min_status, max_status);
}

void Perl__worldwidesetentityvariableclient(const char* variable_name, const char* variable_value)
{
	quest_manager.WorldWideSetEntityVariable(WWSetEntityVariableUpdateType_Character, variable_name, variable_value);
}

void Perl__worldwidesetentityvariableclient(const char* variable_name, const char* variable_value, uint8 min_status)
{
	quest_manager.WorldWideSetEntityVariable(WWSetEntityVariableUpdateType_Character, variable_name, variable_value, min_status);
}

void Perl__worldwidesetentityvariableclient(const char* variable_name, const char* variable_value, uint8 min_status, uint8 max_status)
{
	quest_manager.WorldWideSetEntityVariable(WWSetEntityVariableUpdateType_Character, variable_name, variable_value, min_status, max_status);
}

void Perl__worldwidesetentityvariablenpc(const char* variable_name, const char* variable_value)
{
	quest_manager.WorldWideSetEntityVariable(WWSetEntityVariableUpdateType_NPC, variable_name, variable_value);
}

void Perl__worldwidesignalnpc(int signal)
{
	quest_manager.WorldWideSignal(WWSignalUpdateType_NPC, signal);
}

void Perl__worldwidesignalclient(int signal)
{
	quest_manager.WorldWideSignal(WWSignalUpdateType_Character, signal);
}

void Perl__worldwidesignalclient(int signal, uint8 min_status)
{
	quest_manager.WorldWideSignal(WWSignalUpdateType_Character, signal, min_status);
}

void Perl__worldwidesignalclient(int signal, uint8 min_status, uint8 max_status)
{
	quest_manager.WorldWideSignal(WWSignalUpdateType_Character, signal, min_status, max_status);
}

bool Perl__isnpcspawned(perl::array npc_id_array)
{
	std::vector<uint32> npc_ids;
	for (int i = 0; i < npc_id_array.size(); ++i)
	{
		npc_ids.push_back(static_cast<uint32_t>(npc_id_array[i]));
	}
	return entity_list.IsNPCSpawned(npc_ids);
}

uint32_t Perl__countspawnednpcs(perl::array npc_id_array)
{
	std::vector<uint32> npc_ids;
	for (int i = 0; i < npc_id_array.size(); ++i)
	{
		npc_ids.push_back(static_cast<uint32_t>(npc_id_array[i]));
	}
	return entity_list.CountSpawnedNPCs(npc_ids);
}

SPDat_Spell_Struct* Perl__getspell(uint32 spell_id)
{
	// should be safe, it's read only in perl (could also use proxy lika lua)
	return const_cast<SPDat_Spell_Struct*>(quest_manager.getspell(spell_id));
}

std::string Perl__getfactionname(int faction_id)
{
	return quest_manager.getfactionname(faction_id);
}

std::string Perl__getlanguagename(int language_id)
{
	return quest_manager.getlanguagename(language_id);
}

std::string Perl__getbodytypename(uint32 bodytype_id)
{
	return quest_manager.getbodytypename(bodytype_id);
}

std::string Perl__getconsiderlevelname(uint8 consider_level)
{
	return quest_manager.getconsiderlevelname(consider_level);
}

std::string Perl__getenvironmentaldamagename(uint8 damage_type)
{
	return quest_manager.getenvironmentaldamagename(damage_type);
}

std::string Perl__commify(perl::scalar number)
{
	return Strings::Commify(number.c_str());
}

bool Perl__checknamefilter(std::string name)
{
	return database.CheckNameFilter(name);
}

void Perl__discordsend(std::string webhook_name, std::string message)
{
	zone->SendDiscordMessage(webhook_name, message);
}

void Perl__tracknpc(uint32 entity_id)
{
	quest_manager.TrackNPC(entity_id);
}

int Perl__getrecipemadecount(uint32 recipe_id)
{
	return quest_manager.GetRecipeMadeCount(recipe_id);
}

std::string Perl__getrecipename(uint32 recipe_id)
{
	return quest_manager.GetRecipeName(recipe_id);
}

bool Perl__hasrecipelearned(uint32 recipe_id)
{
	return quest_manager.HasRecipeLearned(recipe_id);
}

bool Perl__IsRaining()
{
	if (!zone) {
		return false;
	}

	return zone->IsRaining();
}

bool Perl__IsSnowing()
{
	if (!zone) {
		return false;
	}

	return zone->IsSnowing();
}

std::string Perl__getaaname(int aa_id)
{
	if (!zone) {
		return std::string();
	}

	return zone->GetAAName(aa_id);
}

std::string Perl__popupbreak() {
	return DialogueWindow::Break();
}

std::string Perl__popupbreak(uint32 break_count) {
	return DialogueWindow::Break(break_count);
}

std::string Perl__popupcentermessage(std::string message) {
	return DialogueWindow::CenterMessage(message);
}

std::string Perl__popupcolormessage(std::string color, std::string message) {
	return DialogueWindow::ColorMessage(color, message);
}

std::string Perl__popupindent() {
	return DialogueWindow::Indent();
}

std::string Perl__popupindent(uint32 indent_count) {
	return DialogueWindow::Indent(indent_count);
}

std::string Perl__popuplink(std::string link) {
	return DialogueWindow::Link(link);
}

std::string Perl__popuplink(std::string link, std::string message) {
	return DialogueWindow::Link(link, message);
}

std::string Perl__popuptable(std::string message) {
	return DialogueWindow::Table(message);
}

std::string Perl__popuptablecell() {
	return DialogueWindow::TableCell();
}

std::string Perl__popuptablecell(std::string message) {
	return DialogueWindow::TableCell(message);
}

std::string Perl__popuptablerow(std::string message) {
	return DialogueWindow::TableRow(message);
}

void Perl__marquee(uint32 type, std::string message)
{
	quest_manager.marquee(type, message);
}

void Perl__marquee(uint32 type, std::string message, uint32 duration)
{
	quest_manager.marquee(type, message, duration);
}

void Perl__marquee(uint32 type, uint32 priority, uint32 fade_in, uint32 fade_out, uint32 duration, std::string message)
{
	quest_manager.marquee(type, priority, fade_in, fade_out, duration, message);
}

void Perl__zonemarquee(uint32 type, std::string message)
{
	if (!zone) {
		return;
	}

	entity_list.Marquee(type, message);
}

void Perl__zonemarquee(uint32 type, std::string message, uint32 duration)
{
	if (!zone) {
		return;
	}

	entity_list.Marquee(type, message, duration);
}

void Perl__zonemarquee(uint32 type, uint32 priority, uint32 fade_in, uint32 fade_out, uint32 duration, std::string message)
{
	if (!zone) {
		return;
	}

	entity_list.Marquee(type, priority, fade_in, fade_out, duration, message);
}

void perl_register_quest()
{
	perl::interpreter perl(PERL_GET_THX);

	auto package = perl.new_package("quest");

#ifdef BOTS
	package.add("botquest", &Perl__botquest);
	package.add("spawnbotcount", (int(*)())&Perl__spawnbotcount);
	package.add("spawnbotcount", (int(*)(uint8))&Perl__spawnbotcount);
	package.add("createbotcount", (int(*)())&Perl__createbotcount);
	package.add("createbotcount", (int(*)(uint8))&Perl__createbotcount);
	package.add("createBot", &Perl__createBot);
#endif //BOTS

	package.add("ChooseRandom", &Perl__ChooseRandom);
	package.add("FlyMode", &Perl__FlyMode);
	package.add("GetSpellResistType", &Perl__GetSpellResistType);
	package.add("GetSpellTargetType", &Perl__GetSpellTargetType);
	package.add("GetTimeSeconds", &Perl__GetTimeSeconds);
	package.add("GetZoneID", &Perl__GetZoneID);
	package.add("GetZoneLongName", &Perl__GetZoneLongName);
	package.add("GetZoneLongNameByID", &Perl__GetZoneLongNameByID);
	package.add("GetZoneShortName", &Perl__GetZoneShortName);
	package.add("set_rule", &Perl__set_rule);
	package.add("get_rule", &Perl__get_rule);
	package.add("get_data", &Perl__get_data);
	package.add("get_data_expires", &Perl__get_data_expires);
	package.add("get_data_remaining", &Perl__get_data_remaining);
	package.add("set_data", (void(*)(std::string, std::string))&Perl__set_data);
	package.add("set_data", (void(*)(std::string, std::string, std::string))&Perl__set_data);
	package.add("delete_data", &Perl__delete_data);
	package.add("IsBeneficialSpell", &Perl__IsBeneficialSpell);
	package.add("IsEffectInSpell", &Perl__IsEffectInSpell);
	package.add("IsRaining", &Perl__IsRaining);
	package.add("IsSnowing", &Perl__IsSnowing);
	package.add("IsRunning", &Perl__IsRunning);
	package.add("LearnRecipe", &Perl__LearnRecipe);
	package.add("MerchantCountItem", &Perl__MerchantCountItem);
	package.add("MerchantSetItem", (void(*)(uint32, uint32))&Perl__MerchantSetItem);
	package.add("MerchantSetItem", (void(*)(uint32, uint32, uint32))&Perl__MerchantSetItem);
	package.add("SendMail", &Perl__SendMail);
	package.add("SetRunning", &Perl__SetRunning);
	package.add("addloot", (void(*)(int))&Perl__addloot);
	package.add("addloot", (void(*)(int, int))&Perl__addloot);
	package.add("addloot", (void(*)(int, int, bool))&Perl__addloot);
	package.add("addskill", &Perl__addskill);
	package.add("attack", &Perl__attack);
	package.add("attacknpc", &Perl__attacknpc);
	package.add("attacknpctype", &Perl__attacknpctype);
	package.add("buryplayercorpse", &Perl__buryplayercorpse);
	package.add("castspell", &Perl__castspell);
	package.add("changedeity", &Perl__changedeity);
	package.add("checknamefilter", &Perl__checknamefilter),
	package.add("checktitle", &Perl__checktitle);
	package.add("clear_npctype_cache", &Perl__clear_npctype_cache);
	package.add("clear_proximity", &Perl__clear_proximity);
	package.add("clear_zone_flag", &Perl__clear_zone_flag);
	package.add("clearspawntimers", &Perl__clearspawntimers);
	package.add("collectitems", &Perl__collectitems);
	package.add("commify", &Perl__commify);
	package.add("countitem", &Perl__countitem);
	package.add("countspawnednpcs", &Perl__countspawnednpcs);
	package.add("createdoor", (int(*)(const char*, float, float, float, float))&Perl__CreateDoor);
	package.add("createdoor", (int(*)(const char*, float, float, float, float, uint8_t))&Perl__CreateDoor);
	package.add("createdoor", (int(*)(const char*, float, float, float, float, uint8_t, uint16_t))&Perl__CreateDoor);
	package.add("creategroundobject", (int(*)(uint32_t, float, float, float, float))&Perl__CreateGroundObject);
	package.add("creategroundobject", (int(*)(uint32_t, float, float, float, float, uint32_t))&Perl__CreateGroundObject);
	package.add("creategroundobjectfrommodel", (int(*)(const char*, float, float, float, float))&Perl__CreateGroundObjectFromModel);
	package.add("creategroundobjectfrommodel", (int(*)(const char*, float, float, float, float, uint8_t))&Perl__CreateGroundObjectFromModel);
	package.add("creategroundobjectfrommodel", (int(*)(const char*, float, float, float, float, uint8_t, uint32_t))&Perl__CreateGroundObjectFromModel);
	package.add("createguild", &Perl__createguild);
	package.add("createitem", (EQ::ItemInstance*(*)(uint32))&Perl__createitem);
	package.add("createitem", (EQ::ItemInstance*(*)(uint32, int16))&Perl__createitem);
	package.add("createitem", (EQ::ItemInstance*(*)(uint32, int16, uint32))&Perl__createitem);
	package.add("createitem", (EQ::ItemInstance*(*)(uint32, int16, uint32, uint32))&Perl__createitem);
	package.add("createitem", (EQ::ItemInstance*(*)(uint32, int16, uint32, uint32, uint32))&Perl__createitem);
	package.add("createitem", (EQ::ItemInstance*(*)(uint32, int16, uint32, uint32, uint32, uint32))&Perl__createitem);
	package.add("createitem", (EQ::ItemInstance*(*)(uint32, int16, uint32, uint32, uint32, uint32, uint32))&Perl__createitem);
	package.add("createitem", (EQ::ItemInstance*(*)(uint32, int16, uint32, uint32, uint32, uint32, uint32, uint32))&Perl__createitem);
	package.add("createitem", (EQ::ItemInstance*(*)(uint32, int16, uint32, uint32, uint32, uint32, uint32, uint32, bool))&Perl__createitem);
	package.add("crosszonecastspellbycharid", &Perl__crosszonecastspellbycharid);
	package.add("crosszonecastspellbygroupid", &Perl__crosszonecastspellbygroupid);
	package.add("crosszonecastspellbyraidid", &Perl__crosszonecastspellbyraidid);
	package.add("crosszonecastspellbyguildid", &Perl__crosszonecastspellbyguildid);
	package.add("crosszonecastspellbyclientname", &Perl__crosszonecastspellbyclientname);
	package.add("crosszonedialoguewindowbycharid", &Perl__crosszonedialoguewindowbycharid);
	package.add("crosszonedialoguewindowbygroupid", &Perl__crosszonedialoguewindowbygroupid);
	package.add("crosszonedialoguewindowbyraidid", &Perl__crosszonedialoguewindowbyraidid);
	package.add("crosszonedialoguewindowbyguildid", &Perl__crosszonedialoguewindowbyguildid);
	package.add("crosszonedialoguewindowbyclientname", &Perl__crosszonedialoguewindowbyclientname);
	package.add("crosszonemarqueebycharid", &Perl__crosszonemarqueebycharid);
	package.add("crosszonemarqueebygroupid", &Perl__crosszonemarqueebygroupid);
	package.add("crosszonemarqueebyraidid", &Perl__crosszonemarqueebyraidid);
	package.add("crosszonemarqueebyguildid", &Perl__crosszonemarqueebyguildid);
	package.add("crosszonemarqueebyclientname", &Perl__crosszonemarqueebyclientname);
	package.add("crosszonemessageplayerbycharid", &Perl__crosszonemessageplayerbycharid);
	package.add("crosszonemessageplayerbygroupid", &Perl__crosszonemessageplayerbygroupid);
	package.add("crosszonemessageplayerbyraidid", &Perl__crosszonemessageplayerbyraidid);
	package.add("crosszonemessageplayerbyguildid", &Perl__crosszonemessageplayerbyguildid);
	package.add("crosszonemessageplayerbyname", &Perl__crosszonemessageplayerbyname);
	package.add("crosszonemoveplayerbycharid", &Perl__crosszonemoveplayerbycharid);
	package.add("crosszonemoveplayerbygroupid", &Perl__crosszonemoveplayerbygroupid);
	package.add("crosszonemoveplayerbyraidid", &Perl__crosszonemoveplayerbyraidid);
	package.add("crosszonemoveplayerbyguildid", &Perl__crosszonemoveplayerbyguildid);
	package.add("crosszonemoveplayerbyname", &Perl__crosszonemoveplayerbyname);
	package.add("crosszoneremovespellbycharid", &Perl__crosszoneremovespellbycharid);
	package.add("crosszoneremovespellbygroupid", &Perl__crosszoneremovespellbygroupid);
	package.add("crosszoneremovespellbyraidid", &Perl__crosszoneremovespellbyraidid);
	package.add("crosszoneremovespellbyguildid", &Perl__crosszoneremovespellbyguildid);
	package.add("crosszoneremovespellbyclientname", &Perl__crosszoneremovespellbyclientname);
	package.add("crosszonesetentityvariablebycharid", &Perl__crosszonesetentityvariablebycharid);
	package.add("crosszonesetentityvariablebygroupid", &Perl__crosszonesetentityvariablebygroupid);
	package.add("crosszonesetentityvariablebyraidid", &Perl__crosszonesetentityvariablebyraidid);
	package.add("crosszonesetentityvariablebyguildid", &Perl__crosszonesetentityvariablebyguildid);
	package.add("crosszonesetentityvariablebyclientname", &Perl__crosszonesetentityvariablebyclientname);
	package.add("crosszonesetentityvariablebynpctypeid", &Perl__crosszonesetentityvariablebynpctypeid);
	package.add("crosszonesignalclientbycharid", &Perl__crosszonesignalclientbycharid);
	package.add("crosszonesignalclientbygroupid", &Perl__crosszonesignalclientbygroupid);
	package.add("crosszonesignalclientbyraidid", &Perl__crosszonesignalclientbyraidid);
	package.add("crosszonesignalclientbyguildid", &Perl__crosszonesignalclientbyguildid);
	package.add("crosszonesignalclientbyname", &Perl__crosszonesignalclientbyname);
	package.add("crosszonesignalnpcbynpctypeid", &Perl__crosszonesignalnpcbynpctypeid);
	package.add("worldwidecastspell", (void(*)(uint32))&Perl__worldwidecastspell);
	package.add("worldwidecastspell", (void(*)(uint32, uint8))&Perl__worldwidecastspell);
	package.add("worldwidecastspell", (void(*)(uint32, uint8, uint8 max_status))&Perl__worldwidecastspell);
	package.add("worldwidedialoguewindow", (void(*)(const char*))&Perl__worldwidedialoguewindow);
	package.add("worldwidedialoguewindow", (void(*)(const char*, uint8))&Perl__worldwidedialoguewindow);
	package.add("worldwidedialoguewindow", (void(*)(const char*, uint8, uint8))&Perl__worldwidedialoguewindow);
	package.add("worldwidemarquee", (void(*)(uint32, uint32, uint32, uint32, uint32, const char*))&Perl__worldwidemarquee);
	package.add("worldwidemarquee", (void(*)(uint32, uint32, uint32, uint32, uint32, const char*, uint8))&Perl__worldwidemarquee);
	package.add("worldwidemarquee", (void(*)(uint32, uint32, uint32, uint32, uint32, const char*, uint8, uint8))&Perl__worldwidemarquee);
	package.add("worldwidemessage", (void(*)(uint32, const char*))&Perl__worldwidemessage);
	package.add("worldwidemessage", (void(*)(uint32, const char*, uint8))&Perl__worldwidemessage);
	package.add("worldwidemessage", (void(*)(uint32, const char*, uint8, uint8))&Perl__worldwidemessage);
	package.add("worldwidemove", (void(*)(const char*))&Perl__worldwidemove);
	package.add("worldwidemove", (void(*)(const char*, uint8))&Perl__worldwidemove);
	package.add("worldwidemove", (void(*)(const char*, uint8, uint8))&Perl__worldwidemove);
	package.add("worldwideremovespell", (void(*)(uint32))&Perl__worldwideremovespell);
	package.add("worldwideremovespell", (void(*)(uint32, uint8))&Perl__worldwideremovespell);
	package.add("worldwideremovespell", (void(*)(uint32, uint8, uint8))&Perl__worldwideremovespell);
	package.add("worldwidesetentityvariableclient", (void(*)(const char*, const char*))&Perl__worldwidesetentityvariableclient);
	package.add("worldwidesetentityvariableclient", (void(*)(const char*, const char*, uint8))&Perl__worldwidesetentityvariableclient);
	package.add("worldwidesetentityvariableclient", (void(*)(const char*, const char*, uint8, uint8))&Perl__worldwidesetentityvariableclient);
	package.add("worldwidesetentityvariablenpc", &Perl__worldwidesetentityvariablenpc);
	package.add("worldwidesignalclient", (void(*)(int))&Perl__worldwidesignalclient);
	package.add("worldwidesignalclient", (void(*)(int, uint8))&Perl__worldwidesignalclient);
	package.add("worldwidesignalclient", (void(*)(int, uint8, uint8))&Perl__worldwidesignalclient);
	package.add("worldwidesignalnpc", &Perl__worldwidesignalnpc);
	package.add("debug", (void(*)(const char*))&Perl__debug);
	package.add("debug", (void(*)(const char*, int))&Perl__debug);
	package.add("delglobal", &Perl__delglobal);
	package.add("depop", (void(*)())&Perl__depop);
	package.add("depop", (void(*)(int))&Perl__depop);
	package.add("depop_withtimer", (void(*)())&Perl__depop_withtimer);
	package.add("depop_withtimer", (void(*)(int))&Perl__depop_withtimer);
	package.add("depopall", (void(*)())&Perl__depopall);
	package.add("depopall", (void(*)(int))&Perl__depopall);
	package.add("depopzone", &Perl__depopzone);
	package.add("ding", &Perl__ding);
	package.add("disable_proximity_say", &Perl__disable_proximity_say);
	package.add("disable_spawn2", &Perl__disable_spawn2);
	package.add("disablerecipe", &Perl__disablerecipe);
	package.add("discordsend", &Perl__discordsend);
	package.add("doanim", &Perl__doanim);
	package.add("echo", &Perl__echo);
	package.add("emote", &Perl__emote);
	package.add("enable_proximity_say", &Perl__enable_proximity_say);
	package.add("enable_spawn2", &Perl__enable_spawn2);
	package.add("enablerecipe", &Perl__enablerecipe);
	package.add("enabletitle", &Perl__enabletitle);
	package.add("exp", &Perl__exp);
	package.add("faction", (void(*)(int, int))&Perl__faction);
	package.add("faction", (void(*)(int, int, int))&Perl__faction);
	package.add("factionvalue", &Perl__FactionValue);
	package.add("follow", (void(*)(int))&Perl__follow);
	package.add("follow", (void(*)(int, int))&Perl__follow);
	package.add("forcedoorclose", (void(*)(uint32))&Perl__forcedoorclose);
	package.add("forcedoorclose", (void(*)(uint32, bool))&Perl__forcedoorclose);
	package.add("forcedooropen", (void(*)(uint32))&Perl__forcedooropen);
	package.add("forcedooropen", (void(*)(uint32, bool))&Perl__forcedooropen);
	package.add("getaaexpmodifierbycharid", (double(*)(uint32, uint32))&Perl__getaaexpmodifierbycharid);
	package.add("getaaexpmodifierbycharid", (double(*)(uint32, uint32, int16))&Perl__getaaexpmodifierbycharid);
	package.add("getaaname", (std::string(*)(int))&Perl__getaaname);
	package.add("getbodytypename", &Perl__getbodytypename);
	package.add("getcharidbyname", &Perl__getcharidbyname);
	package.add("getclassname", (std::string(*)(uint8))&Perl__getclassname);
	package.add("getclassname", (std::string(*)(uint8, uint8))&Perl__getclassname);
	package.add("getcleannpcnamebyid", &Perl__getcleannpcnamebyid);
	package.add("getconsiderlevelname", &Perl__getconsiderlevelname);
	package.add("gethexcolorcode", &Perl__gethexcolorcode);
	package.add("getcurrencyid", &Perl__getcurrencyid);
	package.add("getexpmodifierbycharid", (double(*)(uint32, uint32))&Perl__getexpmodifierbycharid);
	package.add("getexpmodifierbycharid", (double(*)(uint32, uint32, int16))&Perl__getexpmodifierbycharid);
	package.add("getfactionname", &Perl__getfactionname);
	package.add("getinventoryslotid", &Perl__getinventoryslotid);
	package.add("getitemname", &Perl__getitemname);
	package.add("getItemName", &Perl__qc_getItemName);
	package.add("getitemstat", &Perl__getitemstat);
	package.add("getlanguagename", &Perl__getlanguagename);
	package.add("getnpcnamebyid", &Perl__getnpcnamebyid);
	package.add("get_spawn_condition", (int(*)(const char*, uint16))&Perl__get_spawn_condition);
	package.add("get_spawn_condition", (int(*)(const char*, uint32, uint16))&Perl__get_spawn_condition);
	package.add("getcharnamebyid", &Perl__getcharnamebyid);
	package.add("getcurrencyitemid", &Perl__getcurrencyitemid);
	package.add("getgendername", &Perl__getgendername);
	package.add("getdeityname", &Perl__getdeityname);
	package.add("getenvironmentaldamagename", &Perl__getenvironmentaldamagename);
	package.add("getguildnamebyid", &Perl__getguildnamebyid);
	package.add("getguildidbycharid", &Perl__getguildidbycharid);
	package.add("getgroupidbycharid", &Perl__getgroupidbycharid);
	package.add("getinventoryslotname", &Perl__getinventoryslotname);
	package.add("getraididbycharid", &Perl__getraididbycharid);
	package.add("getracename", &Perl__getracename);
	package.add("getremainingtimeMS", &Perl__getremainingtimeMS);
	package.add("getspell", &Perl__getspell);
	package.add("getspellname", &Perl__getspellname);
	package.add("get_spell_level", &Perl__get_spell_level);
	package.add("getspellstat", (int(*)(uint32, std::string))&Perl__getspellstat);
	package.add("getspellstat", (int(*)(uint32, std::string, uint8))&Perl__getspellstat);
	package.add("getskillname", &Perl__getskillname);
	package.add("getlevel", &Perl__getlevel);
	package.add("getplayerburiedcorpsecount", &Perl__getplayerburiedcorpsecount);
	package.add("getplayercorpsecount", &Perl__getplayercorpsecount);
	package.add("getplayercorpsecountbyzoneid", &Perl__getplayercorpsecountbyzoneid);
	package.add("getrecipemadecount", &Perl__getrecipemadecount);
	package.add("getrecipename", &Perl__getrecipename);
	package.add("gettimerdurationMS", &Perl__gettimerdurationMS);
	package.add("givecash", (void(*)(uint32))&Perl__givecash);
	package.add("givecash", (void(*)(uint32, uint32))&Perl__givecash);
	package.add("givecash", (void(*)(uint32, uint32, uint32))&Perl__givecash);
	package.add("givecash", (void(*)(uint32, uint32, uint32, uint32))&Perl__givecash);
	package.add("gmmove", &Perl__gmmove);
	package.add("gmsay", (void(*)(const char*))&Perl__gmsay);
	package.add("gmsay", (void(*)(const char*, int))&Perl__gmsay);
	package.add("gmsay", (void(*)(const char*, int, bool))&Perl__gmsay);
	package.add("gmsay", (void(*)(const char*, int, bool, int))&Perl__gmsay);
	package.add("gmsay", (void(*)(const char*, int, bool, int, int))&Perl__gmsay);
	package.add("has_zone_flag", &Perl__has_zone_flag);
	package.add("hasrecipelearned", &Perl__hasrecipelearned);
	package.add("hastimer", &Perl__hastimer);
	package.add("incstat", &Perl__incstat);
	package.add("isdisctome", &Perl__isdisctome);
	package.add("isdooropen", &Perl__isdooropen);
	package.add("isnpcspawned", &Perl__isnpcspawned);
	package.add("itemlink", &Perl__itemlink);
	package.add("level", &Perl__level);
	package.add("log", &Perl__log);
	package.add("log_combat", &Perl__log_combat);
	package.add("marquee", (void(*)(uint32, std::string))&Perl__marquee);
	package.add("marquee", (void(*)(uint32, std::string, uint32))&Perl__marquee);
	package.add("marquee", (void(*)(uint32, uint32, uint32, uint32, uint32, std::string))&Perl__marquee);
	package.add("me", &Perl__me);
	package.add("message", &Perl__message);
	package.add("modifynpcstat", &Perl__ModifyNPCStat);
	package.add("movegrp", &Perl__movegrp);
	package.add("movepc",(void(*)(int, float, float, float))&Perl__movepc);
	package.add("movepc",(void(*)(int, float, float, float, float))&Perl__movepc);
	package.add("moveto", (void(*)(float, float, float))&Perl__moveto);
	package.add("moveto", (void(*)(float, float, float, float))&Perl__moveto);
	package.add("moveto", (void(*)(float, float, float, float, bool))&Perl__moveto);
	package.add("npcfeature", &Perl__npcfeature);
	package.add("npcgender", &Perl__npcgender);
	package.add("npcrace", &Perl__npcrace);
	package.add("npcsize", &Perl__npcsize);
	package.add("npctexture", &Perl__npctexture);
	package.add("pause", &Perl__pause);
	package.add("permaclass", &Perl__permaclass);
	package.add("permagender", &Perl__permagender);
	package.add("permarace", &Perl__permarace);
	package.add("playerfeature", &Perl__playerfeature);
	package.add("playergender", &Perl__playergender);
	package.add("playerrace", &Perl__playerrace);
	package.add("playersize", &Perl__playersize);
	package.add("playertexture", &Perl__playertexture);
	package.add("popup", (void(*)(const char*, const char*))&Perl__popup);
	package.add("popup", (void(*)(const char*, const char*, int))&Perl__popup);
	package.add("popup", (void(*)(const char*, const char*, int, int))&Perl__popup);
	package.add("popup", (void(*)(const char*, const char*, int, int, int))&Perl__popup);
	package.add("popupbreak", (std::string(*)())&Perl__popupbreak);
	package.add("popupbreak", (std::string(*)(uint32))&Perl__popupbreak);
	package.add("popupcentermessage", &Perl__popupcentermessage);
	package.add("popupcolormessage", &Perl__popupcolormessage);
	package.add("popupindent", (std::string(*)())&Perl__popupindent);
	package.add("popupindent", (std::string(*)(uint32))&Perl__popupindent);
	package.add("popuplink", (std::string(*)(std::string))&Perl__popuplink);
	package.add("popuplink", (std::string(*)(std::string, std::string))&Perl__popuplink);
	package.add("popuptable", &Perl__popuptable);
	package.add("popuptablecell", (std::string(*)())&Perl__popuptablecell);
	package.add("popuptablecell", (std::string(*)(std::string))&Perl__popuptablecell);
	package.add("popuptablerow", &Perl__popuptablerow);
	package.add("processmobswhilezoneempty", &Perl__processmobswhilezoneempty);
	package.add("pvp", &Perl__pvp);
	package.add("qs_player_event", &Perl__qs_player_event);
	package.add("qs_send_query", &Perl__qs_send_query);
	package.add("rain", &Perl__rain);
	package.add("rebind", (void(*)(int, float, float, float))&Perl__rebind);
	package.add("rebind", (void(*)(int, float, float, float, float))&Perl__rebind);
	package.add("reloadzonestaticdata", &Perl__reloadzonestaticdata);
	package.add("removeitem", (void(*)(uint32_t))&Perl__removeitem);
	package.add("removeitem", (void(*)(uint32_t, int))&Perl__removeitem);
	package.add("removetitle", &Perl__removetitle);
	package.add("rename", &Perl__rename);
	package.add("repopzone", &Perl__repopzone);
	package.add("respawn", &Perl__respawn);
	package.add("resume", &Perl__resume);
	package.add("rewardfaction", &Perl__rewardfaction);
	package.add("safemove", &Perl__safemove);
	package.add("save", &Perl__save);
	package.add("say", (void(*)(const char*))&Perl__say);
	package.add("say", (void(*)(const char*, int))&Perl__say);
	package.add("say", (void(*)(const char*, int, int))&Perl__say);
	package.add("say", (void(*)(const char*, int, int, int))&Perl__say);
	package.add("say", (void(*)(const char*, int, int, int, int))&Perl__say);
	package.add("saylink", (std::string(*)(const char*))&Perl__saylink);
	package.add("saylink", (std::string(*)(const char*, bool))&Perl__saylink);
	package.add("saylink", (std::string(*)(const char*, bool, const char*))&Perl__saylink);
	package.add("scribespells", (int(*)(int))&Perl__scribespells);
	package.add("scribespells", (int(*)(int, int))&Perl__scribespells);
	package.add("secondstotime", &Perl__secondstotime);
	package.add("selfcast", &Perl__selfcast);
	package.add("setaaexpmodifierbycharid", (void(*)(uint32, uint32, double))&Perl__setaaexpmodifierbycharid);
	package.add("setaaexpmodifierbycharid", (void(*)(uint32, uint32, double, int16))&Perl__setaaexpmodifierbycharid);
	package.add("set_proximity", (void(*)(float, float, float, float))&Perl__set_proximity);
	package.add("set_proximity", (void(*)(float, float, float, float, float, float))&Perl__set_proximity);
	package.add("set_proximity", (void(*)(float, float, float, float, float, float, bool))&Perl__set_proximity);
	package.add("set_zone_flag", &Perl__set_zone_flag);
	package.add("setallskill", &Perl__setallskill);
	package.add("setanim", &Perl__setanim);
	package.add("setexpmodifierbycharid", (void(*)(uint32, uint32, double))&Perl__setexpmodifierbycharid);
	package.add("setexpmodifierbycharid", (void(*)(uint32, uint32, double, int16))&Perl__setexpmodifierbycharid);
	package.add("setglobal", &Perl__setglobal);
	package.add("setguild", &Perl__setguild);
	package.add("sethp", &Perl__sethp);
	package.add("setlanguage", &Perl__setlanguage);
	package.add("setnexthpevent", &Perl__setnexthpevent);
	package.add("setnextinchpevent", &Perl__setnextinchpevent);
	package.add("setskill", &Perl__setskill);
	package.add("setsky", &Perl__setsky);
	package.add("setstat", &Perl__setstat);
	package.add("settarget", &Perl__settarget);
	package.add("settime", (void(*)(int, int))&Perl__settime);
	package.add("settime", (void(*)(int, int, bool))&Perl__settime);
	package.add("settimer", &Perl__settimer);
	package.add("settimerMS", &Perl__settimerMS);
	package.add("sfollow", &Perl__sfollow);
	package.add("shout", &Perl__shout);
	package.add("shout2", &Perl__shout2);
	package.add("showgrid", &Perl__showgrid);
	package.add("signal", (void(*)(int))&Perl__signal);
	package.add("signal", (void(*)(int, int))&Perl__signal);
	package.add("signalwith", (void(*)(int, int))&Perl__signalwith);
	package.add("signalwith", (void(*)(int, int, int))&Perl__signalwith);
	package.add("snow", &Perl__snow);
	package.add("spawn", &Perl__spawn);
	package.add("spawn2", &Perl__spawn2);
	package.add("spawn_condition", (void(*)(const char*, uint16, int16))&Perl__spawn_condition);
	package.add("spawn_condition", (void(*)(const char*, uint32_t, uint16, int16))&Perl__spawn_condition);
	package.add("spawn_from_spawn2", &Perl__spawn_from_spawn2);
	package.add("start", &Perl__start);
	package.add("stop", &Perl__stop);
	package.add("stopalltimers", &Perl__stopalltimers);
	package.add("stoptimer", &Perl__stoptimer);
	package.add("summonallplayercorpses", &Perl__summonallplayercorpses);
	package.add("summonburiedplayercorpse", &Perl__summonburiedplayercorpse);
	package.add("summonitem", (void(*)(int))&Perl__summonitem);
	package.add("summonitem", (void(*)(int, int))&Perl__summonitem);
	package.add("surname", &Perl__surname);
	package.add("targlobal", &Perl__targlobal);
	package.add("toggle_spawn_event", &Perl__toggle_spawn_event);
	package.add("toggledoorstate", &Perl__toggledoorstate);
	package.add("tracknpc", &Perl__tracknpc);
	package.add("traindisc", &Perl__traindisc);
	package.add("traindiscs", (int(*)(int))&Perl__traindiscs);
	package.add("traindiscs", (int(*)(int, int))&Perl__traindiscs);
	package.add("unique_spawn", (int(*)(int, int, int, float, float, float))&Perl__unique_spawn);
	package.add("unique_spawn", (int(*)(int, int, int, float, float, float, float))&Perl__unique_spawn);
	package.add("unscribespells", &Perl__unscribespells);
	package.add("untraindiscs", &Perl__untraindiscs);
	package.add("updatespawntimer", &Perl__UpdateSpawnTimer);
	package.add("UpdateZoneHeader", &Perl__UpdateZoneHeader);
	package.add("varlink", &Perl__varlink);
	package.add("voicetell", &Perl__voicetell);
	package.add("we", &Perl__we);
	package.add("wearchange", (void(*)(uint8, uint16))&Perl__wearchange);
	package.add("wearchange", (void(*)(uint8, uint16, uint32))&Perl__wearchange);
	package.add("wearchange", (void(*)(uint8, uint16, uint32, uint32))&Perl__wearchange);
	package.add("whisper", &Perl__whisper);
	package.add("write", &Perl__write);
	package.add("ze", &Perl__ze);
	package.add("zone", &Perl__zone);
	package.add("zonemarquee", (void(*)(uint32, std::string))&Perl__zonemarquee);
	package.add("zonemarquee", (void(*)(uint32, std::string, uint32))&Perl__zonemarquee);
	package.add("zonemarquee", (void(*)(uint32, uint32, uint32, uint32, uint32, std::string))&Perl__zonemarquee);
	package.add("zonegroup", &Perl__zonegroup);
	package.add("zoneraid", &Perl__zoneraid);

	/**
	 * Expansions
	 */
	package.add("is_classic_enabled", &Perl__IsClassicEnabled);
	package.add("is_the_ruins_of_kunark_enabled", &Perl__IsTheRuinsOfKunarkEnabled);
	package.add("is_the_scars_of_velious_enabled", &Perl__IsTheScarsOfVeliousEnabled);
	package.add("is_the_shadows_of_luclin_enabled", &Perl__IsTheShadowsOfLuclinEnabled);
	package.add("is_the_planes_of_power_enabled", &Perl__IsThePlanesOfPowerEnabled);
	package.add("is_the_legacy_of_ykesha_enabled", &Perl__IsTheLegacyOfYkeshaEnabled);
	package.add("is_lost_dungeons_of_norrath_enabled", &Perl__IsLostDungeonsOfNorrathEnabled);
	package.add("is_gates_of_discord_enabled", &Perl__IsGatesOfDiscordEnabled);
	package.add("is_omens_of_war_enabled", &Perl__IsOmensOfWarEnabled);
	package.add("is_dragons_of_norrath_enabled", &Perl__IsDragonsOfNorrathEnabled);
	package.add("is_depths_of_darkhollow_enabled", &Perl__IsDepthsOfDarkhollowEnabled);
	package.add("is_prophecy_of_ro_enabled", &Perl__IsProphecyOfRoEnabled);
	package.add("is_the_serpents_spine_enabled", &Perl__IsTheSerpentsSpineEnabled);
	package.add("is_the_buried_sea_enabled", &Perl__IsTheBuriedSeaEnabled);
	package.add("is_secrets_of_faydwer_enabled", &Perl__IsSecretsOfFaydwerEnabled);
	package.add("is_seeds_of_destruction_enabled", &Perl__IsSeedsOfDestructionEnabled);
	package.add("is_underfoot_enabled", &Perl__IsUnderfootEnabled);
	package.add("is_house_of_thule_enabled", &Perl__IsHouseOfThuleEnabled);
	package.add("is_veil_of_alaris_enabled", &Perl__IsVeilOfAlarisEnabled);
	package.add("is_rain_of_fear_enabled", &Perl__IsRainOfFearEnabled);
	package.add("is_call_of_the_forsaken_enabled", &Perl__IsCallOfTheForsakenEnabled);
	package.add("is_the_darkend_sea_enabled", &Perl__IsTheDarkenedSeaEnabled);
	package.add("is_the_broken_mirror_enabled", &Perl__IsTheBrokenMirrorEnabled);
	package.add("is_empires_of_kunark_enabled", &Perl__IsEmpiresOfKunarkEnabled);
	package.add("is_ring_of_scale_enabled", &Perl__IsRingOfScaleEnabled);
	package.add("is_the_burning_lands_enabled", &Perl__IsTheBurningLandsEnabled);
	package.add("is_torment_of_velious_enabled", &Perl__IsTormentOfVeliousEnabled);
	package.add("is_current_expansion_classic", &Perl__IsCurrentExpansionClassic);
	package.add("is_current_expansion_the_ruins_of_kunark", &Perl__IsCurrentExpansionTheRuinsOfKunark);
	package.add("is_current_expansion_the_scars_of_velious", &Perl__IsCurrentExpansionTheScarsOfVelious);
	package.add("is_current_expansion_the_shadows_of_luclin", &Perl__IsCurrentExpansionTheShadowsOfLuclin);
	package.add("is_current_expansion_the_planes_of_power", &Perl__IsCurrentExpansionThePlanesOfPower);
	package.add("is_current_expansion_the_legacy_of_ykesha", &Perl__IsCurrentExpansionTheLegacyOfYkesha);
	package.add("is_current_expansion_lost_dungeons_of_norrath", &Perl__IsCurrentExpansionLostDungeonsOfNorrath);
	package.add("is_current_expansion_gates_of_discord", &Perl__IsCurrentExpansionGatesOfDiscord);
	package.add("is_current_expansion_omens_of_war", &Perl__IsCurrentExpansionOmensOfWar);
	package.add("is_current_expansion_dragons_of_norrath", &Perl__IsCurrentExpansionDragonsOfNorrath);
	package.add("is_current_expansion_depths_of_darkhollow", &Perl__IsCurrentExpansionDepthsOfDarkhollow);
	package.add("is_current_expansion_prophecy_of_ro", &Perl__IsCurrentExpansionProphecyOfRo);
	package.add("is_current_expansion_the_serpents_spine", &Perl__IsCurrentExpansionTheSerpentsSpine);
	package.add("is_current_expansion_the_buried_sea", &Perl__IsCurrentExpansionTheBuriedSea);
	package.add("is_current_expansion_secrets_of_faydwer", &Perl__IsCurrentExpansionSecretsOfFaydwer);
	package.add("is_current_expansion_seeds_of_destruction", &Perl__IsCurrentExpansionSeedsOfDestruction);
	package.add("is_current_expansion_underfoot", &Perl__IsCurrentExpansionUnderfoot);
	package.add("is_current_expansion_house_of_thule", &Perl__IsCurrentExpansionHouseOfThule);
	package.add("is_current_expansion_veil_of_alaris", &Perl__IsCurrentExpansionVeilOfAlaris);
	package.add("is_current_expansion_rain_of_fear", &Perl__IsCurrentExpansionRainOfFear);
	package.add("is_current_expansion_call_of_the_forsaken", &Perl__IsCurrentExpansionCallOfTheForsaken);
	package.add("is_current_expansion_the_darkend_sea", &Perl__IsCurrentExpansionTheDarkenedSea);
	package.add("is_current_expansion_the_broken_mirror", &Perl__IsCurrentExpansionTheBrokenMirror);
	package.add("is_current_expansion_empires_of_kunark", &Perl__IsCurrentExpansionEmpiresOfKunark);
	package.add("is_current_expansion_ring_of_scale", &Perl__IsCurrentExpansionRingOfScale);
	package.add("is_current_expansion_the_burning_lands", &Perl__IsCurrentExpansionTheBurningLands);
	package.add("is_current_expansion_torment_of_velious", &Perl__IsCurrentExpansionTormentOfVelious);

	/**
	 * Content flags
	 */
	package.add("is_content_flag_enabled", &Perl__IsContentFlagEnabled);
	package.add("set_content_flag", &Perl__SetContentFlag);

}

#endif
#endif
