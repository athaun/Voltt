#pragma once

#include <cstdint>

namespace Mem {

using RelPrecision = int32_t;

template<typename To, typename Precision = RelPrecision>
auto make_relative(To* _ptr, Precision _base) -> Precision
{
	return _ptr ? (Precision)((uint8_t*)(_ptr) - (uint8_t*)&_base) : 0;
}

template<typename To, typename Precision = RelPrecision>
auto from_relative(To _base) -> To*
{
	return _base ? (To*)((uint8_t*)((&_base) + _base)) : nullptr;
}

} // namespace Mem

/*
 * WARNING, THIS DATA STRUCTURE WILL RESULT IN UNDEFINED BEHAVIOR IN CERTAIN CASES
 * EX:
 *		auto ret_heap_data() -> RelPtr<Type>
 *		{
 *			Type* data = new Type();
 *			return RelPtr<Type>(data);
 *		}
 *
 *	Why does this not work. In theory.
 *	When the relative pointer is instantiated in the return statement, the base within is allocated to the stack.
 *	The relativity of the pointer is determined by the memory location of the base, which is stack allocated.
 *	Because the base is allocated to temporary stack memory, a new location is assumed at the call site of the function.
 *	This is where the bug occurs. Becuase the address of the base has changed, so does the relativity of the pointer.
 *	So while the base can maintain the same value, when it comes time to aquire the address of the data we are pointing to,
 *	the relativity of the base has changed resulting in undefined behavior.
 *
 *	However, relative pointers can still be used in this scenario but they need to be implemented per use.
 *	This relative pointer object can be used as a pattern that can be implemented into other data structures.
*/

template<typename To, typename Precision = int32_t>
struct RelPtr {
	Precision base;

	auto get() -> To*
	{
		return base ? (To*)((uint8_t*)(&base + base)) : nullptr;
	}

	auto from(To* _ptr) -> void
	{
		base = _ptr ? (Precision)((uint8_t*)(_ptr) - (uint8_t*)&base) : 0;
	}

	auto operator->() -> RelPtr<To, Precision>*
	{
		return this;
	}

	auto operator*() -> To&
	{
		return *this->get(); 
	}

	auto operator=(To* _ptr) -> void
	{
		this->from(_ptr);
	}

	explicit RelPtr(To* _ptr)
	: base(_ptr ? (Precision)((uint8_t*)(_ptr) - (uint8_t*)&base) : 0)
	{}

	explicit RelPtr()
	: base(0)
	{}
};
