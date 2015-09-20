#include <stdio.h>
#include <iostream>
#include <cstring>

enum states {GAME, CONSOLE } st = CONSOLE;

int main()
{
	char buff[256];
	if(st == CONSOLE)
	{
		gets(buff);
		printf("buff is %s\n", buff);

		FILE *f = fopen("test.txt", "w");
		fputs(buff, f);
		fclose(f);
		f = fopen("test.txt", "r");
		int g = 0;
		while(1){
		int res = fscanf(f, "%s", buff);
		if(res == EOF) break;

		if(strcmp(buff, "god") == 0)
		{
			fscanf(f, "%i\n", &g);
		}
		}
		fclose(f);
		printf("god mode is %i\n", g);
	}

	return 0;
}
