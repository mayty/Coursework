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
		source.push_back(buf);
	}

	input.close();
}
