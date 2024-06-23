#include <n2d/app.hpp>

#include <cstdio>

int main()
{
    auto app = n2d::app::create();
    if(app.has_error())
    {
        std::fprintf(stderr, "%s", *app.error());
        std::fflush(stderr);
    }
    else {
        std::fprintf(stdout, "finished");
        std::fflush(stdout);
    }
}