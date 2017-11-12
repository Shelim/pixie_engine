
#if 1

#include <SDL.h>
#include <cstdio>
#include <cstdlib>
#include <thread>
#include <chrono>
#include "core/bootstrapper.hpp"
#include "utility/concurrention/signal.hpp"
#include "core/messenger/queue.hpp"
#include "core/console/msg/logger.hpp"
#include "core/console/msg/meta.hpp"

/*
namespace engine
{
	class msg_sample_t : public console_msg_t
	{

	public:

		msg_sample_t() : console_msg_t(type_t::logger, console_source_t::core, console_kind_t::info)
		{

		}

	};
}
*/
int main(int arg, char * argv[])
{

	engine::bootstrapper_t <

		APP(game),

		ENABLE_COMPONENT(config_storage),
		ENABLE_COMPONENT(filesystem),
		ENABLE_COMPONENT(logger),

		USE_PROVIDER_FOR(config_storage, windows_registry),
		USE_PROVIDER_FOR(filesystem, generic)

	> bootstrapper;

	std::shared_ptr<engine::config_storage_t> config_storage = bootstrapper.construct_component<engine::config_storage_t>();
	std::shared_ptr<engine::filesystem_t> filesystem = bootstrapper.construct_component<engine::filesystem_t>();

	config_storage->store("Test"_u, "Value!"_u);
	engine::paths_t files = filesystem->iterate_files_in_directory("C:\\!Lost Empire");
	engine::paths_t dirs = filesystem->iterate_directories("C:\\!Lost Empire");
	engine::paths_t subfiles = filesystem->iterate_files_in_subdirectories("C:\\!Lost Empire", "*.cs"_u);
	engine::paths_t subdirs = filesystem->iterate_subdirectories("C:\\!Lost Empire");

	int a = 5;

	engine::bootstrapper_t <

		APP(game),

		ENABLE_COMPONENT(config_storage),
		ENABLE_COMPONENT(filesystem),
		ENABLE_COMPONENT(logger),

		USE_PROVIDER_FOR(config_storage, windows_registry),
		USE_PROVIDER_FOR(filesystem, generic),
		USE_PROVIDER_FOR(logger, console)

	> bootstrapper2;

	std::shared_ptr<engine::filesystem_t> filesystem2 = bootstrapper2.construct_component<engine::filesystem_t>();

	engine::paths_t files2 = filesystem2->iterate_files_in_directory("C:\\!Lost Empire");
	engine::paths_t dirs2 = filesystem2->iterate_directories("C:\\!Lost Empire");
	engine::paths_t subfiles2 = filesystem2->iterate_files_in_subdirectories("C:\\!Lost Empire", "*.cs"_u);
	engine::paths_t subdirs2 = filesystem2->iterate_subdirectories("C:\\!Lost Empire");

	std::shared_ptr<engine::messenger_console_t> console = bootstrapper2.construct_component<engine::messenger_console_t>();

	std::shared_ptr<engine::logger_t> logger = bootstrapper2.construct_component<engine::logger_t>();

	logger->log_msg(core, "Hello world!"_u);
	logger->log_msg(core, "Hello world number #1#!"_u, 2);

	printf("Start1\n");

	auto instance1 = console->register_callback([](engine::messenger::msg_console_t* msg)
	{
		switch(msg->get()->get_type())
		{
			case engine::messenger::msg_console_t::actual_t::type_t::meta:
			{
				engine::console::meta_item_t * meta = static_cast<engine::console::meta_item_t *>(msg->get());
				if(meta->get_meta_type() == engine::console::meta_item_t::meta_type_t::output_start)
					printf("OUTPUT STARTED!\n");
				else
					printf("OUTPUT ENDED!\n");
			}
			break;
			case engine::messenger::msg_console_t::actual_t::type_t::logger:
			{
				engine::console::logger_item_t * item = static_cast<engine::console::logger_item_t *>(msg->get());
				const engine::ustring_t msg = item->get_message();
				printf("%s\n", msg.get_cstring());
			}
			break;
		}
	}, engine::messenger::history_t::dump_if_available);


	logger->log_msg(core, "Sometime after..."_u);

	/*
	std::shared_ptr<engine::console_t> console = bootstrapper.construct_component<engine::console_t>();
	console->write(std::make_unique<engine::msg_sample_t>());
	console->write(std::make_unique<engine::msg_sample_t>());
    */
	engine::signaler_t sign;

	std::thread thread1([&sign](){printf("Starting main... %d\n", std::time(nullptr)); std::this_thread::sleep_for(std::chrono::seconds(3));sign.signal_ready();});
	std::thread thread2([&sign](){printf("Starting 1... %d\n", std::time(nullptr)); sign.get_waiter().wait_for_signal(); printf("Completed 1! %d\n", std::time(nullptr));});
	std::thread thread3([&sign](){printf("Starting 2... %d\n", std::time(nullptr)); sign.get_waiter().wait_for_signal(); printf("Completed 2! %d\n", std::time(nullptr));});
	std::thread thread4([&sign](){printf("Starting 3... %d\n", std::time(nullptr)); sign.get_waiter().wait_for_signal(); printf("Completed 3! %d\n", std::time(nullptr));});

	std::this_thread::sleep_for(std::chrono::seconds(4));
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
#include "utility/text/parser.hpp"
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