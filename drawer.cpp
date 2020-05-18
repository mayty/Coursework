#include "drawer.h"
#include <iostream>
#include <Windows.h>

static HANDLE consoleHandle;
static bool inited = false;


void drawer::print(const std::vector<std::vector<uint8_t>>& field)
{
	std::cout << "|";
	for (int i = 0; i < field[0].size(); ++i)
	{
		std::cout << "-";
	}
	std::cout << "|" << std::endl;
	for (auto& i : field)
	{
		std::cout << "|";
		for (auto& j : i)
		{
			if (j & (uint8_t)tile::player)
			{
				std::cout << "P";
			}
			else if (j & (uint8_t)tile::destination)
			{
				std::cout << "D";
			}
			else if (j & (uint8_t)tile::food)
			{
				std::cout << "F";
			}
			else if (j & (uint8_t)tile::wall)
			{
				std::cout << "W";
			}
			else
			{
				std::cout << " ";
			}
		}
		std::cout << "|" << std::endl;
	}
	std::cout << "|";
	for (int i = 0; i < field[0].size(); ++i)
	{
		std::cout << "-";
	}
	std::cout << "|" << std::endl;
}

void drawer::draw(const std::vector<std::vector<uint8_t>>& field)
{
	if (!inited)
	{
		consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		inited = true;
	}
	DWORD buf;
	char letter;
	COORD screen_pos;
	letter = '|';
	screen_pos.first = 0;
	screen_pos.second = 1;
	WriteConsoleOutputCharacterA(consoleHandle, &letter, 1, screen_pos, &buf);
	letter = '-';
	for (int i = 0; i < field[0].size(); ++i)
	{
		screen_pos.first = i + 1;
		WriteConsoleOutputCharacterA(consoleHandle, &letter, 1, screen_pos, &buf);
	}
	letter = '|';
	screen_pos.first = field[0].size() + 1;
	screen_pos.second = 1;
	WriteConsoleOutputCharacterA(consoleHandle, &letter, 1, screen_pos, &buf);

	for (int i = 0; i < field.size(); ++i)
	{
		letter = '|';
		screen_pos.first = 0;
		screen_pos.second = i + 2;
		WriteConsoleOutputCharacterA(consoleHandle, &letter, 1, screen_pos, &buf);
		for (int j = 0; j < field[i].size(); ++j)
		{
			screen_pos.first = j + 1;
			if (field[i][j] & (uint8_t)tile::player)
			{
				letter = 'P';
			}
			else if (field[i][j] & (uint8_t)tile::destination)
			{
				letter = 'D';
			}
			else if (field[i][j] & (uint8_t)tile::food)
			{
				letter = 'F';
			}
			else if (field[i][j] & (uint8_t)tile::wall)
			{
				letter = 'W';
			}
			else
			{
				letter = ' ';
			}
			WriteConsoleOutputCharacterA(consoleHandle, &letter, 1, screen_pos, &buf);
		}
		letter = '|';
		screen_pos.first = field[i].size() + 1;
		WriteConsoleOutputCharacterA(consoleHandle, &letter, 1, screen_pos, &buf);
	}

	letter = '|';
	screen_pos.first = 0;
	screen_pos.second = field.size() + 2;
	WriteConsoleOutputCharacterA(consoleHandle, &letter, 1, screen_pos, &buf);
	letter = '-';
	for (int i = 0; i < field[0].size(); ++i)
	{
		screen_pos.first = i + 1;
		WriteConsoleOutputCharacterA(consoleHandle, &letter, 1, screen_pos, &buf);
	}
	letter = '|';
	screen_pos.first = field[0].size() + 1;
	WriteConsoleOutputCharacterA(consoleHandle, &letter, 1, screen_pos, &buf);

}

void drawer::write(const std::string& message, int str_ind, int col_ind)
{
	if (!inited)
	{
		consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		inited = true;
	}
	DWORD buf;
	char letter;
	COORD screen_pos;

	screen_pos.second = str_ind;
	int i = 0;
	while (i < 40 && message[i])
	{
		letter = message[i];
		screen_pos.first = col_ind + i;
		WriteConsoleOutputCharacterA(consoleHandle, &letter, 1, screen_pos, &buf);
		++i;
	}
	letter = ' ';
	while (i < 40)
	{
		screen_pos.first = col_ind + i;
		WriteConsoleOutputCharacterA(consoleHandle, &letter, 1, screen_pos, &buf);
		++i;
	}
}
