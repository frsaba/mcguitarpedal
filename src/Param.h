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
	float current_value;
	Param();
	Param(String name, float default_value, float min_value, float max_value, float step_size, std::function<void(float)> update_function);
	~Param();
	float increment();
	float decrement();
	float change(int steps);
};