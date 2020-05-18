#pragma once
#include "game.h"
#include <string>
class drawer
{
public:
	static void print(const std::vector<std::vector<uint8_t>>& field);
	static void draw(const std::vector<std::vector<uint8_t>>& field);
	static void write(const std::string& message, int str_ind, int col_ind = 0);
};

