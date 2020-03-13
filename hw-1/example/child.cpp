#include <unistd.h>
#include <cstdio>
#include <string.h>
#include <cctype>


static char *upper(char *buffer)
{
	char *current = buffer;
	while (*current)
	{
		*current = toupper(*current);
		current++;
	}
	return buffer;
}

int main()
{
//   printf("Hello, i'm main_process.out!\n");
  char buffer[10];
  while (!feof(stdin))
  {
    size_t bytes = fread(buffer, 1, sizeof(buffer) - 1, stdin);
	buffer[bytes] = '\0';
	fwrite(upper(buffer), 1, bytes, stdout);
  }
  return 0;
}
