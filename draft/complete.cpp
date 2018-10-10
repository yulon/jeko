#include <iostream>
#include <cassert>

//JEKO_AUTOGEN_BEGIN TOOLVER:0
#include <type_traits>
#include <memory>
#define jeko struct
#define _JEKO_METHODS(_NAME) \
_JEKO_TEMPLATE_HEADER \
_NAME(Impl *obj) { \
	if (obj) { \
		*reinterpret_cast<Impl **>(&_owner_ptr_) = obj; \
		_method_table_ptr_ = &_method_table_t::static_binding<Impl *>(); \
	} else { \
		_method_table_ptr_ = nullptr; \
	} \
} \
_JEKO_TEMPLATE_HEADER \
_NAME(std::shared_ptr<Impl> &&obj) { \
	if (obj) { \
		new (reinterpret_cast<std::shared_ptr<Impl> *>(&_owner_ptr_)) std::shared_ptr<Impl>(std::move(obj)); \
		_method_table_ptr_ = &_method_table_t::static_binding<std::shared_ptr<Impl>>(); \
	} else { \
		_method_table_ptr_ = nullptr; \
	} \
} \
_JEKO_TEMPLATE_HEADER \
_NAME(const std::shared_ptr<Impl> &obj) { \
	if (obj) { \
		new (reinterpret_cast<std::shared_ptr<Impl> *>(&_owner_ptr_)) std::shared_ptr<Impl>(obj); \
		_method_table_ptr_ = &_method_table_t::static_binding<std::shared_ptr<Impl>>(); \
	} else { \
		_method_table_ptr_ = nullptr; \
	} \
} \
_JEKO_TEMPLATE_HEADER \
_NAME(std::unique_ptr<Impl> &&obj) : _NAME(std::shared_ptr<Impl>(obj.release())) {} \
_JEKO_TEMPLATE_HEADER \
_NAME(const std::unique_ptr<Impl> &obj) : _NAME(obj.get()) {} \
_JEKO_TEMPLATE_HEADER \
_NAME(Impl &&obj) : _NAME(std::make_shared<typename std::decay<Impl>::type>(std::forward<Impl>(obj))) {} \
_JEKO_TEMPLATE_HEADER \
_NAME(Impl &obj) : _NAME(&obj) {} \
~_NAME() { \
	if (!_has_value()) { \
		return; \
	} \
	if (_method_table_ptr_->_destructor) { \
		_method_table_ptr_->_destructor(this); \
	} \
	_method_table_ptr_ = nullptr; \
} \
bool _has_value() const { \
	return _method_table_ptr_; \
} \
operator bool() const { \
	return _has_value(); \
} \
const std::type_info &_type() const { \
	return _method_table_ptr_ ? *_method_table_ptr_->_type : typeid(std::nullptr_t); \
};
//JEKO_AUTOGEN_END

jeko animal {
	size_t age() const;

	////////////////////////////////////////////////////////////////////

	//JEKO_AUTOGEN_BEGIN TOOLVER:0 DECLHASH:0

	#define _JEKO_TEMPLATE_HEADER \
	template < \
		typename Impl, \
		typename = decltype(Impl::age) \
	>

	_JEKO_METHODS(animal)

	#undef _JEKO_TEMPLATE_HEADER

	struct _method_table_t {
		const std::type_info *_type;
		void (*_destructor)(animal *);
		size_t (*age)(const animal *);

		template <typename Owner>
		struct _owner_ptr_element {
			using type = typename Owner::element_type;
		};

		template <typename OwnerElmt>
		struct _owner_ptr_element<OwnerElmt *> {
			using type = OwnerElmt;
		};

		template <typename Owner>
		static constexpr _method_table_t static_bind() {
			return _method_table_t{
				&typeid(typename _owner_ptr_element<Owner>::type),
				std::is_pointer<Owner>::value ?
					nullptr :
					static_cast<void (*)(animal *)>([](animal *self) {
						(*reinterpret_cast<Owner *>(self)).~Owner();
					})
				,
				[](const animal *self)->size_t {
					return (**reinterpret_cast<Owner *>(reinterpret_cast<uintptr_t>(&self->_owner_ptr_))).age();
				}
			};
		}

		template <typename Owner>
		static _method_table_t &static_binding() {
			static _method_table_t inst = static_bind<Owner>();
			return inst;
		}
	};

	_method_table_t *_method_table_ptr_;
	uint8_t _owner_ptr_[sizeof(std::shared_ptr<void>)];
};
inline size_t animal::age() const {
	return _method_table_ptr_->age(this);
}
//JEKO_AUTOGEN_END

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
	animal aml1(dog_lval); // Impl &
	animal aml2(&dog_lval); // Impl *
	animal aml3(dog_unique_ptr_lval); // std::unique_ptr<Impl> &

	// save shared pointer
	animal aml4(std::make_shared<dog>()); // std::shared_ptr<Impl>
	animal aml5(dog{}); // Impl &&
	animal aml6(std::make_unique<dog>()); // std::unique_ptr<Impl> &&

	////////////////////////////////////////////////////////////////////////////

	assert(aml1.age() == dog_lval.age());
	assert(aml2.age() == dog_lval.age());
	assert(aml3.age() == dog_lval.age());
	assert(aml4.age() == dog_lval.age());
	assert(aml5.age() == dog_lval.age());
	assert(aml6.age() == dog_lval.age());
}
