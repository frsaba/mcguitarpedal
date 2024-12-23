#include "param.h"

//TODO: unit string (ms, %, etc)
//TODO: param descriptions
Param::Param(String name, float default_value, float min_value, float max_value, float step_size, std::function<void(float)> update_function, String icon, String unit)
    : default_value(default_value), 
      min_value(min_value),         
      max_value(max_value),         
      step_size(step_size),         
      update_function(update_function),
      name(name),
      icon(icon),                   
      current_value(default_value)
{
	//Put an extra space in front of the unit, but only if it's not there already
	//Except for the '%', which doesn't require a space
	//TODO: remove extra space where the unit is used
	this->unit = (unit == "%" || unit[0] == ' ') ? unit : " " + unit;
}

Param::~Param() {
}

Param::Param() {
	name = "Undefined param";
}

float Param::increment() {
    return change(1);
}

float Param::decrement() {
    return change(-1);
}

float Param::change(int steps){
	return set_value( current_value + steps * step_size);
}

float Param::set_value(float value){
	current_value = value;
    if(current_value < min_value) current_value = min_value;
	if(current_value > max_value) current_value = max_value;
	update_function(current_value);
    return current_value;
}

//Return the current value as a percentage along min and max
const float Param::get_as_percentage(){
	return (current_value - min_value) / (max_value - min_value) * 100;
}
