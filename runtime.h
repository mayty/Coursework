#pragma once
#include "script.h"
#include "variable.h"
#include "game.h"
#include <unordered_map>
#include <stack>

extern std::string last_error;
extern bool updated_game;

class runtime
{
private:
	typedef std::unordered_map<std::string, variable> variables;
	typedef std::unordered_map<std::string, int> points;
	script m_script;
	game m_game;
	variables m_global;
	points m_points;
	std::stack<int> ret_adresses;
	std::stack<variable> m_stack;
	std::unordered_map<std::string, int> functions;
	int ip;
public:
	runtime(const std::string& filename);
	std::string execute();
private:
	int find(const std::string& source, const std::string& substr);
	int getType(const std::string& str);
	std::string getName(const std::string& str);
	std::string getWord(const std::string& str, size_t ind);
	variable get_var(const std::string& str);
};

