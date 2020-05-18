#include "variable.h"

variable::variable()
{
	type = var_types::none;
}

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

variable::variable(const variable& other)
	:type{ other.type }, value{other.value}
{
}

variable& variable::operator=(const variable& other)
{
	switch (type)
	{
	case var_types::int_type:
		switch (other.type)
		{
		case var_types::int_type:
			value.int_value = other.value.int_value;
			break;
		case var_types::float_type:
			value.int_value = other.value.float_value;
			break;
		case var_types::none:
			value.int_value = 0;
			break;
		}
		break;
	case var_types::float_type:
		switch (other.type)
		{
		case var_types::int_type:
			value.float_value = other.value.int_value;
			break;
		case var_types::float_type:
			value.float_value = other.value.float_value;
			break;
		case var_types::none:
			value.float_value = 0;
			break;
		}
		break;
	case var_types::none:
		break;
	}
	return *this;
}

variable variable::operator*(const variable& other)
{
	switch (type)
	{
	case var_types::int_type:
		switch (other.type)
		{
		case var_types::int_type:
			return value.int_value * other.value.int_value;
		case var_types::float_type:
			return value.int_value * other.value.float_value;
		case var_types::none:
			return variable{};
		}
	case var_types::float_type:
		switch (other.type)
		{
		case var_types::int_type:
			return value.float_value * other.value.int_value;
		case var_types::float_type:
			return value.float_value * other.value.float_value;
		case var_types::none:
			return variable{};
		}
	case var_types::none:
		return variable{};
	}
}

variable variable::operator+(const variable& other)
{
	switch (type)
	{
	case var_types::int_type:
		switch (other.type)
		{
		case var_types::int_type:
			return value.int_value + other.value.int_value;
		case var_types::float_type:
			return value.int_value + other.value.float_value;
		case var_types::none:
			return variable{};
		}
	case var_types::float_type:
		switch (other.type)
		{
		case var_types::int_type:
			return value.float_value + other.value.int_value;
		case var_types::float_type:
			return value.float_value + other.value.float_value;
		case var_types::none:
			return variable{};
		}
	case var_types::none:
		return variable{};
	}
}

variable variable::operator-(const variable& other)
{
	switch (type)
	{
	case var_types::int_type:
		switch (other.type)
		{
		case var_types::int_type:
			return value.int_value - other.value.int_value;
		case var_types::float_type:
			return value.int_value - other.value.float_value;
		case var_types::none:
			return variable{};
		}
	case var_types::float_type:
		switch (other.type)
		{
		case var_types::int_type:
			return value.float_value - other.value.int_value;
		case var_types::float_type:
			return value.float_value - other.value.float_value;
		case var_types::none:
			return variable{};
		}
	case var_types::none:
		return variable{};
	}
}

variable variable::operator/(const variable& other)
{
	switch (type)
	{
	case var_types::int_type:
		switch (other.type)
		{
		case var_types::int_type:
			return value.int_value / other.value.int_value;
		case var_types::float_type:
			return value.int_value / other.value.float_value;
		case var_types::none:
			return variable{};
		}
	case var_types::float_type:
		switch (other.type)
		{
		case var_types::int_type:
			return value.float_value / other.value.int_value;
		case var_types::float_type:
			return value.float_value / other.value.float_value;
		case var_types::none:
			return variable{};
		}
	case var_types::none:
		return variable{};
	}
}

variable variable::operator%(const variable& other)
{
	if (type != var_types::int_type || other.type != var_types::int_type)
	{
		throw std::exception{};
	}
	return variable{value.int_value % other.value.int_value};
}

bool variable::operator==(const variable& other)
{
	switch (type)
	{
	case var_types::int_type:
		switch (other.type)
		{
		case var_types::int_type:
			return value.int_value == other.value.int_value;
			break;
		case var_types::float_type:
			return value.int_value == other.value.float_value;
		case var_types::none:
			return false;
		}
	case var_types::float_type:
		switch (other.type)
		{
		case var_types::int_type:
			return value.float_value == other.value.int_value;
		case var_types::float_type:
			return value.float_value == other.value.float_value;
		case var_types::none:
			return false;
		}
	case var_types::none:
		return false;
	}
}

bool variable::operator>(const variable& other)
{
	switch (type)
	{
	case var_types::int_type:
		switch (other.type)
		{
		case var_types::int_type:
			return value.int_value > other.value.int_value;
			break;
		case var_types::float_type:
			return value.int_value > other.value.float_value;
		case var_types::none:
			return false;
		}
	case var_types::float_type:
		switch (other.type)
		{
		case var_types::int_type:
			return value.float_value > other.value.int_value;
		case var_types::float_type:
			return value.float_value > other.value.float_value;
		case var_types::none:
			return false;
		}
	case var_types::none:
		return false;
	}
}

bool variable::operator!=(const variable& other)
{
	return !(*this == other);
}

bool variable::operator<(const variable& other)
{
	return !(*this >= other);
}

bool variable::operator>=(const variable& other)
{
	return (*this > other) || (*this == other);
}

bool variable::operator<=(const variable& other)
{
	return (*this < other) || (*this == other);
}

variable::operator int()
{
	switch (type)
	{
	case var_types::int_type:
		return value.int_value;
	case var_types::float_type:
		return value.float_value;
	case var_types::none:
		return 0;
	}
}

void variable::reinit(int val)
{
	type = var_types::int_type;
	value.int_value = val;
}

void variable::reinit(float val)
{
	type = var_types::float_type;
	value.float_value = val;
}

void variable::reinit(const variable& other)
{
	type = other.type;
	value = other.value;
}

std::ostream& operator<<(std::ostream& out, const variable& val)
{
	switch (val.type)
	{
	case var_types::int_type:
		return out << val.value.int_value;
	case var_types::float_type:
		return out << val.value.float_value;
	case var_types::none:
		return out << "none";
	}
}
