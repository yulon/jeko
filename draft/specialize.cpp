#include <iostream>
#include <cassert>

// _JEKO_HEADER_BEGIN VARSION:0
// The Jeko Documentation at https://github.com/yulon/jeko
#include <type_traits>
#include <typeinfo>
#include <typeindex>
#include <memory>
#include <unordered_map>

#ifdef __cpp_lib_shared_mutex
	#include <shared_mutex>
#else
	#include <mutex>
#endif

template <typename MethodSet>
class jeko;

namespace _jeko {
	template <typename P>
	struct _ptr_elmt;
	template <typename Elmt>
	struct _ptr_elmt<Elmt *> {
		using type = Elmt;
	};
	template <typename Elmt>
	struct _ptr_elmt<std::shared_ptr<Elmt>> {
		using type = Elmt;
	};
	template <typename Elmt>
	struct _ptr_elmt<std::unique_ptr<Elmt>> {
		using type = Elmt;
	};
};

#define _JEKO_BASE \
jeko(std::nullptr_t = nullptr) { \
	value()._method_table_ptr = nullptr; \
} \
_JEKO_CONCEPT(Impl) \
jeko(Impl *obj) { \
	if (!obj) { \
		value()._method_table_ptr = nullptr; \
		return; \
	} \
	*reinterpret_cast<Impl **>(&value()._owner_ptr) = obj; \
	value()._method_table_ptr = &wrapper::_method_table_t::static_binding<Impl *>(); \
} \
\
_JEKO_CONCEPT(Impl) \
jeko(std::shared_ptr<Impl> &&obj) { \
	if (!obj) { \
		value()._method_table_ptr = nullptr; \
		return; \
	} \
	new (reinterpret_cast<std::shared_ptr<Impl> *>(&value()._owner_ptr)) std::shared_ptr<Impl>(std::move(obj)); \
	value()._method_table_ptr = &wrapper::_method_table_t::static_binding<std::shared_ptr<Impl>>(); \
} \
\
_JEKO_CONCEPT(Impl) \
jeko(const std::shared_ptr<Impl> &obj) { \
	if (!obj) { \
		value()._method_table_ptr = nullptr; \
		return; \
	} \
	new (reinterpret_cast<std::shared_ptr<Impl> *>(&value()._owner_ptr)) std::shared_ptr<Impl>(obj); \
	value()._method_table_ptr = &wrapper::_method_table_t::static_binding<std::shared_ptr<Impl>>(); \
} \
\
_JEKO_CONCEPT(Impl) \
jeko(std::unique_ptr<Impl> &&obj) : jeko(std::shared_ptr<Impl>(obj.release())) {} \
\
_JEKO_CONCEPT(Impl) \
jeko(const std::unique_ptr<Impl> &obj) : jeko(obj.get()) {} \
\
_JEKO_CONCEPT(Impl) \
jeko(Impl &&obj) : jeko(std::make_shared<typename std::decay<Impl>::type>(std::forward<Impl>(obj))) {} \
\
_JEKO_CONCEPT(Impl) \
jeko(Impl &obj) : jeko(&obj) {} \
\
jeko(const jeko &src) { \
	if (!src) { \
		value()._method_table_ptr = nullptr; \
		return; \
	} \
	src->_method_table_ptr->_copy_owner_ptr(&src->_owner_ptr, &value()._owner_ptr); \
	value()._method_table_ptr = src->_method_table_ptr; \
} \
\
jeko &operator=(const jeko &src) { \
	reset(); \
	new (this) jeko(src); \
	return *this; \
} \
\
jeko(jeko &&src) { \
	if (!src) { \
		value()._method_table_ptr = nullptr; \
		return; \
	} \
	src->_method_table_ptr->_move_owner_ptr(&src->_owner_ptr, &value()._owner_ptr); \
	value()._method_table_ptr = src->_method_table_ptr; \
	src->_method_table_ptr = nullptr; \
} \
\
jeko &operator=(jeko &&src) { \
	reset(); \
	new (this) jeko(std::move(src)); \
	return *this; \
} \
\
_JEKO_CONCEPT(OtherMethodSet) \
jeko(const jeko<OtherMethodSet> &src) { \
	if (!src) { \
		value()._method_table_ptr = nullptr; \
		return; \
	} \
	src->_method_table_ptr->_copy_owner_ptr(&src->_owner_ptr, &value()._owner_ptr); \
	value()._method_table_ptr = &wrapper::_method_table_t::dynamic_binding(src); \
} \
\
_JEKO_CONCEPT(OtherMethodSet) \
jeko(jeko<OtherMethodSet> &&src) { \
	if (!src) { \
		value()._method_table_ptr = nullptr; \
		return; \
	} \
	src->_method_table_ptr->_move_owner_ptr(&src->_owner_ptr, &value()._owner_ptr); \
	value()._method_table_ptr = &wrapper::_method_table_t::dynamic_binding(src); \
	src->_method_table_ptr = nullptr; \
} \
\
~jeko() { \
	reset(); \
} \
\
void reset() { \
	if (!has_value()) { \
		return; \
	} \
	if (value()._method_table_ptr->_free_owner_ptr) { \
		value()._method_table_ptr->_free_owner_ptr(&value()._owner_ptr); \
	} \
	value()._method_table_ptr = nullptr; \
} \
\
bool has_value() const { \
	return value()._method_table_ptr; \
} \
\
operator bool() const { \
	return has_value(); \
} \
\
const std::type_info &type() const { \
	return value()._method_table_ptr ? *value()._method_table_ptr->_type_info_ptr : typeid(std::nullptr_t); \
} \
\
wrapper &value() { \
	return reinterpret_cast<wrapper &>(_value); \
} \
\
const wrapper &value() const { \
	return reinterpret_cast<const wrapper &>(_value); \
} \
\
wrapper &operator*() { \
	return value(); \
} \
\
const wrapper &operator*() const { \
	return value(); \
} \
\
wrapper *operator->() { \
	return &value(); \
} \
\
const wrapper *operator->() const { \
	return &value(); \
} \
\
private: \
uint8_t _value[sizeof(wrapper)];
// _JEKO_HEADER_END

struct animal {
	size_t age() const;
};

/*
1.Define empty specialized for jeko, such as `template <> class jeko<MethodSet> {};`.
2.Run tool.
3.Generate code in `{}`.
*/

template <>
class jeko<animal> {
// _JEKO_SPECIALIZED METHODSET_HASH:0
// The Jeko Documentation at https://github.com/yulon/jeko
#define _JEKO_CONCEPT(_T) \
template < \
	typename _T, \
	typename = decltype(std::remove_reference<_T>::type::age) \
>
public:
	class wrapper {
	public:
		wrapper() = delete;
		wrapper(const wrapper &) = delete;
		wrapper(wrapper &&) = delete;
		wrapper &operator=(const wrapper &) = delete;
		wrapper &operator=(wrapper &&) = delete;

		size_t age() const {
			return _method_table_ptr->age(this);
		}

		struct _method_table_t {
			const std::type_info *_type_info_ptr;
			void (*_free_owner_ptr)(void *);
			void (*_copy_owner_ptr)(const void *, void *);
			void (*_move_owner_ptr)(void *, void *);
			size_t (*age)(const wrapper *);

			template <typename Owner>
			static constexpr _method_table_t static_bind() {
				return _method_table_t{
					&typeid(typename _jeko::_ptr_elmt<Owner>::type),
					std::is_pointer<Owner>::value ?
						nullptr :
						static_cast<void (*)(void *)>([](void *owner_ptr_ptr) {
							(*reinterpret_cast<Owner *>(owner_ptr_ptr)).~Owner();
						})
					,
					std::is_pointer<Owner>::value ?
						nullptr :
						static_cast<void (*)(const void *, void *)>([](const void *src, void *dest) {
							new (reinterpret_cast<Owner *>(dest)) Owner(*reinterpret_cast<const Owner *>(src));
						})
					,
					std::is_pointer<Owner>::value ?
						nullptr :
						static_cast<void (*)(void *, void *)>([](void *src, void *dest) {
							new (reinterpret_cast<Owner *>(dest)) Owner(std::move(*reinterpret_cast<Owner *>(src)));
						})
					,
					[](const wrapper *self)->size_t {
						return (**reinterpret_cast<Owner *>(reinterpret_cast<uintptr_t>(&self->_owner_ptr))).age();
					}
				};
			}

			template <typename Owner>
			static _method_table_t &static_binding() {
				static _method_table_t inst = static_bind<Owner>();
				return inst;
			}

			_JEKO_CONCEPT(OtherMethodSet)
			static _method_table_t &dynamic_binding(const jeko<OtherMethodSet> &src) {
				static std::unordered_map<std::type_index, _method_table_t> map;

				#ifdef __cpp_lib_shared_mutex
					static std::shared_mutex mtx;
					mtx.lock_shared();
				#else
					static std::mutex mtx;
					std::lock_guard<std::mutex> lg(mtx);
				#endif

				auto &src_mt = *src->_method_table_ptr;
				auto it = map.find(*src_mt._type_info_ptr);
				if (it == map.end()) {
					#ifdef __cpp_lib_shared_mutex
						mtx.unlock_shared();
						std::lock_guard<std::mutex> lg(mtx);
					#endif

					auto &mt = map[*src_mt._type_info_ptr];
					mt._type_info_ptr = src_mt._type_info_ptr;
					mt._free_owner_ptr = src_mt._free_owner_ptr;
					mt._copy_owner_ptr = src_mt._copy_owner_ptr;
					mt._move_owner_ptr = src_mt._move_owner_ptr;
					mt.age = src_mt.age;
					return mt;
				}
				#ifdef __cpp_lib_shared_mutex
					mtx.unlock_shared();
				#endif
				return it->second;
			}
		};

		_method_table_t *_method_table_ptr;
		uint8_t _owner_ptr[sizeof(std::shared_ptr<void>)];
	};

	_JEKO_BASE

#undef _JEKO_CONCEPT
};

struct dog {
	size_t age() const {
		return 123;
	}
	size_t color() const {
		return 321;
	}
};

int main() {
	dog dog_lval;
	std::unique_ptr<dog> dog_unique_ptr_lval(new dog);

	////////////////////////////////////////////////////////////////////////////

	// save raw pointer
	jeko<animal> aml1(dog_lval); // Impl &
	jeko<animal> aml2(&dog_lval); // Impl *
	jeko<animal> aml3(dog_unique_ptr_lval); // std::unique_ptr<Impl> &

	// save shared pointer
	jeko<animal> aml4(std::make_shared<dog>()); // std::shared_ptr<Impl>
	jeko<animal> aml5(dog{}); // Impl &&
	jeko<animal> aml6(std::make_unique<dog>()); // std::unique_ptr<Impl> &&

	// save pointer type same as source jeko
	jeko<animal> aml7(aml6); // jeko

	// null
	jeko<animal> aml8;
	jeko<animal> aml9(nullptr);

	////////////////////////////////////////////////////////////////////////////

	assert(aml1->age() == dog_lval.age());
	assert(aml2->age() == dog_lval.age());
	assert(aml3->age() == dog_lval.age());
	assert(aml4->age() == dog_lval.age());
	assert(aml5->age() == dog_lval.age());
	assert(aml6->age() == dog_lval.age());
	assert(aml7->age() == dog_lval.age());
	assert(!aml8);
	assert(!aml9);
}
