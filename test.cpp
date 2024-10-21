#include <iostream> 
#include <string>

#include "src/include/Maze.hpp"

template<typename T>
void print(T value);

void line();

int main ()
{
    bool test = 0b00000000;
    Maze<> m(4, 4);
    m[0].setDirection(L_SIDE, UP);
    m[0].setDirection(R_SIDE, UP);

    std::cout << L_SIDE + UP;
    line();

    print(m[0]);
    line();
    
    std::cout << m[0].str() << std::endl;
    line();

    std::cout << m[0].val(R_SIDE) << std::endl;
    line();

    std::cout << m[0].val(L_SIDE) << std::endl;
    line();

    return 0;
}

template<typename T>
void print(T value){
	unsigned numbytes=sizeof(T),byte,bit;
	char* pval=(char*)&value;
	for(byte=0;byte<numbytes;++byte){
		for(bit=0;bit<8;++bit){
			putchar(((pval[byte]>>bit)&1)+'0');}
		if(byte!=numbytes-1){
			putchar(' ');}}}

void line ()
{
    std::cout << std::endl << "----------" << std::endl; 
}