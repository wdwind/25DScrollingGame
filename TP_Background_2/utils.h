#include <iostream>

void printVector(float* v, int num){
	for (int i = 0; i < num; i++)
	{
		std::cout << v[i] << " ";
	}
	std::cout << std::endl;
}

