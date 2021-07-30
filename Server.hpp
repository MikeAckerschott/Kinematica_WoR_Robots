#ifndef SERVER_HPP_
#define SERVER_HPP_

/*
 * Copyright (c) 2021 Joost Kraaijeveld. See license.txt for details.
 * For information, bug reports and additions send an e-mail to jkr@askesis.nl.
 *
 * Author: jkr
 */

#include "Config.hpp"

#include "Session.hpp"
#include "CommunicationService.hpp"

#include <atomic>
#include <iostream>
#include <memory>

namespace Messaging
{
	/*
	 *
	 */
	class Server : public std::enable_shared_from_this< Server >
	{
		public:
			/**
			 *
			 */
			Server(	unsigned short aPort,
					RequestHandlerPtr aRequestHandler);
			/**
			 *
			 */
			virtual ~Server();
			/**
			 *
			 */
			unsigned short getPort() const
			{
				return port;
			}
			/**
			 *
			 */
			void startHandlingRequests()
			{
				handleAccept( nullptr, boost::system::error_code());
			}
			/**
			 *
			 */
			void handleAccept( 	ServerSession* aSession,
								const boost::system::error_code& error)
			{
				try
				{
					if (!error)
					{
						if(!stopAccepting.load())
						{
							// Create the session that will handle the next incoming connection
							ServerSession* session = new ServerSession( requestHandler);
							// Let the acceptor wait for any new incoming connections
							// and let it call server::handle_accept on the happy occasion
							acceptor.async_accept(	session->getSocket(), // @suppress("Method cannot be resolved")
													[this, session](const boost::system::error_code& error)
													{
														handleAccept(session,error);
													});

							// If there is a session, start it up....
							if (aSession)
							{
								aSession->start();
							}
						}else
						{
							TRACE_DEVELOP("Server does not accept any sessions anymore");
						}
					} else
					{
//						TRACE_DEVELOP("handleAccept called with an error");
						if (aSession)
						{
							delete aSession;
						}
						throw std::runtime_error( __PRETTY_FUNCTION__ + std::string( ": ") + error.message());
					}
				}
				catch (std::exception& e)
				{
//					if(stopAccepting)
//					{
//						std::ostringstream os;
//						os << "xxxxxxxxxxxx " << __PRETTY_FUNCTION__ << ": " << e.what() << ", stopAccepting = " << stopAccepting;
//						TRACE_DEVELOP(os.str());
//					}
					// acceptor.async_accept throws an exception when cancelled during stopping the server
					if(!stopAccepting)
					{
						std::ostringstream os;
						os << "************ " << __PRETTY_FUNCTION__ << ": " << e.what() << ", stopAccepting = " << stopAccepting;
						TRACE_DEVELOP(os.str());
					}
				}
			}
			/**
			 *
			 */
			void stopHandlingRequests()
			{
				stopAccepting.store(true);

				timer.expires_from_now(boost::posix_time::seconds(1));
				timer.async_wait([this](const boost::system::error_code& UNUSEDPARAM(e)) // @suppress("Method cannot be resolved")
								 {
									boost::asio::post(	[this]() // @suppress("Invalid arguments")
														{
															acceptor.cancel();
														});
								 });
			}
			/**
			 *
			 */
			bool isAccepting() const
			{
				return stopAccepting;
			}

		private:
			/**
			 *
			 */
			unsigned short port;
			/**
			 * Provides the ability to accept new connections
			 */
			boost::asio::ip::tcp::acceptor acceptor;
			/**
			 *
			 */
			std::atomic<bool> stopAccepting = false;
			/**
			 *
			 */
			RequestHandlerPtr requestHandler;
			/**
			 *
			 */
			boost::asio::deadline_timer timer;
	};
	/**
	 *
	 */
	typedef std::shared_ptr< Server > ServerPtr;
} /* namespace Messaging */

#endif // SERVER_HPP_ 
