#pragma once
enum class var_types
{
	int_type, float_type
};

struct variable
{
	union
	{
		int int_value;
		float float_value;
	} value;
	var_types type;
	variable(int value);
	variable(float value);
};

