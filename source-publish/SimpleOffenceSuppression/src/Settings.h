#pragma once

#include <string>
#include <vector>

class Settings
{
public:
	[[nodiscard]] static Settings* GetSingleton()
	{
		static Settings singleton;
		return std::addressof(singleton);
	}

	void Load()
	{
		constexpr auto path = L"Data/SKSE/Plugins/po3_SimpleOffenceSuppression.ini";

		CSimpleIniA ini;
		ini.SetUnicode();

		ini.LoadFile(path);

		detail::get_value(ini, setAsAlly, "Settings", "SetAsAlly", ";Whether neutral NPCs should be considered as friends or allies when hitting them. This affects the number of hits they'll take before turning hostile (see gamesettings)");
		detail::get_value(ini, onlyCombat, "Settings", "OnlyCombat", ";Neutral NPCs will only ignore player hits during combat");
		detail::get_value(ini, ignoreFriendlyFire, "Settings", "IgnoreFriendlyFire", ";Overrides the above settings. Neutral NPCs will ALWAYS ignore hits from the player");
		detail::get_value(ini, changeGameSettings, "Settings", "ModifyGameSettings", ";Modify following gamesettings using config. These can still be modified by other mods during playtime");

		detail::get_value(ini, iFriendHitNonCombatAllowed, "GameSettings", "iFriendHitNonCombatAllowed", nullptr);
		detail::get_value(ini, iFriendHitCombatAllowed, "GameSettings", "iFriendHitCombatAllowed", nullptr);
		detail::get_value(ini, iAllyHitNonCombatAllowed, "GameSettings", "iAllyHitNonCombatAllowed", nullptr);
		detail::get_value(ini, iAllyHitCombatAllowed, "GameSettings", "iAllyHitCombatAllowed", nullptr);

		// --- Hostile Safeguard (fork) ---
		detail::get_value(ini, excludeVeryAggressive, "Exclusions", "ExcludeVeryAggressive", ";If true, Very Aggressive / Frenzied actors never get offence suppression (restored from SOS v1.1)");
		detail::get_value(ini, useKeywordExclusion, "Exclusions", "UseKeywordExclusion", ";If true, actors with the exclusion keyword never get offence suppression");
		detail::get_value(ini, exclusionKeyword, "Exclusions", "ExclusionKeyword", ";EditorID of keyword that marks 'hostile by nature' actors (SPID or plugin). Default SOS_NoOffenceSuppression");
		detail::get_value(ini, exclusionFormList, "Exclusions", "ExclusionFormList", ";Optional FormList EditorID of factions/keywords to exclude. Default SOS_HostileFactionList");
		detail::get_value(ini, hostileFactionsRaw, "Exclusions", "HostileFactions", ";Comma-separated faction EditorIDs that never receive offence suppression (toll bandits, Arvel's BanditAllyFaction, warlocks, etc.)");
		detail::get_value(ini, hostileNPCsRaw, "Exclusions", "HostileNPCs", ";Comma-separated NPC base EditorIDs that never receive offence suppression even with ZERO factions (Weylin, Arvel, scripted assassins, etc.)");

		hostileFactionEditorIDs = detail::split_csv(hostileFactionsRaw);
		hostileNPCEditorIDs = detail::split_csv(hostileNPCsRaw);

		if (setAsAlly) {
			newReaction = RE::FIGHT_REACTION::kAlly;
		}

		ini.SaveFile(path);
	}

	bool setAsAlly{ false };
	bool onlyCombat{ false };
	bool ignoreFriendlyFire{ false };

	bool changeGameSettings{ false };

	std::int32_t iFriendHitNonCombatAllowed{ 0 };
	std::int32_t iFriendHitCombatAllowed{ 4 };
	std::int32_t iAllyHitNonCombatAllowed{ 3 };
	std::int32_t iAllyHitCombatAllowed{ 1000 };

	RE::FIGHT_REACTION newReaction{ RE::FIGHT_REACTION::kFriend };

	// Hostile Safeguard
	bool excludeVeryAggressive{ true };
	bool useKeywordExclusion{ true };
	std::string exclusionKeyword{ "SOS_NoOffenceSuppression" };
	std::string exclusionFormList{ "SOS_HostileFactionList" };
	std::string hostileFactionsRaw{
		// Core criminal / enemy archetypes + Arvel's BanditAllyFaction
		"BanditFaction,BanditAllyFaction,BanditFriendFaction,"
		"WarlockFaction,ForswornFaction,NecromancerFaction,HagravenFaction,"
		"VampireFaction,VampireThrallFaction,WerewolfFaction,"
		"DraugrFaction,FalmerFaction,GiantFaction,TrollFaction,"
		"SprigganFaction,DwarvenAutomatonFaction,WolfFaction,"
		"ThalmorFaction,PenitusOculatusFaction,"
		// WE / quest bandit packs
		"WE06BanditFaction,WE19BanditFaction,WE20BanditFaction,"
		"MS07BanditFaction,MS07BanditSiblings,"
		"dunValtheimKeepBanditFaction,dunRobbersGorgeBanditFaction,"
		"DunAnsilvundBanditFaction,dunIcerunnerBanditFaction,"
		// DLC
		"DLC1VampireFaction,DLC1VampireCompanionFaction,"
		"DLC2CultistFaction,DLC2RieklingFaction,DLC2BanditDialogueFaction"
	};
	std::vector<std::string> hostileFactionEditorIDs;

	// Unique scripted "false friendlies" with empty faction lists (Weylin has ZERO factions)
	std::string hostileNPCsRaw{
		"MS01Weylin,"      // Markarth market Forsworn assassin vs Margret
		"MS13Arvel,"       // Bleak Falls claw thief
		"e3DemoArvel"      // demo/e3 Arvel variant if present
	};
	std::vector<std::string> hostileNPCEditorIDs;

private:
	Settings()
	{
		Load();
	}

	struct detail
	{
		static void get_value(CSimpleIniA& a_ini, bool& a_value, const char* a_section, const char* a_key, const char* a_comment)
		{
			a_value = a_ini.GetBoolValue(a_section, a_key, a_value);
			a_ini.SetBoolValue(a_section, a_key, a_value, a_comment);
		}

		static void get_value(CSimpleIniA& a_ini, std::int32_t& a_value, const char* a_section, const char* a_key, const char* a_comment)
		{
			try {
				a_value = static_cast<std::int32_t>(std::stol(a_ini.GetValue(a_section, a_key, std::to_string(a_value).c_str())));
			} catch (...) {
				// keep existing default
			}
			a_ini.SetValue(a_section, a_key, std::to_string(a_value).c_str(), a_comment);
		}

		static void get_value(CSimpleIniA& a_ini, std::string& a_value, const char* a_section, const char* a_key, const char* a_comment)
		{
			a_value = a_ini.GetValue(a_section, a_key, a_value.c_str());
			a_ini.SetValue(a_section, a_key, a_value.c_str(), a_comment);
		}

		static std::vector<std::string> split_csv(const std::string& a_raw)
		{
			std::vector<std::string> out;
			std::string current;
			for (char ch : a_raw) {
				if (ch == ',' || ch == ';' || ch == '\n' || ch == '\r') {
					// trim
					while (!current.empty() && (current.front() == ' ' || current.front() == '\t')) {
						current.erase(current.begin());
					}
					while (!current.empty() && (current.back() == ' ' || current.back() == '\t')) {
						current.pop_back();
					}
					if (!current.empty()) {
						out.push_back(current);
					}
					current.clear();
				} else {
					current.push_back(ch);
				}
			}
			while (!current.empty() && (current.front() == ' ' || current.front() == '\t')) {
				current.erase(current.begin());
			}
			while (!current.empty() && (current.back() == ' ' || current.back() == '\t')) {
				current.pop_back();
			}
			if (!current.empty()) {
				out.push_back(current);
			}
			return out;
		}
	};
};
