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