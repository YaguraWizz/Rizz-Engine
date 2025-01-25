#pragma once

#define BIT(x) (1 << x)
 
#define INLINE inline
#define NOEXCEPT noexcept
#define CONST const
#define OVERRIDE override
#define STATIC static
#define VIRTUAL virtual


#define GENERATE_GETTER(type, name, args) \
    type Get##name() const NOEXCEPT { return args; }

#define GENERATE_SETTER(type, name, args) \
    void Set##name(const type& value) NOEXCEPT { args = value; }

#define GENERATE_IS_METHOD(name, args) \
    bool is##name() const NOEXCEPT { return args; }

#define GENERATE_STATIC_METHOD(type, method_name, return_value) \
    static type method_name() NOEXCEPT { return return_value; }

#define GENERATE_OVERRIDE_METHOD(type, method_name, params, body) \
    type method_name params override { return body; }



#ifdef RZ_ENABLE_ASSERT 
	#define RZ_ASSERT(x,...) { if(!(x)) {RZ_ERROR("Asserting Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define RZ_CORE_ASSERT(x,...) { if(!(x)) {RZ_CORE_ERROR("Asserting Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define RZ_ASSERT(x,...)
	#define RZ_CORE_ASSERT(x,...)
#endif