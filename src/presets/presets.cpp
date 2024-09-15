#include <presets/presets.h>
#include <StreamUtils.h>
#include <ArduinoJson.h>

void save_preset(Effect** effect_chain, size_t num_effects){
	EepromStream eepromStream(0, 2048);

	JsonDocument preset_json;
	JsonDocument effect_array_json;
	JsonArray array = effect_array_json.to<JsonArray>();

	preset_json["name"] = "Preset 1";

	for (size_t i = 0; i < num_effects; i++)
	{
		Effect* effect = effect_chain[i];
		JsonDocument effect_json_doc;
		effect_json_doc["name"] = effect->name;
		effect_json_doc["on"] = 1 - effect->bypass;
		JsonDocument params_json_doc;
		effect_json_doc["vals"] = params_json_doc.to<JsonArray>();

		for (size_t j = 0; j < effect->num_params; j++)
		{
			Param param = effect->params[j];
			effect_json_doc["vals"].add(param.name + ":" + String(param.current_value));
		}

		array.add(effect_json_doc);
		
	}

	preset_json["chain"] = effect_array_json;

	// serialize the array and send the result to Serial
	// serializeJson(doc, Serial);
	Serial.println("Saving preset...");
	size_t len = serializeJson(preset_json, eepromStream);
	Serial.println("Written preset in "+ String(len) + " bytes");
}
//TODO: make sure to free all arrays in the preset data when it is overwritten
preset_data_t effects_to_preset_data(String preset_name, Effect *effects_chain[]){
	preset_data_t result;

	result.name = preset_name;
	for (size_t i = 0; i < CHAIN_LENGTH; i++)
	{
		effect_data_t* effect_data = &result.effect_values[i];
		const Effect* effect = effects_chain[i];

		//TODO: turn this into a class with the constructor doing this?
		effect_data->name = effect->name;
		effect_data->bypass = effect->bypass;
		effect_data->num_params = effect->num_params;

		effect_data->params.clear();
		for (size_t j = 0; j < effect->num_params; j++)
		{
			Param* param = &effect->params[j];
			effect_data->params.push_back(param_data_t {param->name, param->current_value});
		}

	}
	
	return result;
}


size_t serialize_presets(const preset_bank_t& bank, bool write_to_eeprom){
	JsonDocument preset_bank_json;
	JsonDocument presets_array_json;
	JsonArray presets_array = presets_array_json.to<JsonArray>();
	
	preset_bank_json["active"] = bank.active_preset;

	for (size_t preset_index = 0; preset_index < bank.num_presets; preset_index++)
	{
		preset_data_t preset_data = bank.presets[preset_index];
		JsonDocument preset_json;
		JsonDocument effect_array_json;
		JsonArray effect_array = effect_array_json.to<JsonArray>();

		//TODO: fix preset name (missing number)
		preset_json["name"] = preset_data.name;

		for (size_t effect_index = 0; effect_index < bank.num_effects; effect_index++)
		{
			effect_data_t* effect_data = &preset_data.effect_values[effect_index];
			JsonDocument effect_json_doc;
			effect_json_doc["name"] = effect_data->name;
			effect_json_doc["on"] = 1 - effect_data->bypass;
			JsonDocument params_json_doc;
			effect_json_doc["vals"] = params_json_doc.to<JsonArray>();

			for (size_t param_index = 0; param_index < effect_data->num_params; param_index++)
			{
				param_data_t param = effect_data->params[param_index];
				effect_json_doc["vals"].add(param.name + ":" + String(param.current_value));
			}

			effect_array.add(effect_json_doc);
		
		}

		preset_json["chain"] = effect_array_json;
		presets_array.add(preset_json);
	}

	preset_bank_json["presets"] = presets_array;
	
	// serialize the array and send the result to Serial
	Serial.println("Serialized preset bank:\n");
	size_t len = serializeJson(preset_bank_json, Serial);
	Serial.println();
	//Write to EEPROM
	if(write_to_eeprom){
		Serial.println(String("Saving preset '") + bank.presets[0].name + String("' to EEPROM..."));
		EepromStream eepromStream(0, 4096); // Teensy 4.1 should supply 4284 bytes
		serializeJson(preset_bank_json, eepromStream);
		Serial.println("Preset saved");
	}
	Serial.println();
	Serial.println("Preset size "+ String(len) + " bytes");

	return len;
}

void load_presets(preset_bank_t* bank){
	EepromStream eepromStream(0, 4096);
	JsonDocument doc;

	DeserializationError error = deserializeJson(doc, eepromStream);

	if (error) {
		displayText("deserializeJson() failed: ");
		Serial.println(error.c_str());
		//TODO: what should we do if deserialization fails?
		return;
	}

	serializeJson(doc, Serial);
	

	if(bank == nullptr){
		displayText("Preset bank is null");
		return;
	}

	bank->active_preset = doc["active"];

	JsonArray presets_array = doc["presets"];

	size_t preset_index = 0;
	for(JsonObject preset_json : presets_array){

		preset_data_t* preset_data =  &bank->presets[preset_index];
		preset_data->name = String(preset_json["name"]);
		LV_LOG_USER("Loading saved preset '%s'", preset_data->name.begin());
		size_t effect_index = 0;
		for (JsonObject effect_json_doc : preset_json["chain"].as<JsonArray>()) {
			effect_data_t* effect_data = &preset_data->effect_values[effect_index];

			effect_data->name = String(effect_json_doc["name"]);
			effect_data->bypass = 1 - int(effect_json_doc["on"]);

			JsonArray params_json_array = effect_json_doc["vals"];
			effect_data->num_params = params_json_array.size();
			LV_LOG_USER("Loading data for effect '%s'", effect_data->name.begin());

			for (String value_string : effect_json_doc["vals"].as<JsonArray>() )
			{
				param_data_t param;

				size_t colon_index = value_string.lastIndexOf(':');

				param.name = value_string.substring(0, colon_index);
				param.current_value = value_string.substring(colon_index + 1).toFloat();

				// Serial.printf("%s %f\n", param->name, param->current_value);
				char buffer[64];
				snprintf(buffer, sizeof(buffer), "%s:%s = %f\n", effect_data->name.begin(), param.name.begin(), param.current_value);
				Serial.print(buffer);

				effect_data->params.push_back(param);
				// effect_json_doc["vals"].add(param.name + ":" + String(param.current_value));
			}

			effect_index++;
		}

		preset_index++;
	}

	// for (JsonObject presets_0_chain_item : presets_array[0]["chain"].as<JsonArray>()) {

	// const char* presets_0_chain_item_name = presets_0_chain_item["name"]; // "Chorus", "Tremolo", "Delay", ...
	// int presets_0_chain_item_on = presets_0_chain_item["on"]; // 1, 1, 1, 1

	// JsonArray presets_0_chain_item_vals = presets_0_chain_item["vals"];
	// const char* presets_0_chain_item_vals_0 = presets_0_chain_item_vals[0]; // "Dry/Wet:40.00", ...
	// const char* presets_0_chain_item_vals_1 = presets_0_chain_item_vals[1]; // "Voices:3.00", "Rate:4.00", ...
	// const char* presets_0_chain_item_vals_2 = presets_0_chain_item_vals[2]; // "Length:16.00", "Depth:1.00", ...

	// }
}


void apply_preset_values(effect_data_t effect_values[], Effect** effect_chain,  size_t num_effects){

	for (size_t i = 0; i < num_effects; i++)
	{
		if(effect_values[i].name == ""){
			LV_LOG_WARN("Effect %d name was empty. Are we loading an empty preset?", i);
			continue;
		}

		Effect* effect = findEffectByName(effect_values[i].name, effect_chain, num_effects);

		if(effect == nullptr){
			Serial.print(F("Could not find effect: "));
			Serial.println(effect_values[i].name);
			continue;
		}
		
		for (param_data_t param_data : effect_values[i].params)
		{
			Param* param = findParamByName(param_data.name, effect);

			if(effect == nullptr){
				Serial.print(F("Could not find param: "));
				Serial.println(effect->name + ' '+ param_data.name);
			}

			param->set_value(param_data.current_value);

			char buffer[64];
			snprintf(buffer, sizeof(buffer), "Set %s:%s to %f\n", effect->name.begin(), param->name.begin(), param->current_value);
			Serial.print(buffer);
		}

		effect->set_bypass(effect_values[i].bypass);
		char buffer[64];
		snprintf(buffer, sizeof(buffer), "Set %s:bypass to %d\n", effect->name.begin(), effect->bypass);
		Serial.print(buffer);

	}
}

Effect* findEffectByName(String name, Effect** effect_chain, size_t num_effects){
	for (size_t i = 0; i < num_effects; i++)
	{
		if(effect_chain[i]->name == name) return effect_chain[i];
	}
	return nullptr;
}

Param* findParamByName(String name, Effect* effect){
	for (size_t i = 0; i < effect->num_params; i++)
	{
		if(effect->params[i].name == name) return &effect->params[i];
	}
	return nullptr;
}