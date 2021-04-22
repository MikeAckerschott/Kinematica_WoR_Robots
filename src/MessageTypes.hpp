#ifndef MESSAGETYPES_HPP_
#define MESSAGETYPES_HPP_

namespace Messaging {

/**
 * @name The types of messages usable in the application
 */
//@{
enum MessageType
{
	GenericRequest,
	GenericResponse,
	StopCommunicatingRequest,
	StopCommunicatingResponse,
	EchoRequest,
	EchoResponse
};
//@}

} /* namespace Messaging */

#endif // MESSAGETYPES_HPP_
