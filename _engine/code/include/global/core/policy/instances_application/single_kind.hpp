#ifndef ENGINE_IGNITER_INSTANCES_APPLICATION_single_given_kind_HPP
#define ENGINE_IGNITER_INSTANCES_APPLICATION_single_given_kind_HPP
#pragma once

#include "global/core/policy/instances_application.hpp"

namespace engine
{

    class instances_application_single_given_kind_t : public instances_application_t
    {

    public:

        instances_application_single_given_kind_t(std::shared_ptr<messenger_accountable_app_t> messenger_accountable_app) : messenger_accountable_app(messenger_accountable_app)
        {
            messenger_instance = messenger_accountable_app->register_callback([this](messenger::msg_accountable_app_t* msg){on_accountable_changed(msg);}, messenger::history_t::dump_if_available);
        }

        bool allow_application_instance_start(app_t::kind_t kind) final
        {
			for(auto app_meta : apps_meta)
			{
				if(app_meta->get_app() == kind)
				{
					return false;
				}
			}
            return true;
        }

    private:

        std::shared_ptr<messenger_accountable_app_t> messenger_accountable_app;
		std::unique_ptr<messenger::instance_t<messenger::msg_accountable_app_t> > messenger_instance;
		std::vector<std::shared_ptr<app_meta_t> > apps_meta;
    
		void on_accountable_created(messenger::msg_accountable_app_t * msg)
		{
			auto iter = std::find(apps_meta.begin(), apps_meta.end(), msg->get_object()->get_meta());
			if(iter == apps_meta.end())
				apps_meta.push_back(msg->get_object()->get_meta());
		}

        void on_accountable_destroyed(messenger::msg_accountable_app_t * msg)
		{
			auto iter = std::find(apps_meta.begin(), apps_meta.end(), msg->get_object()->get_meta());
			if(iter != apps_meta.end())
				apps_meta.erase(iter);
		}

		void on_accountable_changed(messenger::msg_accountable_app_t* msg)
		{
			switch(msg->get_type())
			{
				case messenger::accountable_type_t::create: 
                    on_accountable_created(msg);
				break;
				case messenger::accountable_type_t::destroy: 
                    on_accountable_destroyed(msg);
				break;
			}
		}

    };

}

#endif