#ifndef OPEN_FILE_HPP
#define OPEN_FILE_HPP


#include <iostream>
#include <fstream>

class OpenFile
{
public:

	void Open(const std::string& filename )
	{
		char buff[50];
		std::ifstream F(filename.c_str());
		F.getline(buff, 50);
		F.close();
		std::cout<<buff<<std::endl;
	}
	
	
private:
	char buffer[100];
};



#endif