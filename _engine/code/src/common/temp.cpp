#include <SDL.h>
#include <cstdio>
#include <cstdlib>
#include "bootstrapper.hpp"

struct owner1 { virtual ~owner1() {}  };
struct owner1_child : public owner1 { };
struct owner2 { };

struct provider_base { };

REGISTER_PROVIDER_BASE_TYPE(owner1, provider_base)
REGISTER_PROVIDER_BASE_TYPE(owner2, provider_base)

struct provider1a : public provider_base { };
struct provider1b : public provider_base { };
struct provider2 : public provider_base { };


int main(int arg, char * argv[])
{
	engine::bootstrapper_t< 

		engine::register_providers_for<owner1, provider1a, provider1b>,
		engine::register_providers_for<owner2, provider2>,
		engine::register_as<owner1_child, owner1>
	
	> bootstrapper;

	auto component = bootstrapper.construct_component<owner1>();

	owner1_child * ptr = dynamic_cast<owner1_child*>(component.get());

	return 0;
}