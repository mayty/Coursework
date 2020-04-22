#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "script.h"
#include "runtime.h"

int main(int argC, char** argV)
{
	runtime sandbox{ "input.txt" };
	while (true)
	{
		try
		{
			std::cout << sandbox.execute() << std::endl;
			//system("pause");
		}
		catch (std::exception e)
		{
			std::cout << "end" << std::endl;
			break;
		}
	}
	return 0;
}