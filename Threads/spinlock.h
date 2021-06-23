#ifndef __spinlock_h__
#define __spinlock_h__

#include <thread>
#include <atomic>
#include <cassert>

namespace ThreadLib
{


	//ALIGNMENT - minimum alignment
	template <size_t ALIGNMENT = sizeof(std::atomic_flag)>
	class SpinlockAligned
	{
		alignas(ALIGNMENT) std::atomic_flag flag = {};
	public:
		void lock()
		{
			while (flag.test_and_set() == true)
				std::this_thread::yield();
		}
		bool try_lock() {
			return flag.test_and_set() == false;
		}
		void unlock(){	flag.clear(); }
	};

	using Spinlock = SpinlockAligned<sizeof(std::atomic_flag)>;
	using SpinlockAlign1B = SpinlockAligned<1>;
	using SpinlockAlign2B = SpinlockAligned<2>;
	using SpinlockAlign4B = SpinlockAligned<4>;
	using SpinlockAlign8B = SpinlockAligned<8>;
	using SpinlockAlign16B = SpinlockAligned<16>;
	using SpinlockAlign32B = SpinlockAligned<32>;
	using SpinlockAlign64B = SpinlockAligned<64>;
	using SpinlockAlign128B = SpinlockAligned<128>;
	using SpinlockAlign256B = SpinlockAligned<256>;
	
	
	
	
	
	
	
	
	
	//ALIGNMENT - minimum alignment
	template <size_t ALIGNMENT = sizeof(std::atomic_flag)>
	class SpinlockRecurrentAligned
	{
		alignas(ALIGNMENT) std::atomic_flag flag = {};
		size_t recursionCounter;
		std::atomic<std::thread::id> thid;
		
	public:
	
		void lock()
		{
			while (try_lock() == false)
				std::this_thread::yield();
		}
		
		bool try_lock() {
			
			if(flag.test_and_set() == false)
			{
				thid.store(std::this_thread::get_id());
				recursionCounter = 1;
				return true;
			}
			else 
			{
				if(thid.load() == std::this_thread::get_id())
				{
					recursionCounter++;
					return true;
				}
				else
					return false;
			}
		}
		
		void unlock(){
			assert(thid.load() == std::this_thread::get_id());
			if(recursionCounter-- == 1)
			{
				thid = std::thread::id();
				flag.clear(); 
			}
		}
	};

	using SpinlockRecurrent = SpinlockRecurrentAligned<sizeof(std::atomic_flag)>;
	using SpinlockRecurrentAlign1B = SpinlockRecurrentAligned<1>;
	using SpinlockRecurrentAlign2B = SpinlockRecurrentAligned<2>;
	using SpinlockRecurrentAlign4B = SpinlockRecurrentAligned<4>;
	using SpinlockRecurrentAlign8B = SpinlockRecurrentAligned<8>;
	using SpinlockRecurrentAlign16B = SpinlockRecurrentAligned<16>;
	using SpinlockRecurrentAlign32B = SpinlockRecurrentAligned<32>;
	using SpinlockRecurrentAlign64B = SpinlockRecurrentAligned<64>;
	using SpinlockRecurrentAlign128B = SpinlockRecurrentAligned<128>;
	using SpinlockRecurrentAlign256B = SpinlockRecurrentAligned<256>;
	
	
	static_assert(std::atomic<std::thread::id>::is_always_lock_free == true, "SpinlockRecurrentAligned can be non-lockfree");
}



#endif
