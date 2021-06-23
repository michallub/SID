#ifndef __outstream_h__
#define __outstream_h__

#include <iostream>
#include <sstream>
#include <fstream>

namespace StreamLib
{
	#include "../Threads/spinlock.h"

	
	class MyStreamLocks
	{
		template<class StreamType, uint8_t LOCKID, StreamType *STREAM>
		friend class OutStream;
		
		static ThreadLib::SpinlockAlign64B mtx[256];
	};
	ThreadLib::SpinlockAlign64B MyStreamLocks::mtx[256] = {};
	
	enum class FlushSwitch{
		off = 0,
		on = 1
	};
	enum class NewLineSwitch{
		off = 0,
		on = 1
	};
	
	template<class StreamType = std::ostream, uint8_t LOCKID = 0, StreamType *STREAM = nullptr>
	class OutStream{
		std::stringstream oss;
		StreamType *stream;
		uint8_t lockID = 0;
		NewLineSwitch shouldNewLine = NewLineSwitch::on;
		FlushSwitch shouldFlush = FlushSwitch::on;
		
	public:
		OutStream(
			StreamType *stream = nullptr, 
			uint8_t lockID = 0, 
			NewLineSwitch shouldNewLine = NewLineSwitch::on, 
			FlushSwitch shouldFlush = FlushSwitch::on)
			: 
			stream(stream),lockID(lockID), shouldNewLine(shouldNewLine), shouldFlush(shouldFlush) {}
		
		
		
		template<typename Type>
		OutStream& operator<<(const Type& value)
		{
			oss << value;
			return *this;
		}
		
		void setFlush(FlushSwitch ShouldFlush) noexcept {shouldFlush = ShouldFlush;}
		void setNewLine(NewLineSwitch ShouldNewLine) noexcept {shouldNewLine = ShouldNewLine;}
		
		void flush(){
			if(stream)
			{
				std::lock_guard<ThreadLib::SpinlockAlign64B> lock(MyStreamLocks::mtx[lockID ? lockID : LOCKID]);
				*stream << oss.rdbuf();
				if(shouldNewLine == NewLineSwitch::on)
					*stream << "\n";
				if(shouldFlush == FlushSwitch::on)
					*stream << std::flush;
			}
			else if(STREAM)
			{
				std::lock_guard<ThreadLib::SpinlockAlign64B> lock(MyStreamLocks::mtx[lockID ? lockID : LOCKID]);
				*STREAM << oss.rdbuf() << std::endl;
				if(shouldNewLine == NewLineSwitch::on)
					*STREAM << "\n";
				if(shouldFlush == FlushSwitch::on)
					*STREAM << std::flush;
			}
			oss.clear();
		}
		
		~OutStream()
		{
			flush();
		}
	};

	using StdOS = OutStream<std::ostream, 0, &std::cout>;
	using ErrOS = OutStream<std::ostream, 0, &std::cerr>;
	using LogOS = OutStream<std::ostream, 0, &std::clog>;

	using FileOS = OutStream<std::ofstream>;
	
	
}



#endif
