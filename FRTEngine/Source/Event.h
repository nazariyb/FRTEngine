#pragma once
#include <stdint.h>


namespace frt
{
	class Event
	{
	public:
		inline virtual bool IsValid() const noexcept = 0;
	};
}

