#ifndef POINT_HPP_
#define POINT_HPP_

/*
 * Copyright (c) 2016 Joost Kraaijeveld. See license.txt for details.
 * For information, bug reports and additions send an e-mail to DistributedRobotWorld@Askesis.nl.
 *
 * Author: jkr
 */

#include "Config.hpp"

#include <wx/gdicmn.h>

#include <iostream>

namespace Widgets
{
	/**
	 * @see http://docs.wxwidgets.org/stable/classwx_point
	 */
	typedef wxPoint Point;
} // namespace Widgets
using namespace Widgets;

inline std::ostream& operator<<(std::ostream& os, const Point& aPoint)
{
	return os << "(" << aPoint.x << "," << aPoint.y << ")";
}

#endif // POINT_HPP_
