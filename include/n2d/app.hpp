#pragma once

#include "n2d/basics.hpp"
#include "n2d/activity.hpp"

namespace n2d {
    class app {
        struct M;
        M* _m;

        friend class result<app>;
        app() = delete;
        app(M* m) : _m(m) {}
        static auto create_impl(n2d::activity* activity) -> result<app>;
    public:

        template<std::derived_from<n2d::activity> activity_derivative>
        static auto create() -> result<app>
        {
            activity_derivative* activity = reinterpret_cast<activity_derivative*>(
                n2d::memory::default_allocator::alloc(
                    sizeof(activity_derivative), 
                    alignof(activity_derivative)
                )
            );
            new(activity) activity_derivative();
            return create_impl(static_cast<n2d::activity*>(activity));
        }

        app(app&& app) : _m(app._m) { app._m = nullptr; }

        ~app();
        
        bool is_running();
        void update();
        void render();

        void switch_to_activity(n2d::activity* activity);
        
        app(const app&) = delete;
        app& operator=(const app&) = delete;
    };

    void run_app(app&& app);
}