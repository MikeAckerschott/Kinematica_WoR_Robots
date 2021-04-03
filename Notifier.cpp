#include "Notifier.hpp"
#include <sstream>
#include <typeinfo>
#include <algorithm>
#include "Observer.hpp"
#include "Logger.hpp"


namespace Base
{
	/**
	 *
	 */
	Notifier::Notifier( bool enable /*= true*/) :
								notify( enable)
	{
	}
	/**
	 *
	 */
	Notifier::~Notifier()
	{
	}
	/**
	 *
	 */
	void Notifier::enableNotification( bool enable /* = true */)
	{
		notify = enable;
	}
	/**
	 *
	 */
	void Notifier::disableNotification()
	{
		notify = false;
	}
	/**
	 *
	 */
	bool Notifier::isEnabledForNotification() const
	{
		return notify;
	}
	/**
	 *	The implementation of operator== uses pointer comparison!
	 */
	void Notifier::addObserver( Observer& anObserver)
	{
		if( std::any_of(observers.begin(), observers.end(), [&anObserver](Observer* observer){ return *observer == anObserver;}))
		{
			return;
		}

		observers.push_back( &anObserver);
	}
	/**
	 *	The implementation of operator== uses pointer comparison!
	 */
	void Notifier::removeObserver( Observer& anObserver)
	{
		for (std::vector< Observer* >::iterator i = observers.begin(); i != observers.end(); ++i)
		{
			if (*(*i) == anObserver)
			{
				observers.erase( i);
				break;
			}
		}
	}
	/**
	 *
	 */
	void Notifier::removeAllObservers()
	{
		observers.erase( observers.begin(), observers.end());
	}
	/**
	 *
	 */
	void Notifier::notifyObservers()
	{
		if (notify)
		{
			for (Observer* observer : observers)
			{
				observer->handleNotification();
			}
		}
	}
	/**
	 *
	 */
	std::string Notifier::asString() const
	{
		std::ostringstream os;
		os << typeid(*this).name();
		return os.str();
	}
	/**
	 *
	 */
	std::string Notifier::asDebugString() const
	{
		return asString();
	}
} //namespace Base
