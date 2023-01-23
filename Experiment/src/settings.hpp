#pragma once
#include "logger.hpp"

namespace big
{
	class settings;
	inline settings* g_settings{};

	class settings {
		nlohmann::json default_options;
		nlohmann::json options;

		struct self
		{
			bool infinite_psychic{ false };
			bool no_sas_cooldown{ false };
			bool infinite_sas_duration{ false };
			bool infinite_brain_dive{ false };
			bool instant_brain_field{ false };
			bool infinite_battle_point{ false };
			bool no_items_cooldown{ false };
			bool enter_brain_dive{ false };
			bool infinite_credits{ false };
			bool infinite_health{ false };
		};

		struct weapons
		{
			bool unlimited_ammo = false;
			bool unlimited_clip = false;
			bool unlimited_lethals = false;
			bool unlimited_tacticals = false;
		};
	public:
		settings()
		{
			g_settings = this;
		}

		~settings()
		{
			g_settings = nullptr;
		}

		self self{};
		weapons weapons{};

		void from_json(const nlohmann::json& j)
		{
			this->self.no_sas_cooldown = j["self"]["no_sas_cooldown"];
			this->self.infinite_psychic = j["self"]["infinite_psychic"];
			this->self.infinite_sas_duration = j["self"]["infinite_sas_duration"];
			this->self.infinite_brain_dive = j["self"]["infinite_brain_dive"];
			this->self.instant_brain_field = j["self"]["instant_brain_field"];
			this->self.infinite_battle_point = j["self"]["infinite_battle_point"];
			this->self.no_items_cooldown = j["self"]["no_items_cooldown"];
			this->self.enter_brain_dive = j["self"]["enter_brain_dive"];
			this->self.infinite_credits = j["self"]["infinite_credits"];
			this->self.infinite_health = j["self"]["infinite_health"];
		}

		nlohmann::json to_json()
		{
			return nlohmann::json{
				{
					"self", {
						{ "no_sas_cooldown", this->self.no_sas_cooldown },
						{ "infinite_psychic", this->self.infinite_psychic },
						{ "infinite_sas_duration", this->self.infinite_sas_duration },
						{ "infinite_brain_dive", this->self.infinite_brain_dive },
						{ "instant_brain_field", this->self.instant_brain_field },
						{ "infinite_battle_point", this->self.infinite_battle_point },
						{ "no_items_cooldown", this->self.no_items_cooldown },
						{ "enter_brain_dive", this->self.enter_brain_dive },
						{ "infinite_credits", this->self.infinite_credits },
						{ "infinite_health", this->self.infinite_health}
					}
				}
			};
		}

		void attempt_save()
		{
			nlohmann::json j = this->to_json();

			if (deep_compare(this->options, j, true))
				this->save();
		}

		bool load()
		{
			this->default_options = this->to_json();

			std::string settings_file = std::getenv("appdata");
			settings_file += this->settings_location;

			std::ifstream file(settings_file);

			if (!file.is_open())
			{
				this->write_default_config();

				file.open(settings_file);
			}

			try
			{
				file >> this->options;
			}
			catch (const std::exception&)
			{
				LOG(HACKER) << "Detected corrupt settings, writing default config...";

				this->write_default_config();

				return this->load();
			}

			bool should_save = this->deep_compare(this->options, this->default_options);

			this->from_json(this->options);

			if (should_save)
			{
				LOG(HACKER) << "Updating settings...";
				save();
			}

			return true;
		}

	private:
		const char* settings_location = "\\Scarlet Nexus Trainer\\settings.json";

		bool deep_compare(nlohmann::json& current_settings, const nlohmann::json& default_settings, bool compare_value = false)
		{
			bool should_save = false;

			for (auto& e : default_settings.items())
			{
				const std::string& key = e.key();

				if (current_settings.count(key) == 0 || (compare_value && current_settings[key] != e.value()))
				{
					current_settings[key] = e.value();

					should_save = true;
				}
				else if (current_settings[key].is_structured() && e.value().is_structured())
				{
					if (deep_compare(current_settings[key], e.value(), compare_value))
						should_save = true;
				}
				else if (!current_settings[key].is_structured() && e.value().is_structured()) {
					current_settings[key] = e.value();

					should_save = true;
				}
			}

			return should_save;
		}

		bool save()
		{
			std::string settings_file = std::getenv("appdata");
			settings_file += this->settings_location;

			std::ofstream file(settings_file, std::ios::out | std::ios::trunc);
			file << this->to_json().dump(4);
			file.close();

			return true;
		}

		bool write_default_config()
		{
			std::string settings_file = std::getenv("appdata");
			settings_file += this->settings_location;

			std::ofstream file(settings_file, std::ios::out);
			file << this->to_json().dump(4);
			file.close();

			return true;
		}
	};
}