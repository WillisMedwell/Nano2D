#pragma once

namespace n2d {
    class activity {
    public:
        virtual void update(double delta_time_ms) = 0;
        virtual void render() = 0;
        virtual bool is_running() = 0;
        virtual ~activity() = default;
    };
}