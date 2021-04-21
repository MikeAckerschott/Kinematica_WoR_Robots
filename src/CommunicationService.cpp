#include "CommunicationService.hpp"
#include "Server.hpp"
#include "Logger.hpp"
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
	CommunicationService::CommunicationService()
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
		Application::Logger::log( std::string("> ") + __PRETTY_FUNCTION__);

		try
		{
			if(getIOService().stopped())
			{
				Application::Logger::log( "Restarting the io_service");
				getIOService().restart();
			}

			// Create the server object. This must be alive while the program runs
			Messaging::Server server( aPort, aRequestHandler);

			// Run the service until further notice
			getIOService().run();
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
		Application::Logger::log( std::string("< ") + __PRETTY_FUNCTION__);
	}
} // namespace Messaging
