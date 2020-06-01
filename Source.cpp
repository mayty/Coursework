#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "runtime.h"
#include "drawer.h"
#include <Windows.h>

int main(int argC, char** argV)
{
	std::string filename;
	int i = 1;
	if (argC == 1)
	{
		return 1;
	}
	else
	{
		filename = argV[1];
	}
	try
	{
		runtime sandbox{ filename };
		while (true)
		{
			drawer::write(sandbox.execute(), i++, 40);
			if (i >= 24)
			{
				i = 1;
			}
		}
	}
	catch (...)
	{
		drawer::write(last_error, 24);
	}
	system("pause");
	return 0;
}