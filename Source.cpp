#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "script.h"
#include "runtime.h"

int main(int argC, char** argV)
{
	//script m_script{ "input.txt" };
	runtime sandbox{ "input.txt" };
	return 0;
}