#include <SDL.h>
#include <cstdio>
#include <cstdlib>
#include "bootstrapper.hpp"

int main(int arg, char * argv[])
{
	engine::bootstrapper_t bootstrapper;

	bootstrapper.	register_providers_for<int, double, float>().
					register_providers_for<char, double, float>();




	return 0;
}