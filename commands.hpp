#include <stdio.h>
#include <iostream>
#include <cstring>

//enum states {GAME, CONSOLE } st = CONSOLE;

void console_update()
{
	char buff[256];
	float nil = 0;
	//static bool run = true;
	if(st == CONSOLE)
	{
	while(c_run){
		gets(buff);
		//printf("buff is %s\n", buff);

		FILE *f = fopen("test.txt", "w+");
		//fputs(buff, f);
		//fclose(f);
		//f = fopen("test.txt", "r");
		
		fprintf(f, "%s", buff);
		rewind(f);
		
		while(1){
		int res = fscanf(f, "%s", buff);
		if(res == EOF) break;

		if(strcmp(buff, "tgm") == 0)
		{
			fscanf(f, "%i\n", &tgm);
		}
		else if(strcmp(buff, "tcl") == 0)
		{
			fscanf(f, "%i\n", &tcl);
		}
		else if(strcmp(buff, "end") == 0)
		{
			printf("___console_exit\n");
			st = GAME;
			c_run = false;
		}
		else if(strcmp(buff, "pos") == 0)
		{
			fscanf(f, "%f %f %f\n", &position.x, &position.y, &position.z);
		}
		else if(strcmp(buff, "pos --help") == 0)
		{
			printf("to change a position, write: pos x y z and hit ENTER\n");
		}
		else if(strcmp(buff, "angle") == 0)
		{
			fscanf(f, "%f %f\n", &verticalAngle, &horizontalAngle);
		}
		}
		fclose(f);
		remove("test.txt");

		//printf("god mode is : %i\n", tgm);
		//printf("tcl is : %i\n", tcl);
	}
	}

	//return 0;
}
