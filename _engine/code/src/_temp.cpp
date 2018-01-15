#if 0

#include <exception>
#include <SDL.h>
#include <cstdio>
#include <cstdlib>
#include <thread>
#include <chrono>
#include "global/bootstrapper.hpp"
#include "utility/concurrention/signal.hpp"
#include "core/messenger/messenger.hpp"
#include "core/console/msg/logger.hpp"
#include "core/console/msg/meta.hpp"
#include "core/console/filter.hpp"


int main(int arg, char * argv[])
{
	BEGIN_BOOTSTRAPPER(bootstrapper)

		APP(game)

		ENABLE_COMPONENT(config)
		ENABLE_COMPONENT(config_monitor)
		ENABLE_COMPONENT(config_storage)
		ENABLE_COMPONENT(filesystem)
		ENABLE_COMPONENT(logger)
		ENABLE_COMPONENT(terminal)

		USE_PROVIDER_FOR(config, config_storage)
		USE_PROVIDERS_FOR(config_changed, messenger, logger)
		USE_PROVIDER_FOR(config_storage, windows_registry)
		USE_PROVIDER_FOR(filesystem, windows)
		USE_PROVIDERS_FOR(logger, console)
		USE_PROVIDER_FOR(terminal, windows)
		
#if PIXIE_IS_DEBUG_BUILD
		USE_SETTINGS(config_t, debug)
#else
		USE_SETTINGS(config_t, release)
#endif
		USE_SETTINGS(config_storage_t, normal)

	END_BOOTSTRAPPER()

	std::shared_ptr<engine::config_t> config = GET_COMPONENT(bootstrapper, config);
	std::shared_ptr<engine::config_storage_t> config_storage = GET_COMPONENT(bootstrapper, config_storage);
	std::shared_ptr<engine::filesystem_t> filesystem = GET_COMPONENT(bootstrapper, filesystem);
	std::shared_ptr<engine::terminal_t> terminal = GET_COMPONENT(bootstrapper, terminal);
	std::shared_ptr<engine::logger_t> logger = GET_COMPONENT(bootstrapper, logger);
	std::shared_ptr<engine::messenger_console_t> console = GET_COMPONENT(bootstrapper, messenger_console);
	std::shared_ptr<engine::monitors_t> monitors = GET_COMPONENT(bootstrapper, monitors);

	logger->log_msg(core, "Test"_u);
	logger->log_msg(core, "Hello world!"_u);
	logger->log_msg(core, "Hello world number #1#!"_u, 2);

/*
	engine::ustring_t filter_src = "kind is meta or kond and kind nor blah is success or (type is loger and (not any or source is not core or source is not meta))"_u;
	engine::console_filter_string_t filter(filter_src);

	auto term_filter = terminal->open("Console"_u, engine::terminal_t::color_t::teal_dark);
	term_filter->write(filter_src, engine::terminal_t::color_t::pink_light);
	term_filter->write_new_line();

	for(auto & token : filter)
	{
		engine::ustring_t str = engine::format_string("Token = #1#"_u, token->get_id());
		printf("%s\n", str.get_cstring());
		term_filter->write(str, engine::terminal_t::color_t::white);
		term_filter->write_new_line();

		if(token->get_id() == engine::parser::token_console_filter_matcher_constant_t::id)
			str = engine::format_string(" -> is equal = #1#"_u, static_cast<const engine::parser::token_console_filter_matcher_constant_t*>(token.get())->get_value());
		else if(token->get_id() == engine::parser::token_console_filter_unary_operator_t::id)
			str = engine::format_string(" -> is equal = #1#"_u, static_cast<const engine::parser::token_console_filter_unary_operator_t*>(token.get())->get_value());
		else if(token->get_id() == engine::parser::token_console_filter_binary_operator_t::id)
			str = engine::format_string(" -> is equal = #1#"_u, static_cast<const engine::parser::token_console_filter_binary_operator_t*>(token.get())->get_value());
		else if(token->get_id() == engine::parser::token_console_filter_parenthesis_t::id)
		{
			if(static_cast<const engine::parser::token_console_filter_parenthesis_t*>(token.get())->get_value() == engine::parser::token_console_filter_parenthesis_t::value_t::open)
				str = engine::format_string(" -> is opening parenthesis"_u);
			else
				str = engine::format_string(" -> is closing parenthesis"_u);
		}
		else if(token->get_id() == engine::parser::token_console_filter_comparision_t::id)
			str = engine::format_string(" -> is equal = #1#"_u, static_cast<const engine::parser::token_console_filter_comparision_t*>(token.get())->get_value());
		else if(token->get_id() == engine::parser::token_console_filter_object_t::id)
			str = engine::format_string(" -> is equal = #1#"_u, static_cast<const engine::parser::token_console_filter_object_t*>(token.get())->get_value());
		else if(token->get_id() == engine::parser::token_console_filter_value_t::id)
		{
			auto obj = static_cast<const engine::parser::token_console_filter_value_t*>(token.get());
			engine::ustring_t all = ""_u;
			if(obj->is_available<engine::messenger::msg_console_t::actual_t::kind_t>())
			{
				all.append("kind"_u);
				str = engine::format_string(" -> is equal = #1# (#2#)"_u, obj->get<engine::messenger::msg_console_t::actual_t::kind_t>(), all);
			}
			if(obj->is_available<engine::messenger::msg_console_t::actual_t::source_t>())
			{
				if(all.len() > 0) all.append(", "_u);
				all.append("source"_u);
				str = engine::format_string(" -> is equal = #1# (#2#)"_u, obj->get<engine::messenger::msg_console_t::actual_t::source_t>(), all);
			}
			if(obj->is_available<engine::messenger::msg_console_t::actual_t::type_t>())
			{
				if(all.len() > 0) all.append(", "_u);
				all.append("type"_u);
				str = engine::format_string(" -> is equal = #1# (#2#)"_u, obj->get<engine::messenger::msg_console_t::actual_t::type_t>(), all);
			}
		}
		else if(token->get_id() == engine::parser::token_console_filter_unknown_t::id)
		{
			if(static_cast<const engine::parser::token_console_filter_unknown_t*>(token.get())->get_suggestions().size() > 0)
				str = engine::format_string(" -> is unregognized '#1#' (Did you meant: #2#?)"_u, static_cast<const engine::parser::token_console_filter_unknown_t*>(token.get())->get_value(), static_cast<const engine::parser::token_console_filter_unknown_t*>(token.get())->get_suggestions()[0], filter_src.substr(std::max(0, static_cast<int>(token->get_pos())) - 10, 20));
			else
				str = engine::format_string(" -> is unregognized '#1#'"_u, static_cast<const engine::parser::token_console_filter_unknown_t*>(token.get())->get_value(), filter_src.substr(std::max(0, static_cast<int>(token->get_pos())) - 10, 20));
		}
		else if(token->get_id() == engine::parser::token_text_t::id)
			str = engine::format_string(" -> is unexpected '#1#'"_u, static_cast<const engine::parser::token_text_t*>(token.get())->get_buffer().trim(), filter_src.substr(std::max(0, static_cast<int>(token->get_pos())) - 10, 20));

		printf("%s\n", str.get_cstring());
		term_filter->write(str, engine::terminal_t::color_t::yellow_light);
		term_filter->write_new_line();
	}
	*/

	auto term = terminal->open("Errors"_u, engine::terminal_t::color_t::red_dark);
	term->write("Hello world!"_u, engine::terminal_t::color_t::white, engine::terminal_t::color_t::green_dark);
	term->write_new_line();
	term->write("Test!"_u, engine::terminal_t::color_t::black, engine::terminal_t::color_t::red_light);

	auto term2 = terminal->open("Messages"_u, engine::terminal_t::color_t::blue_dark);
	term2->write("Hello world!"_u, engine::terminal_t::color_t::white, engine::terminal_t::color_t::green_dark);
	term2->write_new_line();
	term2->write("Test!"_u, engine::terminal_t::color_t::black, engine::terminal_t::color_t::red_light);

	std::this_thread::sleep_for(std::chrono::seconds(2));
	term.reset();
	term2->write_new_line();
	term2->write("After being dead!"_u, engine::terminal_t::color_t::black, engine::terminal_t::color_t::pink_light);

	engine::paths_t files = filesystem->iterate_files_in_directory("C:\\!Lost Empire");
	engine::paths_t dirs = filesystem->iterate_directories("C:\\!Lost Empire");
	engine::paths_t subfiles = filesystem->iterate_files_in_subdirectories("C:\\!Lost Empire", "*.cs"_u);
	engine::paths_t subdirs = filesystem->iterate_subdirectories("C:\\!Lost Empire");

	printf("Start1\n");

	auto term_logger = terminal->open("Logger"_u, engine::terminal_t::color_t::green_dark);

	auto instance1 = console->register_callback([term_logger](engine::messenger::msg_console_t* msg)
	{
		switch(msg->get()->get_type())
		{
			case engine::messenger::msg_console_t::actual_t::type_t::meta:
			{
				engine::console::meta_item_t * meta = static_cast<engine::console::meta_item_t *>(msg->get());
				if(meta->get_meta_type() == engine::console::meta_item_t::meta_type_t::output_start)
					term_logger->write("OUTPUT STARTED!"_u, engine::terminal_t::color_t::white);
				else
					term_logger->write("OUTPUT ENDED!"_u, engine::terminal_t::color_t::white);
				
				term_logger->write_new_line();
			}
			break;
			case engine::messenger::msg_console_t::actual_t::type_t::logger:
			{
				engine::console::logger_item_t * item = static_cast<engine::console::logger_item_t *>(msg->get());
				const engine::ustring_t msg_text = item->get_message();
				term_logger->write(msg_text, engine::terminal_t::color_t::yellow_light);
				if(msg->get()->get_kind() == engine::messenger::msg_console_t::actual_t::kind_t::pending) term_logger->write("..."_u, engine::terminal_t::color_t::yellow_dark);
				if(msg->get()->get_kind() == engine::messenger::msg_console_t::actual_t::kind_t::success) term_logger->write("... DONE!"_u, engine::terminal_t::color_t::green_light);
				term_logger->write_new_line();
			}
			break;
		}
	}, engine::messenger::history_t::dump_if_available);
/*
	for(int i = 0; i < 100000; i++)
	{
		logger->log_msg(core, "Test #1#"_u, i);
	}
*/
	for(int i = 0; i < 100; i++)
	{
		term2->write("Async write!"_u, engine::terminal_t::color_t::black, engine::terminal_t::color_t::pink_light);
		term2->write_new_line();
	}

	config->set_local_test_setting(true);
	config->set_local_test_setting(false);
	config->set_local_test_setting(false);
	config->set_local_test_setting(true);

	logger->log_msg(core, "Sometime after..."_u);

	std::this_thread::sleep_for(std::chrono::seconds(15));

	engine::signaler_t sign;

	std::thread thread1([&sign](){printf("Starting main... %d\n", std::time(nullptr)); std::this_thread::sleep_for(std::chrono::seconds(3));sign.signal_ready();});
	std::thread thread2([&sign](){printf("Starting 1... %d\n", std::time(nullptr)); sign.get_waiter().wait_for_signal(); printf("Completed 1! %d\n", std::time(nullptr));});
	std::thread thread3([&sign](){printf("Starting 2... %d\n", std::time(nullptr)); sign.get_waiter().wait_for_signal(); printf("Completed 2! %d\n", std::time(nullptr));});
	std::thread thread4([&sign](){printf("Starting 3... %d\n", std::time(nullptr)); sign.get_waiter().wait_for_signal(); printf("Completed 3! %d\n", std::time(nullptr));});

	std::this_thread::sleep_for(std::chrono::seconds(7));
	std::thread thread5([&sign](){printf("Starting 4... %d\n", std::time(nullptr)); sign.get_waiter().wait_for_signal(); printf("Completed 4! %d\n", std::time(nullptr));});

	thread1.join();
	thread2.join();
	thread3.join();
	thread4.join();
	thread5.join();

	return 0;
}

#endif

#if 0
#include <SDL.h>
#include <array>
#include <memory>
#include "fruit/fruit.h"

class interface1
{

	public:
		virtual ~interface1(){}
		virtual int print_address() = 0;

};

class impl1 : public interface1
{
	
	public:

		impl1()
		{

		}
	
		int print_address() final
		{
			return (int)(void*)this;
		}

};


class provider_base_t
{

	public:

		virtual ~provider_base_t() { }
		virtual int name() const = 0;

};

class provider1 : public provider_base_t
{
	public:

		provider1(){}
		int name() const final { return 1;}
};

class provider2 : public provider_base_t
{
	public:

		provider2(){}
		int name() const final { return 2;}
};


class provider3 : public provider_base_t
{
	public:

		provider3(){}
		int name() const final { return 3;}
};


class interface2
{
	public:
		virtual ~interface2(){}
};

class impl2 : public interface2
{
	public:
		impl2(std::shared_ptr<interface1> in)
		{
			printf("Impl2 instanced: %d\n", in->print_address());
		}
};

class impl2_dummy : public interface2
{
	public:
		impl2_dummy()
		{
			printf("dumnmy impl");
		}
};


class igame
{

	public:

		virtual ~igame() { }

};

template<class owner_t> struct provider_manifest_t;

#define REGISTER_PROVIDER_BASE_TYPE(owner, provider_base) template<> struct provider_manifest_t<owner> { typedef provider_base provider_t; };
		

template<class owner_t> class holder_t
{

public:

	virtual ~holder_t()
	{

	}
	
	virtual typename provider_manifest_t<owner_t>::provider_t * get_provider(std::size_t id = 0) = 0;
	virtual std::size_t get_providers_count() const = 0;

};

template<class owner_t, class ... providers_t> class holder_implementation_t : public holder_t<owner_t>
{

public:

	holder_implementation_t(std::shared_ptr<providers_t>... providers) : providers{ {std::move(providers)...} }
	{

	}
	
	typename provider_manifest_t<owner_t>::provider_t * get_provider(std::size_t id = 0) final
	{
		return providers[id].get();
	}

	std::size_t get_providers_count() const final
	{
		return sizeof...(providers_t);
	}

private:

	std::array<std::shared_ptr<typename provider_manifest_t<owner_t>::provider_t >, sizeof...(providers_t)> providers;
};

REGISTER_PROVIDER_BASE_TYPE(igame, provider_base_t)

class interface3
{

	public:
		virtual ~interface3(){}
		virtual int print_address() = 0;

};

class impl3 : public interface3
{
	
	public:

		impl3(std::shared_ptr<interface1> interface1, int a)
		{

		}
	
		int print_address() final
		{
			return (int)(void*)this;
		}

		using factory = std::function<std::unique_ptr<impl3>(int)>;
		using factory_object = std::unique_ptr<impl3>(std::shared_ptr<interface1>, fruit::Assisted<int>);
		static auto factory_func() {
			return [](std::shared_ptr<interface1> arg1, int arg2){ return std::make_unique<impl3>(arg1, arg2); };
		}

};

class game : public igame
{
	public:

		game(std::shared_ptr<impl2> impl2, std::shared_ptr<holder_t<igame>> provider_base, impl3::factory factory)
		{
			printf("Provider list:\n");
			for(std::size_t i = 0; i < provider_base->get_providers_count(); i++)
			{
				printf(" + provider %d\n", provider_base->get_provider(i)->name());
			}

			printf("Creating some objects...\n");
			auto item1 = factory(5);
			auto item2 = factory(5);
			auto item3 = factory(5);
			printf(" + Obj = %d\n", item1->print_address());
			printf(" + Obj = %d\n", item2->print_address());
			printf(" + Obj = %d\n", item3->print_address());
		}

	private:
		
};

fruit::Component<igame, interface2> getFinal() {
    return fruit::createComponent()
	    .bind<igame, game>()
        .bind<interface1, impl1>()
        .bind<interface2, impl2>()
        .bind<holder_t<igame>, holder_implementation_t<igame, provider1, provider3>>()
		.registerFactory<impl3::factory_object>(impl3::factory_func());
}

int main(int arg, char * argv[])
{
	std::unique_ptr<fruit::Injector<igame, interface2>> injector1 = std::make_unique<fruit::Injector<igame, interface2>>(getFinal);
	std::unique_ptr<fruit::Injector<igame, interface2>> injector2 = std::make_unique<fruit::Injector<igame, interface2>>(getFinal);

	printf("First test!\n");
	std::shared_ptr<igame> root1 = injector1->get<std::shared_ptr<igame>>();
	std::shared_ptr<igame> root2 = injector1->get<std::shared_ptr<igame>>();
	std::shared_ptr<igame> root3 = injector1->get<std::shared_ptr<igame>>();
	std::shared_ptr<interface2> int1 = injector1->get<std::shared_ptr<interface2>>();
	std::shared_ptr<interface2> int2 = injector1->get<std::shared_ptr<interface2>>();
	printf("First test B!\n");
	injector1 = std::make_unique<fruit::Injector<igame, interface2>>(getFinal);
	std::shared_ptr<igame> root3a = injector1->get<std::shared_ptr<igame>>();
	printf("Second test!\n");
	std::shared_ptr<igame> root4 = injector2->get<std::shared_ptr<igame>>();
	std::shared_ptr<igame> root5 = injector2->get<std::shared_ptr<igame>>();
	printf("End-of-tests!\n");

	return 0;
}

#endif

#if 0
#include <SDL.h>
#include <iostream>
#include <memory>
#include <cstdio>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <unordered_map>
#include <functional>
#include <typeindex>
#include <typeinfo>
#include <map>
#include <utility>
#include "boost/di.hpp"

struct ifactory_base
{
	virtual ~ifactory_base() noexcept = default;
};

template <class T, class... TArgs>
struct ifactory : ifactory_base {
	virtual ~ifactory() noexcept = default;
	virtual std::unique_ptr<T> create(TArgs&&...) const = 0;
};

template <class, class, class>
struct factory_impl;

template <class TInjector, class T, class I>
struct factory_impl<TInjector, T, ifactory<I>> : ifactory<I> {
	explicit factory_impl(const TInjector& injector) : injector_((TInjector&)injector) {}

	std::unique_ptr<I> create() const override {
		
		return injector_.template create<std::unique_ptr<T>>();
	}

private:
	TInjector& injector_;
};

template <class T>
struct factory {
  template <class TInjector, class TDependency>
  auto operator()(const TInjector& injector, const TDependency&) noexcept {
	auto iter = data_map.find(std::type_index(typeid(typename factory<T>::key_t<TInjector, TDependency>)));
	std::shared_ptr<ifactory_base> ret;
	if(iter == data_map.end())
	{
    	ret = std::make_shared<factory_impl<TInjector, T, typename TDependency::expected>>(injector);
		data_map.insert(std::make_pair(std::type_index(typeid(typename factory<T>::key_t<TInjector, TDependency>)), ret));
	}
	else
		ret = iter->second;
	return std::static_pointer_cast<factory_impl<TInjector, T, typename TDependency::expected>>(ret);
  }

private:
	
	template <class T1, class T2> class key_t {};

	std::unordered_map<std::type_index, std::shared_ptr<ifactory_base> > data_map;
	
};

class scoped_scope {
public:
	template <class, class T>
	class scope {
	public:
		template <class T_>
		using is_referable = typename boost::di::wrappers::shared<scoped_scope, T>::template is_referable<T_>;

		template <class, class, class TProvider, class T_ = boost::di::aux::decay_t<decltype(boost::di::aux::declval<TProvider>().get())>>
		static decltype(boost::di::wrappers::shared<scoped_scope, T_>{
			std::shared_ptr<T_>{std::shared_ptr<T_>{boost::di::aux::declval<TProvider>().get()}}})
			try_create(const TProvider &);

			template <class T_, class, class TProvider>
			auto create(const TProvider &provider) {
				return create_impl<boost::di::aux::decay_t<decltype(provider.get())>>(provider);
			}

			scope() = default;
			scope(scope &&other) noexcept : object_(other.object_) { other.object_ = nullptr; }
			~scope() noexcept { delete object_; }

	private:
		template <class, class TProvider>
		auto create_impl(const TProvider &provider) {
			if (!object_) {
				object_ = new std::shared_ptr<T>{ provider.get() };
			}
			return boost::di::wrappers::shared<scoped_scope, T, std::shared_ptr<T> &>{*object_};
		}

		std::shared_ptr<T> *object_ = nullptr;
	};
};

static constexpr scoped_scope scoped{};

class interface_test
{
public:

    virtual int print_address() = 0;
};

class real_test : public interface_test
{
public:

    int print_address() final
    {
        return (int)(void*)this;
    }
};

class ifactorable
{

public:

    virtual int get_address() = 0;

};

class factorable : public ifactorable
{
public:

    factorable(std::shared_ptr<interface_test> test) : test(test)
    {

    }

    int get_address() final
    {
        return test->print_address();
    }

private:

    std::shared_ptr<interface_test> test;
};

int main(int arg, char * argv[]) 
{
    // clang-format off
    auto injector1 = boost::di::make_injector(boost::di::bind<interface_test>().in(scoped).to<real_test>()[boost::di::override]
        , boost::di::bind<ifactorable>().in(scoped).to<factorable>()
        , boost::di::bind<ifactory<factorable>>().to(factory<factorable>{})
    );

    auto injector2 = boost::di::make_injector(boost::di::bind<interface_test>().in(scoped).to<real_test>()[boost::di::override]
        , boost::di::bind<ifactorable>().in(scoped).to<factorable>()
        , boost::di::bind<ifactory<factorable>>().to(factory<factorable>{})
    );
    // clang-format on

    std::shared_ptr<ifactorable> ptr1 = injector1.create<std::shared_ptr<ifactorable>>();
    std::shared_ptr<ifactorable> ptr2 = injector1.create<std::shared_ptr<ifactorable>>();
    std::unique_ptr<ifactorable> ptr3 = injector1.create<std::unique_ptr<factorable>>();
    std::unique_ptr<ifactorable> ptr4 = injector1.create<ifactory<factorable> &>().create();
    std::unique_ptr<ifactorable> ptr5 = injector1.create<ifactory<factorable> &>().create();

    std::shared_ptr<ifactorable> ptr6 = injector2.create<std::shared_ptr<ifactorable>>();
	std::unique_ptr<ifactorable> ptr7 = injector2.create<ifactory<factorable> &>().create();
	
	auto & fac1 = injector1.create<ifactory<factorable> &>();
	auto & fac2 = injector2.create<ifactory<factorable> &>();

	printf("%d should NOT be equal %d\n\n", &fac1, &fac2);

	printf("These should be equal!\n");
	printf("ptr 1 = %d\n", ptr1->get_address());
	printf("ptr 2 = %d\n", ptr2->get_address());
	printf("ptr 3 = %d\n", ptr3->get_address());
	printf("ptr 4 = %d\n", ptr4->get_address());
	printf("ptr 5 = %d\n", ptr5->get_address());
	printf("These should be equal (but different from previous group)!\n");
	printf("ptr 6 = %d\n", ptr6->get_address());
	printf("ptr 7 = %d\n", ptr7->get_address());

    return 0;
}
#endif

#if 0

#include <SDL.h>
#include <cstdio>
#include <cstdlib>
#include <thread>
#include <chrono>
#include "bootstrapper.hpp"
#include "core/vfs/filesystem.hpp"
#include "utility/text/ustring.hpp"
#include "core/vfs/virtual_path.hpp"
#include "settings/default_archive.hpp"
#include "settings/default_paths.hpp"
#include "settings/default_terminal.hpp"
#include "settings/default_logger.hpp"
#include "settings/default_config.hpp"
#include "settings/default_environment_info.hpp"
#include "utility/text/ustring.hpp"
#include "utility/concurrention/thread_pool.hpp"

class sample_job_t final : public engine::thread_pool_job_t
{

public:

	sample_job_t(std::shared_ptr<engine::logger_t> logger) : logger(logger)
	{
		id++;
		cur_id = id;
	}

	engine::ustring_t get_name() const final
	{
		return engine::format_string("Sample job #1#"_u, cur_id);
	}

	void run(engine::thread_pool_token_t * token) final
	{
		for (int i = 0; i < 5; i++)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(750));
			token->get_logger()->log_msg(core, "Simulating execution #1#: #2#..."_u, cur_id, i);

			if (token->is_shutdown_requested())
			{
				token->get_logger()->log_msg(core, "Requested #1# termination before completion!"_u, cur_id);
				return;
			}
		}

	}

private:

	uint32_t cur_id;
	static uint32_t id;
	std::shared_ptr<engine::logger_t> logger;
};

uint32_t sample_job_t::id;

int main(int arg, char * argv[])
{

	engine::bootstrapper_t <

		APP(game),

		ENABLE_COMPONENT(logger),
		ENABLE_COMPONENT(terminal_writer),
		ENABLE_COMPONENT(config),
		ENABLE_COMPONENT(environment_info),
		ENABLE_COMPONENT(logger_file_writer),
		ENABLE_COMPONENT(data_source),
		ENABLE_COMPONENT(data_archives),
		ENABLE_COMPONENT(storage),

#if PIXIE_WINDOWS
#if PIXIE_IS_PORTABLE_BUILD
		USE_SETTINGS(save_location_resolver_t, windows_portable),
		USE_SETTINGS(scanners_provider_settings_t, windows_portable),
#else
		USE_SETTINGS(save_location_resolver_t, windows_normal),
		USE_SETTINGS(scanners_provider_settings_t, windows_normal),
#endif
#endif
#if PIXIE_IS_DEBUG_BUILD
		USE_SETTINGS(config_t, debug),
#else
		USE_SETTINGS(config_t, release),
#endif
		USE_SETTINGS(data_archives_t, normal),
		USE_SETTINGS(common_filenames_t, normal),
		USE_SETTINGS(terminal_writer_colors_t, normal),
		USE_SETTINGS(logger_output_t, normal),
		USE_SETTINGS(environment_info_output_t, normal),

		USE_PROVIDER_FOR(config, storage),
#if PIXIE_WINDOWS
		USE_PROVIDER_FOR(config_storage, windows_registry),
#endif
		
		USE_PROVIDERS_FOR(environment_info_output, terminal, file),
		USE_PROVIDERS_FOR(logger_output, terminal, file)
	
	> bootstrapper;

	std::shared_ptr<engine::terminal_writer_t> terminal = bootstrapper.construct_component<engine::terminal_writer_t>();
	std::shared_ptr<engine::logger_t> logger = bootstrapper.construct_component<engine::logger_t>();
	std::shared_ptr<engine::config_t> config = bootstrapper.construct_component<engine::config_t>();
	std::shared_ptr<engine::environment_info_t> environment_info = bootstrapper.construct_component<engine::environment_info_t>();
	std::shared_ptr<engine::data_source_t> data_source = bootstrapper.construct_component<engine::data_source_t>();
	std::shared_ptr<engine::thread_pool_t<4>> thread_pool = bootstrapper.construct_component<engine::thread_pool_t<4>>();
	std::shared_ptr<engine::process::runner_engine_thread_t<engine::process::runner_thread_t::engine_update> > runner_update = bootstrapper.construct_component<engine::process::runner_engine_thread_t<engine::process::runner_thread_t::engine_update> >();

	thread_pool->enqueue_job(std::make_unique<sample_job_t>(logger));
	thread_pool->enqueue_job(std::make_unique<sample_job_t>(logger));
	thread_pool->enqueue_job(std::make_unique<sample_job_t>(logger));
	thread_pool->enqueue_job(std::make_unique<sample_job_t>(logger));
	thread_pool->enqueue_job(std::make_unique<sample_job_t>(logger));
	thread_pool->enqueue_job(std::make_unique<sample_job_t>(logger));
	thread_pool->enqueue_job(std::make_unique<sample_job_t>(logger));
	thread_pool->enqueue_job(std::make_unique<sample_job_t>(logger));
	
	int i = 0;

	auto task = logger->log_task_start(core, "Calculating"_u);
	logger->log_task_done(task);

	task = logger->log_task_start(core, "Calculating no 2"_u);
	logger->log_task_failed(task);
	task = logger->log_msg(core, "Sample message"_u);
	task = logger->log_warn(core, "Sample warning"_u);
	task = logger->log_err(core, "Sample error"_u);
	
	std::this_thread::sleep_for(std::chrono::seconds(1));
	config->set_cfg_has_terminal(false);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	config->set_cfg_has_terminal(true);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	
	for (;;)
	{
		runner_update->run_all_available_once();
		
		/*
		if (i == 250000) config->set_game_has_console(false);
		if (i == 400000) config->set_game_has_console(true);
		if (i == 500000) break;
		*/

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		i++;
//		if (i == 10000) break;
		

		/*
		logger->log_msg(core, "Hello world: #1#!"_u, ++i);
		if ((i % 100) == 66)
			logger->log_err(core, "Critical failure!"_u);
			*/
	}

	runner_update->run_on_shutdown();

	return 0;
}

#endif