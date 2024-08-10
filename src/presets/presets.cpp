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

		for (size_t i = 0; i < effect->num_params; i++)
		{
			Param* param = &effect->params[i];
			effect_data->params.push_back(param_data_t {param->name, param->current_value});
		}

	}
	
	return result;
}


size_t save_presets(const preset_bank_t& bank){
	EepromStream eepromStream(0, 4096); // Teensy 4.1 should supply 4284 bytes

	JsonDocument preset_bank_json;
	JsonDocument presets_array_json;
	JsonArray presets_array = presets_array_json.to<JsonArray>();
	
	preset_bank_json["active"] = bank.active_preset;

	for (size_t preset_index = 0; preset_index < bank.num_presets; preset_index++)
	{
		preset_data_t data = bank.presets[preset_index];
		JsonDocument preset_json;
		JsonDocument effect_array_json;
		JsonArray effect_array = effect_array_json.to<JsonArray>();

		preset_json["name"] = data.name;

		for (size_t effect_index = 0; effect_index < bank.num_effects; effect_index++)
		{
			effect_data_t* effect = &data.effect_values[effect_index];
			JsonDocument effect_json_doc;
			effect_json_doc["name"] = effect->name;
			effect_json_doc["on"] = 1 - effect->bypass;
			JsonDocument params_json_doc;
			effect_json_doc["vals"] = params_json_doc.to<JsonArray>();

			for (size_t param_index = 0; param_index < effect->num_params; param_index++)
			{
				param_data_t param = effect->params[param_index];
				effect_json_doc["vals"].add(param.name + ":" + String(param.current_value));
			}

			effect_array.add(effect_json_doc);
		
		}

		preset_json["chain"] = effect_array_json;
		presets_array.add(preset_json);
	}

	preset_bank_json["presets"] = presets_array;
	

	// serialize the array and send the result to Serial
	Serial.println("Saving preset '" + bank.presets[0].name + "' ...");
	serializeJson(preset_bank_json, Serial);
	size_t len = serializeJson(preset_bank_json, eepromStream);
	Serial.println();
	Serial.println("Written preset in "+ String(len) + " bytes");

	return len;
}

void load_preset(Effect** effect_chain,  size_t num_effects){
	EepromStream eepromStream(0, 2048);

	JsonDocument doc;
	// doc["value"] = 42;
	// doc["lat"] = 48.748010;
	// doc["lon"] = 2.293491;

	ReadLoggingStream loggingStream(eepromStream, Serial);
	deserializeJson(doc, loggingStream);


	// serializeJson(doc, Serial);
}