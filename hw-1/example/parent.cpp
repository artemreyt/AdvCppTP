#include <iostream>
#include "process.hpp"
#include <string>
#include <string.h>

static const size_t str_size = 1000;

int main()
{
	// std::string str(str_size, '\0');
	
		Process proc("./main_process.out -d skjwen wekj");
		while (!std::cin.eof())
		{
			char buf[200];
			// std::getline(std::cin, buf);
			gets(buf);
			proc.write(buf, strlen(buf));
			ssize_t bytes;
			bytes = proc.read(buf, sizeof(buf));
			fwrite(buf, 1, bytes, stdout);
		}
	// proc.closeStdin();
	// char buf[200];
	// ssize_t bytes;
	// while ((bytes = proc.read(buf, sizeof(buf))) > 0)
	// 	fwrite(buf, 1, bytes, stdout);
	
	return 0;
}
