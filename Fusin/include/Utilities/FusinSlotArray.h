#ifndef _FUSIN_SLOT_ARRAY_H
#define _FUSIN_SLOT_ARRAY_H

#include <vector>
#include <stdexcept>

namespace Fusin
{
	class SlotException : public std::runtime_error
	{
	public:
		inline explicit SlotException(const std::string& what_arg) : std::runtime_error(what_arg) {}
		inline explicit SlotException(const char* what_arg) : std::runtime_error(what_arg) {}
	};

	template <class T>
	class SlotArray
	{
	public:
		static const Index no_index = -1;

		T& operator[](Index slot)
		{
			if (slot >= mSlots.size())
			{
				mSlots.reserve(slot + 1);
				mSlotsFree.reserve(slot + 1);
				for (size_t i = mSlots.size(); i < slot; i++)
				{
					mSlots.push_back(T());
					mSlotsFree.push_back(true);
				}
				mSlots.push_back(T());
				mSlotsFree.push_back(false);
				return mSlots.back();
			}
			else
			{
				mSlotsFree[slot] = false;
				return mSlots[slot];
			}
		}

		const T& operator[](Index slot) const
		{
			if (slot < mSlots.size() && !mSlotsFree[slot])
			{
				return mSlots[slot];
			}
			else
			{
				throw SlotException("Trying to access a free slot of a const SlotArray. "
					"Accessing the slot's content would need to take the slot first, which is impossible since the SlotArray is const.");
			}
		}

		Index getFreeSlot()
		{
			for (Index i = 0; i < mSlots.size(); i++)
			{
				if (mSlotsFree[i]) return i;
			}
			mSlots.push_back(T());
			mSlotsFree.push_back(false);
			return (int)mSlots.size() - 1;
		}

		Index getFreeSlot() const
		{
			for (Index i = 0; i < mSlots.size(); i++)
			{
				if (mSlotsFree[i]) return i;
			}
			throw SlotException("Can't find a free slot, or allocate a new free one since the SlotArray is const.");
		}

		void freeSlot(Index slot)
		{
			if (slot < mSlots.size())
			{
				mSlotsFree[slot] = true;
				while (mSlotsFree.back())
				{
					mSlots.pop_back();
					mSlotsFree.pop_back();
				}
			}
		}

		void clear()
		{
			mSlots.clear();
			mSlotsFree.clear();
		}

		bool isSlotFree(Index slot) const
		{
			if (slot < mSlots.size())
				return mSlotsFree[slot];
			return true;
		}

		Index find(const T& a) const
		{
			for (int i = 0; i < mSlots.size(); i++)
			{
				if (mSlots[i] == a) return i;
			}
			return no_index;
		}

		size_t size() const
		{
			return mSlots.size();
		}

	private:
		std::vector<T> mSlots;
		std::vector<bool> mSlotsFree;
	};


}

#endif