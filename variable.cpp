#include "variable.h"

variable::variable(int value)
{
	type = var_types::int_type;
	this->value.int_value = value;
}

variable::variable(float value)
{
	type = var_types::float_type;
	this->value.float_value = value;
}
