#include "param.h"

Param::Param(String name, float default_value, float min_value, float max_value, float step_size, std::function<void(float)> update_function)
    : default_value(default_value), 
      min_value(min_value),         
      max_value(max_value),         
      step_size(step_size),         
      update_function(update_function),
      name(name),                   
      current_value(default_value)  
{
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
	current_value += steps * step_size;
    if(current_value < min_value) current_value = min_value;
	if(current_value > max_value) current_value = max_value;
	update_function(current_value);
    return current_value;
}
