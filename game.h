#pragma once
#include <vector>
enum class state
{
	regular, win, lose
};

enum class tile
{
	nothing = 0b0000, player = 0b0001, food = 0b0010, destination = 0b0100, wall = 0b1000
};

class game
{
private:
#define X first
#define Y second
	std::pair<int, int> player_pos;
	state m_state;
	std::vector<std::vector<uint8_t>> field;
	int score;
public:
	game(int width, int height);
	state moveUp();
	state moveDown();
	state moveLeft();
	state moveRight();
	int getScore();
	int playerX();
	int playerY();
	int width();
	int height();
	tile getTile(int x, int y);
	const std::vector<std::vector<uint8_t>>& getState();
	
};

