#include "script.h"
#include <fstream>

script::script(std::string filename)
{
	std::ifstream input{ filename };
	if (!input.is_open())
		throw std::exception{};

	while (!input.eof())
	{
		std::string buf;
		std::getline(input, buf);
		if (buf.empty())
			continue;
		std::string str;
		bool write_space = true;
		int i = -1;
		while (buf[++i])
		{
			if (buf[i] >= 'a' && buf[i] <= 'z')
			{
				str += buf[i];
				write_space = true;
				continue;
			}
			if (buf[i] >= 'A' && buf[i] <= 'Z')
			{
				str += buf[i];
				write_space = true;
				continue;
			}
			if (buf[i] == '-' || buf[i] == '+' || buf[i] == '/' || buf[i] == '*' || buf[i] == '=')
			{
				str += buf[i];
				write_space = true;
				continue;
			}
			if (buf[i] == ' ')
			{
				if (write_space)
				{
					str += buf[i];
				}
				write_space = false;
				continue;
			}
		}
		if (!str.empty())
		{
			source.push_back(str);
		}
	}

	input.close();
}
