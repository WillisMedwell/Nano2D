#pragma once

#include <concepts>
#include <type_traits>

namespace n2d 
{
    class str_literal {
        struct M {
            const char* str;
        } _m;
    public:
        constexpr str_literal(const str_literal& other) noexcept : _m{other._m} {}
        consteval explicit str_literal(const char* str) noexcept : _m{str} {}    
        [[nodiscard]] constexpr const char* operator*() const noexcept { return _m.str; }
    };

    template <typename value_type>
    class result {
        struct M {
            union {
                unsigned char value[sizeof(value_type)];
                str_literal error;
            };
            bool has_error;

            constexpr ~M() = default;
        } _m;

    public:
        constexpr explicit result(str_literal error_message)
            : _m {
                .error = error_message, 
                .has_error = true
            } {}

        result() = delete;

        template<typename... args_t>
        constexpr explicit result(args_t&&... args) /* requires std::constructible_from<value_type, args_t&&...> // this crashes msvc. */
            : _m { .value = { 0 }, .has_error = false } 
        { 
            new(_m.value) value_type(std::forward<args_t>(args)...); 
        }

        constexpr explicit result(value_type&& value) requires std::move_constructible<value_type>
            : _m { .value = { 0 }, .has_error = false } 
            { new(_m.value) value_type(std::move(value)); }

        constexpr explicit result(const value_type& value) requires std::copy_constructible<value_type>
            : _m {.value = { 0 }, .has_error = false} 
            { new(_m.value) value_type(value); }

        constexpr ~result() requires (!std::is_trivially_destructible_v<value_type>)
        {
            if(!_m.has_error)
            {
                reinterpret_cast<value_type*>(_m.value)->~value_type();
            }
            _m.error = str_literal(nullptr);
            _m.has_error = false;
        }
        constexpr ~result() requires std::is_trivially_destructible_v<value_type> = default;

        constexpr inline operator bool() const noexcept { return !_m.has_error; }
        constexpr inline bool has_value() const noexcept { return *this; }
        constexpr inline bool has_error() const noexcept { return !(*this); }

        constexpr inline str_literal error() const noexcept { return _m.error; }

        inline value_type* operator->() noexcept { return reinterpret_cast<value_type*>(_m.value); }
        inline const value_type* operator->() const noexcept { return reinterpret_cast<const value_type*>(_m.value); }

        // causes UB after access after.
        [[nodiscard]] inline value_type&& pop_value() requires std::is_move_constructible_v<value_type>
        {
            value_type* good_lookup_or_crash[2] = {
                reinterpret_cast<value_type*>(_m.value),
                nullptr
            };
            return static_cast<value_type&&>(*good_lookup_or_crash[_m.has_error]);        
        }

        [[nodiscard]] inline const value_type& pop_value() requires (std::is_copy_constructible_v<value_type> && !std::is_move_constructible_v<value_type>) 
        {
            value_type* good_lookup_or_crash[2] = {
                reinterpret_cast<value_type*>(_m.value),
                nullptr
            };
            return static_cast<const value_type&>(*good_lookup_or_crash[_m.has_error]);        
        }
    };  
}

namespace n2d::memory
{
    template<typename allocator>
    concept is_instance_allocator = requires(allocator a)
    {
        { a.alloc(size_t(1), size_t(1)) } -> std::same_as<void*>;
        { a.realloc(nullptr, size_t(1), size_t(1)) } -> std::same_as<void*>;
        { a.dealloc(nullptr) } -> std::same_as<void>;
    };

    template<typename allocator>
    concept is_static_allocator = requires(allocator a) {
        { allocator::alloc(size_t(1), size_t(1)) } -> std::same_as<void*>;
        { allocator::realloc(nullptr, size_t(1), size_t(1)) } -> std::same_as<void*>;
        { allocator::dealloc(nullptr) } -> std::same_as<void>;
    };

    template<typename allocator>
    concept is_allocator = is_instance_allocator<allocator> || is_static_allocator<allocator>;

    struct default_allocator {
        static void* alloc(size_t size_bytes, size_t alignment);
        static void* realloc(void* ptr, size_t size_bytes, size_t alignment);
        static void dealloc(void* ptr);
    };

    struct default_deleter {

        template<typename value_type>
        void operator()(value_type* ptr ) const
        {
            default_allocator::dealloc(reinterpret_cast<void*>(ptr));
        }   
    };

    static_assert(is_static_allocator<default_allocator>);

}