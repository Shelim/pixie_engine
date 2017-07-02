#include <SDL.h>
#include <cstdio>
#include <cstdlib>
#include "bootstrapper.hpp"

struct owner1 { virtual ~owner1() {}  };
struct owner1_child : public owner1 { };
struct owner2 { };

struct provider1a : public engine::provider_base_t<owner1> { };
struct provider1b : public engine::provider_base_t<owner1> { };
struct provider2 : public engine::provider_base_t<owner2> { };

int main(int arg, char * argv[])
{
	engine::bootstrapper_t< engine::register_providers_for<owner1, provider1a, provider1b>,
							engine::register_providers_for<owner2, provider2>,
							engine::register_as<owner1_child, owner1>
	> bootstrapper;

	auto component = bootstrapper.construct_component<owner1>();

	owner1_child * ptr = dynamic_cast<owner1_child*>(component.get());

	return 0;
}