#include "runtime.h"
#include <iostream>
#include <stack>
#include <Windows.h>

#define RUNTIME_DEBUG1
#define DEMO_DISPLAY1

#include "drawer.h"

#define MAX_STACK_SIZE 256

enum types {
	procedure, int_var, float_var, start, call, other, _mul, _div, _sub, _add, _rem, push, pop, move_up, move_down, move_left, move_right, map, ret, comp, point, ja, jb, je, print, jmp
};

std::string last_error{};
bool updated_game;

static bool comp_even;
static bool comp_greater;
static bool comp_less;

enum arg_type {
	var, raw_int, raw_float
};

static arg_type get_arg_type(const std::string& str);

runtime::runtime(const std::string& filename)
	:m_script{ filename }, m_game{ 100, 100 }
{

#ifdef RUNTIME_DEBUG
	{
		int j = 1;
		std::cout << "-------" << std::endl;
		for (auto& i : m_script.source)
		{
			std::cout << j++ << ": " << i << std::endl;
		}
		std::cout << "-------" << std::endl;
	}
#endif

#ifdef RUNTIME_DEBUG
	std::cout << "source code parcing started" << std::endl;
#endif

	bool found_start = false;
	for (int i = 0; i < m_script.source.size(); ++i)
	{
		switch (getType(m_script.source[i]))
		{
		case procedure:
			try
			{
				functions.emplace(getName(m_script.source[i]), i);
#ifdef RUNTIME_DEBUG
				std::cout << "emplaced function " << getName(m_script.source[i]) << std::endl;
#endif
			}
			catch (std::exception e)
			{
#ifdef RUNTIME_DEBUG
				std::cout << m_script.source[i] << ": invalid syntax" << std::endl;
#endif
				throw;
			}
			break;
		case int_var:
		{
			std::string int_name = getWord(m_script.source[i], 1);
			if (!getWord(m_script.source[i], 2).empty() || int_name.empty())
			{
				last_error = m_script.source[i] + ": invalid syntax";
				throw std::exception{};
			}
			if (int_name == "stack" || int_name == "stackp" || int_name == "none" || int_name == "player_x" || int_name == "player_y" || int_name == "field_width" || int_name == "field_height" ||
				int_name == "wall" || int_name == "nothing" || int_name == "player" || int_name == "food" || int_name == "destination")
			{
				last_error = m_script.source[i] + ": can't use this name";
				throw std::exception{};
			}
			if (m_global.find(int_name) == m_global.end())
			{
				m_global.emplace(int_name, 0);
#ifdef RUNTIME_DEBUG
				std::cout << "added int variable " << getWord(m_script.source[i], 1) << std::endl;
#endif
			}
			else
			{
#ifdef RUNTIME_DEBUG
				std::cout << "variable " << getWord(m_script.source[i], 1) << " already exists" << std::endl;
#endif
				last_error = getWord(m_script.source[i], 1) + " already exists";
				throw std::exception{};
			}
		}
			break;
		case float_var:
		{
			std::string float_name = getWord(m_script.source[i], 1);
			if (!getWord(m_script.source[i], 2).empty() || float_name.empty())
			{
				last_error = m_script.source[i] + ": invalid syntax";
				throw std::exception{};
			}
			if (float_name == "stack" || float_name == "stackp" || float_name == "none" || float_name == "player_x" || float_name == "player_y" || float_name == "field_width" || float_name == "field_height" ||
				float_name == "wall" || float_name == "nothing" || float_name == "player" || float_name == "food" || float_name == "destination")
			{
				last_error = m_script.source[i] + ": can't use this name";
				throw std::exception{};
			}
			if (m_global.find(float_name) == m_global.end())
			{
				m_global.emplace(float_name, 0.0f);
#ifdef RUNTIME_DEBUG
				std::cout << "added float variable " << getWord(m_script.source[i], 1) << std::endl;
#endif
			}
			else
			{
#ifdef RUNTIME_DEBUG
				std::cout << "variable " << getWord(m_script.source[i], 1) << " already exists" << std::endl;
#endif
				last_error = getWord(m_script.source[i], 1) + " already exists";
				throw std::exception{};
			}
		}
			break;
		case point:
		{
			std::string name = getWord(m_script.source[i], 0);
			if (name.empty() || !getWord(m_script.source[i], 1).empty())
			{
				last_error = m_script.source[i] + ": invalid syntax";
				throw std::exception{};
			}
			if (m_points.find(name) != m_points.end())
			{
#ifdef RUNTIME_DEBUG
				std::cout << "point " << getWord(m_script.source[i], 0) << " already exists" << std::endl;
#endif
				last_error = getWord(m_script.source[i], 1) + " already exists";
				throw std::exception{};
			}
			m_points.emplace(name, i);
#ifdef RUNTIME_DEBUG
			std::cout << "added point " << getWord(m_script.source[i], 0) << std::endl;
#endif
		}
		default:
			break;
		}
	}
	if (functions.find("main") != functions.end())
	{
		ip = (*functions.find("main")).second;
	}
	else
	{
		last_error = "entry point not found";
		throw std::exception{};
	}
#ifdef RUNTIME_DEBUG
	std::cout << "source code parcing ended" << std::endl;
	std::cout << "-------" << std::endl;
#endif
#ifndef DEMO_DISPLAY
	drawer::draw(m_game.getState());
	drawer::write("score: " + std::to_string(m_game.getScore()), 0);
#endif
}

std::string runtime::execute()
{
	if (ip == -1)
	{
		last_error = "programm has already ended";
		throw std::exception{};
	}
	std::string result{};
	result = std::to_string(ip + 1) + ": ";
	int val = getType(m_script.source[ip]);
	switch (val)
	{
	case ret:		// return from function
	{
		if (ret_adresses.empty())
		{
			ip = -1;
			return result + "ret";
		}
		ip = ret_adresses.top();
		ret_adresses.pop();
		return result + "ret";
	}
	case call:		// call function
	{
		if (functions.find(getName(m_script.source[ip])) != functions.end())
		{
			result += m_script.source[ip];
			ret_adresses.push(ip + 1);
			ip = (*functions.find(getName(m_script.source[ip]))).second;
			return result;
		}
		else
		{
			last_error = "unknown function name \"" + getName(m_script.source[ip]) + "\"";
			throw std::exception{};
		}
		break;
	}
	case _mul:		// multiply 2 variables
	{	
		if (m_stack.size() >= MAX_STACK_SIZE)
		{
			last_error = "stack overflow";
			throw std::exception{};
		}

		result += m_script.source[ip];
		std::string first_arg = getWord(m_script.source[ip], 1);
		std::string second_arg = getWord(m_script.source[ip], 2);

		if (!getWord(m_script.source[ip], 3).empty() || first_arg.empty() || second_arg.empty())
		{
			last_error = m_script.source[ip] + ": invalid syntax";
			throw std::exception{};
		}

		variable var1;
		variable var2;

		try
		{
			var1.reinit(get_var(first_arg));
			var2.reinit(get_var(second_arg));
		}
		catch (...)
		{
			last_error = m_script.source[ip] + ": unknown variable name";
			throw std::exception{};
		}

		m_stack.push(var1 * var2);
#ifdef RUNTIME_DEBUG
		std::cout << "new value: " << m_stack.top() << std::endl;
		std::cout << "new stack size = " << m_stack.size() << std::endl;
#endif

		++ip;
		return result;
	}
	case _div:		// divide 2 varibles
	{
		if (m_stack.size() >= MAX_STACK_SIZE)
		{
			last_error = "stack overflow";
			throw std::exception{};
		}
		result += m_script.source[ip];
		std::string first_arg = getWord(m_script.source[ip], 1);
		std::string second_arg = getWord(m_script.source[ip], 2);

		if (!getWord(m_script.source[ip], 3).empty() || first_arg.empty() || second_arg.empty())
		{
			last_error = m_script.source[ip] + ": invalid syntax";
			throw std::exception{};
		}

		variable var1;
		variable var2;

		try
		{
			var1.reinit(get_var(first_arg));
			var2.reinit(get_var(second_arg));
		}
		catch(...)
		{
			last_error = m_script.source[ip] + ": unknown variable name";
			throw std::exception{};
		}

		m_stack.push(var1 / var2);
#ifdef RUNTIME_DEBUG
		std::cout << "new value: " << m_stack.top() << std::endl;
		std::cout << "new stack size = " << m_stack.size() << std::endl;
#endif

		++ip;
		return result;
	}
	case _sub:		// substitute 2 variables
	{
		if (m_stack.size() >= MAX_STACK_SIZE)
		{
			last_error = "stack overflow";
			throw std::exception{};
		}
		result += m_script.source[ip];
		std::string first_arg = getWord(m_script.source[ip], 1);
		std::string second_arg = getWord(m_script.source[ip], 2);

		if (!getWord(m_script.source[ip], 3).empty() || first_arg.empty() || second_arg.empty())
		{
			last_error = m_script.source[ip] + ": invalid syntax";
			throw std::exception{};
		}

		variable var1;
		variable var2;

		try
		{
			var1.reinit(get_var(first_arg));
			var2.reinit(get_var(second_arg));
		}
		catch (...)
		{
			last_error = m_script.source[ip] + ": unknown variable name";
			throw std::exception{};
		}

		m_stack.push(var1 - var2);
#ifdef RUNTIME_DEBUG
		std::cout << "new value: " << m_stack.top() << std::endl;
		std::cout << "new stack size = " << m_stack.size() << std::endl;
#endif

		++ip;
		return result;
	}
	case _add:		// add 2 variables
	{
		if (m_stack.size() >= MAX_STACK_SIZE)
		{
			last_error = "stack overflow";
			throw std::exception{};
		}
		result += m_script.source[ip];
		std::string first_arg = getWord(m_script.source[ip], 1);
		std::string second_arg = getWord(m_script.source[ip], 2);

		if (!getWord(m_script.source[ip], 3).empty() || first_arg.empty() || second_arg.empty())
		{
			last_error = m_script.source[ip] + ": invalid syntax";
			throw std::exception{};
		}

		variable var1;
		variable var2;

		try
		{
			var1.reinit(get_var(first_arg));
			var2.reinit(get_var(second_arg));
		}
		catch (...)
		{
			last_error = m_script.source[ip] + ": unknown variable name";
			throw std::exception{};
		}

		m_stack.push(var1 + var2);
#ifdef RUNTIME_DEBUG
		std::cout << "new value: " << m_stack.top() << std::endl;
		std::cout << "new stack size = " << m_stack.size() << std::endl;
#endif

		++ip;
		return result;
	}
	case _rem:		// get remainder
	{
		if (m_stack.size() >= MAX_STACK_SIZE)
		{
			last_error = "stack overflow";
			throw std::exception{};
		}
		result += m_script.source[ip];
		std::string first_arg = getWord(m_script.source[ip], 1);
		std::string second_arg = getWord(m_script.source[ip], 2);

		if (!getWord(m_script.source[ip], 3).empty() || first_arg.empty() || second_arg.empty())
		{
			last_error = m_script.source[ip] + ": invalid syntax";
			throw std::exception{};
		}

		variable var1;
		variable var2;

		try
		{
			var1.reinit(get_var(first_arg));
			var2.reinit(get_var(second_arg));
		}
		catch (...)
		{
			last_error = m_script.source[ip] + ": unknown variable name";
			throw std::exception{};
		}

		try
		{
			m_stack.push(var1 % var2);
		}
		catch (...)
		{
			last_error = m_script.source[ip] + ": can only get remainder of integer value";
			throw std::exception{};
		}

#ifdef RUNTIME_DEBUG
		std::cout << "new value: " << m_stack.top() << std::endl;
		std::cout << "new stack size = " << m_stack.size() << std::endl;
#endif

		++ip;
		return result;
	}
	case push:		// push variable to stack
	{
		result += m_script.source[ip];

		variable var1;

		std::string arg = getWord(m_script.source[ip], 1);
		if (arg.empty() || !getWord(m_script.source[ip], 2).empty())
		{
			last_error = m_script.source[ip] + ": syntax error";
			throw std::exception{};
		}

		if (m_stack.size() >= MAX_STACK_SIZE)
		{
			last_error = "stack overflow";
			throw std::exception{};
		}

		if (arg == "none")
		{
			last_error = m_script.source[ip] + ": syntax error";
			throw std::exception{};
		}
		var1.reinit(get_var(arg));

		m_stack.push(var1);

#ifdef RUNTIME_DEBUG
		std::cout << "pushed to stack " << m_stack.top() << std::endl;
		std::cout << "new stack size = " << m_stack.size() << std::endl;
#endif

		++ip;
		return result;
	}
	case pop:		// pop stack to variable
	{
		result += m_script.source[ip];

		if (m_stack.empty())
		{
			last_error = "stack is empty";
			throw std::exception{};
		}

		std::string arg = getWord(m_script.source[ip], 1);
		if (arg.empty() || !getWord(m_script.source[ip], 2).empty())
		{
			last_error = m_script.source[ip] + ": syntax error";
			throw std::exception{};
		}

		if (arg != "none")
		{
			if (m_global.find(arg) == m_global.end())
			{
				last_error = "variable " + arg + " not found";
				throw std::exception{};
			}

			m_global.at(arg) = m_stack.top();
			m_stack.pop();
#ifdef RUNTIME_DEBUG
		std::cout << arg << " new value: " << m_global.at(arg) << std::endl;
		std::cout << "new stack size = " << m_stack.size() << std::endl;
#endif
		}
		else
		{
			m_stack.pop();
#ifdef RUNTIME_DEBUG
			std::cout << "new stack size = " << m_stack.size() << std::endl;
#endif
		}

		++ip;
		return result;
	}
	case comp:		// compare 2 variables and set flags
	{
		result += m_script.source[ip];

		std::string first_arg = getWord(m_script.source[ip], 1);
		std::string second_arg = getWord(m_script.source[ip], 2);

		if (!getWord(m_script.source[ip], 3).empty() || first_arg.empty() || second_arg.empty())
		{
			last_error = m_script.source[ip] + ": invalid syntax";
			throw std::exception{};
		}

		variable var1;
		variable var2;

		try
		{
			var1.reinit(get_var(first_arg));
			var2.reinit(get_var(second_arg));
		}
		catch (...)
		{
			last_error = m_script.source[ip] + ": unknown variable name";
			throw std::exception{};
		}

		if (var1 == var2)
		{
			comp_even = true;
			comp_greater = false;
			comp_less = false;
#ifdef RUNTIME_DEBUG
			std::cout << var1 << " = " << var2 << std::endl;
#endif
		}
		else if (var1 > var2)
		{
			comp_even = false;
			comp_greater = true;
			comp_less = false;
#ifdef RUNTIME_DEBUG
			std::cout << var1 << " > " << var2 << std::endl;
#endif
		}
		else
		{
			comp_even = false;
			comp_greater = false;
			comp_less = true;
#ifdef RUNTIME_DEBUG
			std::cout << var1 << " < " << var2 << std::endl;
#endif
		}

		++ip;
		return result;
	}
	case ja:
	{
		result += m_script.source[ip];

		std::string arg = getWord(m_script.source[ip], 1);
		if (arg.empty() || !getWord(m_script.source[ip], 2).empty())
		{
			last_error = m_script.source[ip] + ": syntax error";
			throw std::exception{};
		}

		if (m_points.find(arg) == m_points.end())
		{
			last_error = "point " + arg + " not found";
			throw std::exception{};
		}

		if (comp_greater)
		{
			ip = m_points.at(arg);
		}
		else
		{
			++ip;
		}

		return result;
	}
	case jb:
	{
		result += m_script.source[ip];

		std::string arg = getWord(m_script.source[ip], 1);
		if (arg.empty() || !getWord(m_script.source[ip], 2).empty())
		{
			last_error = m_script.source[ip] + ": syntax error";
			throw std::exception{};
		}

		if (m_points.find(arg) == m_points.end())
		{
			last_error = "point " + arg + " not found";
			throw std::exception{};
		}

		if (comp_less)
		{
			ip = m_points.at(arg);
		}
		else
		{
			++ip;
		}

		return result;
	}
	case je:
	{
		result += m_script.source[ip];

		std::string arg = getWord(m_script.source[ip], 1);
		if (arg.empty() || !getWord(m_script.source[ip], 2).empty())
		{
			last_error = m_script.source[ip] + ": syntax error";
			throw std::exception{};
		}

		if (m_points.find(arg) == m_points.end())
		{
			last_error = "point " + arg + " not found";
			throw std::exception{};
		}

		if (comp_even)
		{
			ip = m_points.at(arg);
		}
		else
		{
			++ip;
		}

		return result;
	}
	case jmp:
	{
		result += m_script.source[ip];

		std::string arg = getWord(m_script.source[ip], 1);
		if (arg.empty() || !getWord(m_script.source[ip], 2).empty())
		{
			last_error = m_script.source[ip] + ": syntax error";
			throw std::exception{};
		}

		if (m_points.find(arg) == m_points.end())
		{
			last_error = "point " + arg + " not found";
			throw std::exception{};
		}

		ip = m_points.at(arg);

		return result;
	}
	case print:
	{
		result += m_script.source[ip];

		variable var1;

		std::string arg = getWord(m_script.source[ip], 1);
		if (arg.empty() || !getWord(m_script.source[ip], 2).empty())
		{
			last_error = m_script.source[ip] + ": syntax error";
			throw std::exception{};
		}

		if (m_stack.size() >= MAX_STACK_SIZE)
		{
			last_error = "stack overflow";
			throw std::exception{};
		}

		if (arg == "none")
		{
			last_error = m_script.source[ip] + ": syntax error";
			throw std::exception{};
		}
		var1.reinit(get_var(arg));

		std::cout << arg << " = " << var1 << std::endl;

		++ip;
		return result;
	}
	case move_up:	// move entity up
	{
		bool hasWon = false;
		result += m_script.source[ip++];
		if (m_game.moveUp() == state::win)
		{
			hasWon = true;
		}
#ifdef DEMO_DISPLAY
		drawer::print(m_game.getState());
#endif		
#ifndef DEMO_DISPLAY
		drawer::write("score: " + std::to_string(m_game.getScore()), 0);
		drawer::draw(m_game.getState());
#endif
		updated_game = true;
		if (hasWon)
		{
			last_error = "victory. Your score: " + std::to_string(m_game.getScore());
			throw std::exception{};
		}
		return result;
	}
	case move_down:	// move entity down
	{
		bool hasWon = false;
		result += m_script.source[ip++];
		if (m_game.moveDown() == state::win)
		{
			hasWon = true;
		}
#ifdef DEMO_DISPLAY
		drawer::print(m_game.getState());
#endif
#ifndef DEMO_DISPLAY
		drawer::write("score: " + std::to_string(m_game.getScore()), 0);
		drawer::draw(m_game.getState());
#endif
		updated_game = true;
		if (hasWon)
		{
			last_error = "victory. Your score: " + std::to_string(m_game.getScore());
			throw std::exception{};
		}
		return result;
	}
	case move_left:	// move entity left
	{
		bool hasWon = false;
		result += m_script.source[ip++];
		if (m_game.moveLeft() == state::win)
		{
			hasWon = true;
		}
#ifdef DEMO_DISPLAY
		drawer::print(m_game.getState());
#endif
#ifndef DEMO_DISPLAY
		drawer::write("score: " + std::to_string(m_game.getScore()), 0);
		drawer::draw(m_game.getState());
#endif
		updated_game = true;
		if (hasWon)
		{
			last_error = "victory. Your score: " + std::to_string(m_game.getScore());
			throw std::exception{};
		}
		return result;
	}
	case move_right:// move entity right
	{
		bool hasWon = false;
		result += m_script.source[ip++];
		if (m_game.moveRight() == state::win)
		{
			hasWon = true;
		}
#ifdef DEMO_DISPLAY
		drawer::print(m_game.getState());
#endif
#ifndef DEMO_DISPLAY
		drawer::draw(m_game.getState());
		drawer::write("score: " + std::to_string(m_game.getScore()), 0);
#endif
		updated_game = true;
		if (hasWon)
		{
			last_error = "victory. Your score: " + std::to_string(m_game.getScore());
			throw std::exception{};
		}
		return result;
	}
	case map:		// get value at map pos
	{
		result += m_script.source[ip];

		std::string first_arg = getWord(m_script.source[ip], 1);
		std::string second_arg = getWord(m_script.source[ip], 2);

		if (!getWord(m_script.source[ip], 3).empty() || first_arg.empty() || second_arg.empty())
		{
			last_error = m_script.source[ip] + ": invalid syntax";
			throw std::exception{};
		}

		variable var1;
		variable var2;

		try
		{
			var1.reinit(get_var(first_arg));
			var2.reinit(get_var(second_arg));
		}
		catch (...)
		{
			last_error = m_script.source[ip] + ": unknown variable name";
			throw std::exception{};
		}

		m_stack.push(variable{ (int)m_game.getTile((int)var1, (int)var2) });
#ifdef RUNTIME_DEBUG
		std::cout << "new stack size = " << m_stack.size() << std::endl;
#endif

		++ip;
		return result;
	}
	case int_var:	// behavioar matches 'other', no break needed
	case float_var:	// behavioar matches 'other', no break needed
	case procedure:	// behavioar matches 'other', no break needed
	case point:		// behavioar matches 'other', no break needed
	case other:		// do nothing, just move on
		result += m_script.source[ip++];
		return result;
	}	
	last_error = "invalid syntax";
	throw std::exception{};
}

int runtime::find(const std::string& source, const std::string& substr)
{
	int result = -1;
	int i = 0;
	while (source[i])
	{
		if (source[i] == substr[0])
		{
			int j = i;
			int k = 0;
			while (substr[k])
			{
				if (!source[j])
				{
					break;
				}
				if (source[j] != substr[k])
				{
					break;
				}
				j++;
				k++;
			}
			if (!substr[k])
			{
				return i;
			}
		}
		i++;
	}
	return -1;
}

int runtime::getType(const std::string& str)
{
	if (str == "start")
	{
		return start;
	}
	if (str == "ret")
	{
		return ret;
	}
	if (str == "moveUp")
	{
		return move_up;
	}
	if (str == "moveDown")
	{
		return move_down;
	}
	if (str == "moveLeft")
	{
		return move_left;
	}
	if (str == "moveRight")
	{
		return move_right;
	}
	if (find(str, ":") != -1)
	{
		return point;
	}
	if (find(str, "print") == 0)
	{
		return print;
	}
	if (find(str, "proc") == 0)
	{
		return procedure;
	}
	if (find(str, "int") == 0)
	{
		return int_var;
	}
	if (find(str, "float") == 0)
	{
		return float_var;
	}
	if (find(str, "call") == 0)
	{
		return call;
	}
	if (find(str, "comp") == 0 || find(str, "cmp") == 0)
	{
		return comp;
	}
	if (find(str, "mul") == 0)
	{
		return _mul;
	}
	if (find(str, "sub") == 0)
	{
		return _sub;
	}
	if (find(str, "add") == 0)
	{
		return _add;
	}
	if (find(str, "div") == 0)
	{
		return _div;
	}
	if (find(str, "push") == 0)
	{
		return push;
	}
	if (find(str, "pop") == 0)
	{
		return pop;
	}
	if (find(str, "rem") == 0)
	{
		return _rem;
	}
	if (find(str, "ja") == 0)
	{
		return ja;
	}
	if (find(str, "jb") == 0)
	{
		return jb;
	}
	if (find(str, "je") == 0)
	{
		return je;
	}
	if (find(str, "jmp") == 0)
	{
		return jmp;
	}
	if (find(str, "map") == 0)
	{
		return map;
	}
	return other;
}

std::string runtime::getName(const std::string& str)
{
	std::string result{};
	result = getWord(str, 1);
	if (result.empty() || !getWord(str, 2).empty())
	{
		throw std::exception{};
	}
	return result;
}

std::string runtime::getWord(const std::string& str, size_t ind)
{
	size_t i = 0;
	size_t word_ind = 0;
	std::string result{};
	while (str[i])
	{
		while (str[i] == ' ')
		{
			++i;
		}
		while (str[i] != ' ' && str[i] != '\0')
		{
			if (word_ind == ind)
			{
				if (str[i] != ':')
				{
					result += str[i];
				}
			}
			++i;
		}
		if (word_ind == ind)
		{
			return result;
		}
		++word_ind;
	}

	return std::string{};
}

variable runtime::get_var(const std::string& str)
{
	variable result;
	if (get_arg_type(str) == raw_int)
	{
		result.reinit(std::stoi(str));
	}
	else if (get_arg_type(str) == raw_float)
	{
		result.reinit(std::stof(str));
	}
	else
	{
		if (str == "player_x")
		{
			result.reinit(variable{ m_game.playerX() });
		}
		else if (str == "player_y")
		{
			result.reinit(variable{ m_game.playerY() });
		}
		else if (str == "field_height")
		{
			result.reinit(variable{ m_game.height() });
		}
		else if (str == "field_width")
		{
			result.reinit(variable{ m_game.width() });
		}
		else if (str == "none")
		{
			result.reinit(variable{});
		}
		else if (str == "wall")
		{
			result.reinit(variable{ (int)tile::wall });
		}
		else if (str == "nothing")
		{
			result.reinit(variable{ (int)tile::nothing });
		}
		else if (str == "player")
		{
			result.reinit(variable{ (int)tile::player });
		}
		else if (str == "food")
		{
			result.reinit(variable{ (int)tile::food });
		}
		else if (str == "destination")
		{
			result.reinit(variable{ (int)tile::destination });
		}
		else if (str == "stack")
		{
			if (m_stack.empty())
			{
				last_error = "stack is empty";
				throw std::exception{};
			}
			result.reinit(m_stack.top());
		}
		else if (str == "stackp")
		{
			if (m_stack.empty())
			{
				last_error = "stack is empty";
				throw std::exception{};
			}
			result.reinit(m_stack.top());
			m_stack.pop();
#ifdef RUNTIME_DEBUG
			std::cout << "new stack size = " << m_stack.size() << std::endl;
#endif
		}
		else if (m_global.find(str) == m_global.end())
		{
			last_error = "";
			throw std::exception{};
		}
		else
		{
			result.reinit(m_global.at(str));
		}
	}
	return result;
}

arg_type get_arg_type(const std::string& str)
{

	bool is_float = true;
	bool is_int = true;

	for (auto i : str)
	{
		if (i >= '0' && i <= '9' || i == '-')
		{
			continue;
		}
		is_int = false;
		if (i == '.')
		{
			continue;
		}
		is_float = false;
	}

	if (is_int)
	{
		return raw_int;
	}
	if (is_float)
	{
		return raw_float;
	}
	return var;
}