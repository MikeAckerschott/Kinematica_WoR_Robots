#include <iostream>
#include <StdOutTraceFunction.hpp>

namespace Base
{
	void StdOutTraceFunction::trace( const std::string& aText)
	{
		std::cout << aText << std::endl;
	}
} // namespace Base
