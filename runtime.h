#pragma once
#include "script.h"
#include "variable.h"
#include <unordered_map>
#include <stack>
class runtime
{
private:
	typedef std::unordered_map<std::string, variable> variables;
	script m_script;
	variables m_global;
	std::stack<int> ret_adresses;
	std::unordered_map<std::string, size_t> functions;
	size_t ip;
public:
	runtime(const std::string& filename);
	std::string execute();// executes current line and returns it
private:
	int find(const std::string& source, const std::string& str);
};

