#pragma once
#include <string>
#include <functional>
#include <Arduino.h>

class Param
{
private:
	float default_value;
	float min_value;
	float max_value;
	float step_size;
	std::function<void(float)> update_function;
public:
	String name;
	String unit;
	float current_value;
	Param();
	Param(String name, float default_value, float min_value, float max_value, float step_size, std::function<void(float)> update_function, String unit = "");
	~Param();
	float increment();
	float decrement();
	float change(int steps);
	float get_as_percentage();
};