#include "global/core/app/context.hpp"
#include "global/core/app.hpp"
#include "global/core/thread/thread.hpp"
#include "global/core/accountable.hpp"
#include "global/component/program_args.hpp"

int32_t engine::app_context_t::next_instance_id = 1;
engine::instance_id_t engine::instance_id;

engine::program_args_real_t::program_args_real_t(std::shared_ptr<engine::args_t> args) : args(*(args.get()))
{

}

engine::app_t::app_t(std::unique_ptr<app_actual_t> app, std::unique_ptr<app_context_t> context, std::shared_ptr<holder_t<accountable_app_t> > notifier, std::shared_ptr<thread_factory_t> thread_factory) : context(context.get()), app(std::move(app)), meta(std::make_shared<app_meta_t>(this->app->get_app(), std::move(context)))
{
    thread = thread_factory->create(this->app->get_app(), this->meta->get_instance_id(), format_string("App #1#:#2# main thread"_u, this->app->get_app(), this->meta->get_instance_id()), [this]{this->run_app();});
    instance = std::make_unique<accountable_app_instance_t>(notifier, this);
}

engine::app_t::~app_t()
{
    meta->wait_till_completed();
}

void engine::app_t::run_app()
{
    meta->set_return_code(app->main(context));
    meta->notify_on_completion();
}