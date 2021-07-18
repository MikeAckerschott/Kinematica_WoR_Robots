#include "CommunicationService.hpp"

#include "Logger.hpp"
#include "Server.hpp"

#include <iostream>

namespace Messaging
{
	/**
	 *
	 */
	/* static */CommunicationService& CommunicationService::getCommunicationService()
	{
		static CommunicationService communicationService;
		return communicationService;
	}
	/**
	 *
	 */
	boost::asio::io_service& CommunicationService::getIOService()
	{
		return io_service;
	}
	/**
	 *
	 */
	void CommunicationService::runRequestHandler( 	RequestHandlerPtr aRequestHandler,
													unsigned short aPort /* = 12345*/)
	{
		std::thread newRequestHandlerThread( [this,aRequestHandler,aPort]
											 {
												runRequestHandlerWorker(aRequestHandler,aPort);
											 });
		newRequestHandlerThread.detach();
		requestHandlerThread.swap( newRequestHandlerThread);
	}
	/**
	 *
	 */
	void CommunicationService::stop()
	{
		server->stop();
	}
	/**
	 *
	 */
	CommunicationService::CommunicationService() : server(nullptr)
	{
	}
	/**
	 *
	 */
	CommunicationService::~CommunicationService()
	{
	}
	/**
	 *
	 */
	void CommunicationService::runRequestHandlerWorker(	RequestHandlerPtr aRequestHandler,
														unsigned short aPort)
	{
		try
		{
			if(io_service.stopped())
			{
				io_service.restart();
			}

			// Create the server object. This must be alive while the program communicates
			Messaging::Server theServer( aPort, aRequestHandler);
			server = &theServer;

			// Run the service until further notice
			io_service.run();
		}
		catch (std::exception& e)
		{
			Application::Logger::log( __PRETTY_FUNCTION__ + std::string(": ") + e.what());
			std::cerr << __PRETTY_FUNCTION__ << ": " << e.what() << std::endl;
		}
		catch (...)
		{
			Application::Logger::log( __PRETTY_FUNCTION__ + std::string(": unknown exception"));
			std::cerr << __PRETTY_FUNCTION__ << ": unknown exception" << std::endl;
		}
	}
} // namespace Messaging
