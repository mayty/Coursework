#include "runtime.h"

runtime::runtime(const std::string& filename)
	:m_script(filename)
{
	for (int i = 0; i < m_script.source.size(); ++i)
	{
		switch (m_script.source[i].find("proc"))
		{
		case std::string::npos:
			break;
		default:
			functions.emplace(m_script.source[i].c_str() + 5, i);
		}
	}
}

std::string runtime::execute()
{
	return std::string();
}

int runtime::find(const std::string& source, const std::string& str)
{
	size_t j = 0;
	bool any = false;
	for (size_t i = 0; i < source.size(); ++i)
	{
		switch (str[j])
		{
		case '*':
			any = true;
			++j;
			break;
		default:
			any = false;
		}
		for (i; i < source.size(); ++i)
		{
			if (source[i] != str[j])
			{
				if (!any)
				{
					return -1;
				}
			}
		}
	}
	return 0;
}
