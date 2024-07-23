#include <presets/presets.h>
#include <StreamUtils.h>
#include <ArduinoJson.h>

void save_preset(Effect** effect_chain, size_t num_effects){
	// EepromStream eepromStream(0, 2048);

	// JsonDocument doc;
	// doc["value"] = 42;
	// doc["lat"] = 48.748010;
	// doc["lon"] = 2.293491;
	// create an empty array
	JsonDocument doc;
	JsonArray array = doc.to<JsonArray>();

	for (size_t i = 0; i < num_effects; i++)
	{
		Effect* effect = effect_chain[i];
		JsonDocument effect_json_doc;
		effect_json_doc["effect"] = effect_chain[i]->name;
		JsonDocument params_json_doc;
		effect_json_doc["params"] = params_json_doc.to<JsonArray>();

		for (size_t j = 0; j < effect->num_params; j++)
		{
			// JsonDocument param_json;
			Param param = effect->params[j];

			// param_json["name"] = param.name;
			// param_json["value"] = param.current_value;

			effect_json_doc["params"].add(param.name + ":" + String(param.current_value));
		}

		array.add(effect_json_doc);
		
	}

	// serialize the array and send the result to Serial
	size_t len = serializeJson(doc, Serial);
	Serial.println("\nLength: "+ len);
}

void load_preset(Effect** effect_chain,  size_t num_effects){
	// EepromStream eepromStream(0, 2048);

	// JsonDocument doc;
	// doc["value"] = 42;
	// doc["lat"] = 48.748010;
	// doc["lon"] = 2.293491;


	// serializeJson(doc, Serial);
}