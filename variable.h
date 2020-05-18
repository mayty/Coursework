#pragma once
#include <iostream>
enum class var_types
{
	int_type, float_type, none
};

class variable
{
private:
	union
	{
		int int_value;
		float float_value;
	} value;
	var_types type;
public:
	variable();
	variable(int value);
	variable(float value);
	variable(const variable& other);
	variable& operator=(const variable& other);
	variable operator*(const variable& other);
	variable operator+(const variable& other);
	variable operator-(const variable& other);
	variable operator/(const variable& other);
	variable operator%(const variable& other);
	bool operator==(const variable& other);
	bool operator>(const variable& other);
	bool operator!=(const variable& other);
	bool operator<(const variable& other);
	bool operator>=(const variable& other);
	bool operator<=(const variable& other);
	operator int ();

	void reinit(int val);
	void reinit(float val);
	void reinit(const variable& other);
	friend std::ostream& operator<<(std::ostream& out, const variable& val);
};

