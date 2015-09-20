#include <stdio.h>
#include <iostream>
#include <cstring>

enum states {GAME, CONSOLE } st = CONSOLE;

int main()
{
	char buff[256];
	bool run = true;
	if(st == CONSOLE)
	{
	while(run){
		gets(buff);
		//printf("buff is %s\n", buff);

		FILE *f = fopen("test.txt", "w");
		fputs(buff, f);
		fclose(f);
		f = fopen("test.txt", "r");
		int g = 0, tcl = 0;
		while(1){
		int res = fscanf(f, "%s", buff);
		if(res == EOF) break;

		if(strcmp(buff, "god") == 0)
		{
			fscanf(f, "%i\n", &g);
		}
		else if(strcmp(buff, "tcl") == 0)
		{
			fscanf(f, "%i\n", &tcl);
		}
		else if(strcmp(buff, "end") == 0)
		{
			printf("___console_exit\n");
			run = false;
		}

		}
		fclose(f);
		printf("god mode is : %i\n", g);
		printf("tcl is : %i\n", tcl);
	}
	}

	return 0;
}
