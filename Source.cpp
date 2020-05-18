#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "script.h"
#include "runtime.h"
#include "drawer.h"
#include <Windows.h>

int main(int argC, char** argV)
{
	std::string filename;
	int i = 0;
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
			if (i >= 25)
			{
				i = 0;
			}
		}
	}
	catch (...)
	{
		drawer::write(last_error, 25);
	}
	system("pause");
	return 0;
}