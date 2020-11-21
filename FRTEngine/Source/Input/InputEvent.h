#pragma once
#include "Event.h"


namespace frt
{
	class InputEvent : public virtual Event
	{
	public:

		inline virtual bool IsPress() const noexcept = 0;
		inline virtual bool IsRelease() const noexcept = 0;

		inline virtual bool IsValid() const noexcept override = 0;
	};

#define INPUT_EVENT_TYPE_INIT()\
Invalid,\
Press,\
Release,
}
