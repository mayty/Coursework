#include "game.h"
#include <random>

const int min_width = 10;
const int min_height = 10;
const int max_width = 38;
const int max_height = 22;

game::game(int width, int height)
{
	if (width < min_width)
	{
		width = min_width;
	}
	if (width > max_width)
	{
		width = max_width;
	}
	if (height < min_height)
	{
		height = min_height;
	}
	if (height > max_height)
	{
		height = max_height;
	}
	for (int i = 0; i < height; ++i)
	{
		field.push_back(std::vector<uint8_t>{});
		field[i].resize(width);
		for (int j = 0; j < width; ++j)
		{
			field[i][j] = (uint8_t)tile::nothing;
		}
	}

	score = 0;
	player_pos.X = 0;
	player_pos.Y = 0;
	m_state = state::regular;
	field[0][0] |= (uint8_t)tile::player;
	field[height - 1][width - 1] |= (uint8_t)tile::destination;
	std::random_device rd;
	std::mt19937 random(rd());
	for (int i = 0; i < height * width / 20; ++i)
	{
		field[random() % height][random() % width] |= (uint8_t)tile::food;
	}
	field[height - 1][0] |= (uint8_t)tile::food;
}

state game::moveUp()
{
	if (player_pos.Y <= 0)
	{
		player_pos.Y = 0;
		field[player_pos.Y][player_pos.X] |= (uint8_t)tile::player;
	}
	else
	{
		field[player_pos.Y][player_pos.X] &= ~(uint8_t)tile::player;
		--player_pos.Y;
		field[player_pos.Y][player_pos.X] |= (uint8_t)tile::player;
	}
	if (field[player_pos.Y][player_pos.X] & (uint8_t)tile::destination)
	{
		m_state = state::win;
	}
	if (field[player_pos.Y][player_pos.X] & (uint8_t)tile::food)
	{
		++score;
		field[player_pos.Y][player_pos.X] &= ~(uint8_t)tile::food;
	}
	return m_state;
}

state game::moveDown()
{
	if (player_pos.Y >= field.size() - 1)
	{
		player_pos.Y = field.size() - 1;
		field[player_pos.Y][player_pos.X] |= (uint8_t)tile::player;
	}
	else
	{
		field[player_pos.Y][player_pos.X] &= ~(uint8_t)tile::player;
		++player_pos.Y;
		field[player_pos.Y][player_pos.X] |= (uint8_t)tile::player;
	}
	if (field[player_pos.Y][player_pos.X] & (uint8_t)tile::destination)
	{
		m_state = state::win;
	}
	if (field[player_pos.Y][player_pos.X] & (uint8_t)tile::food)
	{
		++score;
		field[player_pos.Y][player_pos.X] &= ~(uint8_t)tile::food;
	}
	return m_state;
}

state game::moveLeft()
{
	if (player_pos.X <= 0)
	{
		player_pos.X = 0;
		field[player_pos.Y][player_pos.X] |= (uint8_t)tile::player;
	}
	else
	{
		field[player_pos.Y][player_pos.X] &= ~(uint8_t)tile::player;
		--player_pos.X;
		field[player_pos.Y][player_pos.X] |= (uint8_t)tile::player;
	}
	if (field[player_pos.Y][player_pos.X] & (uint8_t)tile::destination)
	{
		m_state = state::win;
	}
	if (field[player_pos.Y][player_pos.X] & (uint8_t)tile::food)
	{
		++score;
		field[player_pos.Y][player_pos.X] &= ~(uint8_t)tile::food;
	}
	return m_state;
}

state game::moveRight()
{
	if (player_pos.X >= field[0].size() - 1)
	{
		player_pos.X = field[0].size() - 1;
		field[player_pos.Y][player_pos.X] |= (uint8_t)tile::player;
	}
	else
	{
		field[player_pos.Y][player_pos.X] &= ~(uint8_t)tile::player;
		++player_pos.X;
		field[player_pos.Y][player_pos.X] |= (uint8_t)tile::player;
	}
	if (field[player_pos.Y][player_pos.X] & (uint8_t)tile::destination)
	{
		m_state = state::win;
	}
	if (field[player_pos.Y][player_pos.X] & (uint8_t)tile::food)
	{
		++score;
		field[player_pos.Y][player_pos.X] &= ~(uint8_t)tile::food;
	}
	return m_state;
}

int game::getScore()
{
	return score;
}

int game::playerX()
{
	return player_pos.X;
}

int game::playerY()
{
	return player_pos.Y;
}

int game::width()
{
	return field[0].size();
}

int game::height()
{
	return field.size();
}

tile game::getTile(int x, int y)
{
	if (x < 0)
	{
		return tile::wall;
	}
	if (x >= field[0].size())
	{
		return tile::wall;
	}
	if (y < 0)
	{
		return tile::wall;
	}
	if (y >= field.size())
	{
		return tile::wall;
	}
	if (field[y][x] & (uint8_t)tile::player)
	{
		return tile::player;
	}
	if (field[y][x] & (uint8_t)tile::destination)
	{
		return tile::destination;
	}
	if (field[y][x] & (uint8_t)tile::food)
	{
		return tile::food;
	}
	if (field[y][x] & (uint8_t)tile::wall)
	{
		return tile::wall;
	}
	return tile::nothing;
}

const std::vector<std::vector<uint8_t>>& game::getState()
{
	return field;
}
