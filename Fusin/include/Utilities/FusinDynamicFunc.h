#ifndef _FUSIN_DYN_FUNC_H
#define _FUSIN_DYN_FUNC_H

#include <functional>

namespace Fusin
{
	template <class _Dest>
	class DynamicFunc;

	template <class _RetDest, class ... _ArgsDest>
	class DynamicFunc<_RetDest(_ArgsDest ...)>
	{
	public:
		using ImplType = std::function<_RetDest(_ArgsDest ...)>;

		DynamicFunc() noexcept {}
		DynamicFunc(nullptr_t) noexcept {}

		DynamicFunc(const DynamicFunc<_RetDest(_ArgsDest ...)>& other):
			mFunction(other.mFunction)
		{}

		template <class _Fx>
		DynamicFunc(_Fx func) :
			DynamicFunc(std::function(func))
		{};

		template <class _RetSrc, class ... _ArgsSrc>
		DynamicFunc(std::function<_RetSrc(_ArgsSrc ...)> func):
			mFunction(
				[func](_ArgsDest... args) {
					return (_RetDest)func(dynamic_cast<_ArgsSrc...>(args...));
				}
			)
		{
			static_assert((sizeof...(_ArgsSrc) == sizeof...(_ArgsDest)), "The function func does not have the expected number of parameters.");
		};

		DynamicFunc& operator=(const DynamicFunc<_RetDest(_ArgsDest ...)> & other) {
			mFunction = other.mFunction;
		}

		explicit operator bool() const {
			return mFunction.operator bool();
		}

		_RetDest operator()(_ArgsDest... args) const {
			return mFunction(args...);
		}

		const type_info& target_type() const noexcept {
			return mFunction.target_type()
		}

		template <class TargetType>
		TargetType* target() noexcept {
			return mFunction.target<TargetType>();
		}

		template <class TargetType>
		const TargetType* target() const noexcept {
			return mFunction.target<TargetType>();
		}

		void swap(const DynamicFunc<_OwnerDest, _RetDest(_ArgsDest ...)>& other) noexcept {
			mFunction.swap(other.mFunction);
		}

	private:
		ImplType mFunction;
	};
}

#endif
