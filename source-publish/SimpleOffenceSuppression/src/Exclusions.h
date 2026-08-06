#pragma once

#include "Settings.h"

#include <mutex>
#include <string>
#include <unordered_set>
#include <vector>

// Resolves INI EditorIDs + optional FormList/Keyword after data load.
// Used to skip offence suppression for "friendly-looking" hostiles
// (toll bandits, Arvel, Weylin, quest neutrals in criminal factions, etc.).
class Exclusions
{
public:
	[[nodiscard]] static Exclusions* GetSingleton()
	{
		static Exclusions singleton;
		return std::addressof(singleton);
	}

	void CacheForms()
	{
		std::lock_guard lock(_mutex);
		_factions.clear();
		_npcBases.clear();
		_keyword = nullptr;
		_formList = nullptr;

		const auto* settings = Settings::GetSingleton();

		// Optional keyword (from our ESL or any plugin)
		if (settings->useKeywordExclusion && !settings->exclusionKeyword.empty()) {
			_keyword = RE::TESForm::LookupByEditorID<RE::BGSKeyword>(settings->exclusionKeyword);
			if (_keyword) {
				logger::info("Exclusion keyword resolved: {}", settings->exclusionKeyword);
			} else {
				logger::warn("Exclusion keyword not found (optional): {}", settings->exclusionKeyword);
			}
		}

		// Optional FormList (factions, keywords, AND/OR NPC bases)
		if (!settings->exclusionFormList.empty()) {
			_formList = RE::TESForm::LookupByEditorID<RE::BGSListForm>(settings->exclusionFormList);
			if (_formList) {
				logger::info("Exclusion FormList resolved: {} ({} entries)", settings->exclusionFormList, _formList->forms.size());
				for (auto* form : _formList->forms) {
					if (!form) {
						continue;
					}
					if (auto* fac = form->As<RE::TESFaction>()) {
						_factions.insert(fac);
					} else if (auto* npc = form->As<RE::TESNPC>()) {
						_npcBases.insert(npc);
					}
				}
			} else {
				logger::warn("Exclusion FormList not found (optional): {}", settings->exclusionFormList);
			}
		}

		// INI faction EditorIDs
		for (const auto& edid : settings->hostileFactionEditorIDs) {
			if (edid.empty()) {
				continue;
			}
			if (auto* fac = RE::TESForm::LookupByEditorID<RE::TESFaction>(edid)) {
				_factions.insert(fac);
			} else {
				logger::warn("Hostile faction EditorID not found: {}", edid);
			}
		}

		// INI unique NPC base EditorIDs (Weylin has no factions on the record)
		for (const auto& edid : settings->hostileNPCEditorIDs) {
			if (edid.empty()) {
				continue;
			}
			if (auto* npc = RE::TESForm::LookupByEditorID<RE::TESNPC>(edid)) {
				_npcBases.insert(npc);
				logger::info("Hostile NPC base resolved: {}", edid);
			} else {
				logger::warn("Hostile NPC EditorID not found: {}", edid);
			}
		}

		logger::info("Exclusion factions cached: {}", _factions.size());
		logger::info("Exclusion NPC bases cached: {}", _npcBases.size());
		logger::info("ExcludeVeryAggressive: {}", settings->excludeVeryAggressive);
		logger::info("UseKeywordExclusion: {}", settings->useKeywordExclusion);
	}

	// true = this actor should NOT receive friend/ally offence suppression
	[[nodiscard]] bool IsExcluded(RE::Actor* a_subject) const
	{
		if (!a_subject) {
			return false;
		}

		const auto* settings = Settings::GetSingleton();

		// Unique NPC bases (Weylin, Arvel, …) — works even with empty faction list
		if (!_npcBases.empty()) {
			if (auto* base = a_subject->GetActorBase()) {
				if (_npcBases.contains(base)) {
					return true;
				}
			}
		}

		// FormList may also hold NPC bases (already folded into _npcBases) + keywords
		if (_formList) {
			for (auto* form : _formList->forms) {
				if (auto* kywd = form ? form->As<RE::BGSKeyword>() : nullptr) {
					if (a_subject->HasKeyword(kywd)) {
						return true;
					}
				}
			}
		}

		// Very aggressive / frenzied (restores v1.1 behaviour)
		if (settings->excludeVeryAggressive) {
			const auto aggression = a_subject->GetActorValue(RE::ActorValue::kAggression);
			// 0 Unaggressive, 1 Aggressive, 2 VeryAggressive, 3 Frenzied
			if (aggression >= 2.0f) {
				return true;
			}
		}

		// Keyword (SPID-distributed or baked on record)
		if (_keyword && a_subject->HasKeyword(_keyword)) {
			return true;
		}

		// Faction membership (INI + FormList)
		if (!_factions.empty()) {
			for (auto* fac : _factions) {
				if (fac && a_subject->IsInFaction(fac)) {
					return true;
				}
			}
		}

		return false;
	}

private:
	Exclusions() = default;

	mutable std::mutex _mutex;
	std::unordered_set<RE::TESFaction*> _factions;
	std::unordered_set<RE::TESNPC*> _npcBases;
	RE::BGSKeyword* _keyword{ nullptr };
	RE::BGSListForm* _formList{ nullptr };
};
