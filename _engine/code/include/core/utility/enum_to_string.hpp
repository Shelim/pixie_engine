#define ENGINE_ENUM_START(namespace, name) template<> inline engine::ustring_t engine::to_string<namespace::name>(const namespace::name & item, const ustring_t &) { using type_t = namespace::name;
#define ENGINE_ENUM_VALUE(val, str)  if(item == type_t::val) return str;
#define ENGINE_ENUM_END()  return "unknown"_u; }
#include ENGINE_ENUM_HEADER_TO_USE

#define ENGINE_ENUM_START(namespace, name) template<> inline namespace::name engine::from_string(const engine::ustring_t & string) { using type_t = namespace::name;
#define ENGINE_ENUM_VALUE(val, str) if(str == string) return type_t::val;
#define ENGINE_ENUM_END() return type_t::count; }
#include ENGINE_ENUM_HEADER_TO_USE