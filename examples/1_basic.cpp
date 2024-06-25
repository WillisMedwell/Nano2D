#include <n2d/n2d.hpp>

#include <cstdio>
#include <cstdlib>

#include <imgui.h>

class basic_activity final : public n2d::activity {
public:
    void update(double delta_time_ms) override 
    {
        
    }
    void render() override 
    {
        ImGui::Begin("Basic");
        ImGui::Text("This is the most simple app");
        ImGui::End();
    }
    bool is_running() override 
    {
        return false;
    }
};


int main()
{
    auto app = n2d::app::create<basic_activity>();
    if(app.has_error())
    {
        std::fprintf(stderr, "%s", *app.error());
        std::fflush(stderr);
        std::abort();
    }
    n2d::run_app(app.pop_value());
}