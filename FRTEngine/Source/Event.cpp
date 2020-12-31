#include "Event.h"


namespace frt
{
    void Event::Invoke() noexcept
    {
        for (auto listener : listeners)
        {
            listener(this);
        }
    }

    Event& Event::operator+=(std::function<void(Event*)> listener)
    {
        listeners.push_back(listener);
        return *this;
    }
}
