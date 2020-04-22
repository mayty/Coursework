#include "runtime.h"
#include <iostream>

enum types {
	procedure, var, start, call, other
};


runtime::runtime(const std::string& filename)
	:m_script(filename)
{
	bool found_start = false;
	for (int i = 0; i < m_script.source.size(); ++i)
	{
		switch (getType(m_script.source[i]))
		{
		case procedure:
			try
			{
				functions.emplace(getName(m_script.source[i]), i);
			}
			catch (std::exception e)
			{
				std::cout << "invalid syntax" << std::endl;
			}
			break;
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
		throw std::exception{};
	}
}

std::string runtime::execute()
{
	if (ip == -1)
	{
		throw std::exception{};
	}
	std::string result{};
	result = std::to_string(ip) + ": ";
	if (m_script.source[ip] == "ret")
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
	if (getType(m_script.source[ip]) == call)
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
			throw std::exception{};
		}
	}
	result += m_script.source[ip++];
	return result;
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
	if (find(str, "proc") == 0)
	{
		return procedure;
	}
	if (find(str, "val") == 0)
	{
		return var;
	}
	if (find(str, "call") == 0)
	{
		return call;
	}
	return other;
}

std::string runtime::getName(const std::string& str)
{
	std::string result{};
	int i = 0;
	while (str[i] && str[i] != ' ')
	{
		i++;
	}
	while (str[i] && str[i] == ' ')
	{
		i++;
	}
	while (str[i])
	{
		if (str[i] == ' ')
		{
			throw std::exception{};
		}
		result += str[i++];
	}
	if (result.empty())
	{
		throw std::exception{};
	}
	return result;
}
