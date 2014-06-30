// Copyright (C) 2013 Jimmie Bergmann - jimmiebergmann@gmail.com
//
// This software is provided 'as-is', without any express or
// implied warranty. In no event will the authors be held
// liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute
// it freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but
//    is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any
//    source distribution.
// ///////////////////////////////////////////////////////////////////////////

#ifndef BIT_NETWORK_HTTP_HPP
#define BIT_NETWORK_HTTP_HPP

#include <Bit/Build.hpp>
#include <Bit/Network/Address.hpp>
#include <Bit/System/Time.hpp>
#include <unordered_map>
#include <sstream>

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup Network
	/// \brief HTTP protocol class
	///
	/// Resources:	http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html
	///				http://www.w3.org/Protocols/rfc2616/rfc2616-sec9.html
	///				http://code.tutsplus.com/tutorials/http-headers-for-dummies--net-8039
	///
	////////////////////////////////////////////////////////////////
	class BIT_API Http
	{

	public:

		////////////////////////////////////////////////////////////////
		/// \brief Method enumeraton
		///
		////////////////////////////////////////////////////////////////
		enum eMethod
		{
			NoMethod,
			Options,	///< Get information about the communication options available.
			Get,		///< Retrieve whatefter information is identified by the "Request-URI"
			Head,		///< Same as get, but does not required message-body.
			Post,		///< Let's the client provide a block of data, for example.
			Put,		///< Create a resource.
			Delete,		///< Delete a resource.
			Trace,		///< ?
			Connect		///< Reserved for use with a proxy.
		};

		////////////////////////////////////////////////////////////////
		/// \brief Status code enumeraton
		///
		////////////////////////////////////////////////////////////////
		enum eCode
		{
			NoCode					= 0,

			// Informational codes
			Continue				= 100, ///< The client should send the remainder of the request
			SwitchingProtocols		= 101, ///< Server will change protocols

			// Successful codes
			Ok						= 200,	///< The request has succeeded.
			Created					= 201,	///< A new resource has been created.
			Accepted				= 202,	///< The request has been accepted, but not compleded yet.
			NonAuthoritative		= 203,	///< The content is a copy from a third party server.
			NoContent				= 204,	///< Fulfilled request, but no body was sent.
			ResetContent			= 205,	///< FUlfilled request, but used agent should reset the document view.
			PartialContent			= 206,	///< The server has fulfilled the partial GET request.

			// Redirection codes
			MultipleChoices			= 300,	///< ?
			MovedPermanently		= 301,	///< Requested resource has been assigned a new parmanent URI.
			Found					= 302,	///< Requested resource resides temporarily under a different URI.
			SeeOther				= 303,	///< Response to the request can be found under a different URI.
			NotModified				= 304,	///< "Access" is allowed, and the requested document(GET) has not been modified.
			UseProxy				= 305,	///< Requested resource must be accessed through the given proxy("Location" field).
			Unused					= 306,	///< Removed status code from previous specification version.
			TemporaryRedirect		= 307,	///< Requested resource resides temporarily under a different URI.

			// Client error codes
			BadRequest				= 400,	///< The request could not be understood by the server because of syntax errors.
			Unauthorized			= 401,	///< The request requires user authentication.
			PaymentRequired			= 402,	///< Reserved for future use.
			Forbidden				= 403,	///< The server understood the request, but refused it.
			NotFound				= 404,	///< The server could not find anything mathing the request-URI.
			MethodNotAllowed		= 405,	///< The method specified in the "Request-Line" is not allowed.
			NotAcceptable			= 406,	///< ?
			ProxyAuthentication		= 407,	///< (Almost like 401) Client must authenticate itself with the proxy.
			RequestTimeout			= 408,	///< Client did not send a request fast enough.
			Conflict				= 409,	///< Request could not be completed due to a conflict with the current statue of the resource.
			Gone					= 410,	///< Requested resource is no longer available.
			LengthRequired			= 411,	///< Server requires the "Content-Length" field, it's missing in the request.
			PreconditionFailed		= 412,	///< Server did not receive a response from it's upstream fast enough.
			RequestEntityTooLarge	= 413,	///<
			RequestUriTooLong		= 414,	///< The requested URI was too long.
			UnsupportedMediaType	= 415,	///<
			RangeNotSatisfiable		= 416,	///<
			ExpectationFailed		= 417,	///<

			// Server error codes
			InternalServerError		= 500,	///< The server could not fulfill the request.
			NotImplemented			= 501,	///< The server does not support the functionality required to fulfill the request.
			BadGateway				= 502,	///< The server received an invalid response from upstream server.
			ServiceUnavailable		= 503,	///< The server is current unable to handle the request due to overloading or maintenance.
			GatewayTimeout			= 504,	///< The server timeout from it's upstream.
			VersionNotSupported		= 505	///< The server does not support the specified http protocol version.
		};

		////////////////////////////////////////////////////////////////
		/// \brief Http packet base class
		///
		/// Used for requests and responses.
		///
		////////////////////////////////////////////////////////////////
		class BIT_API HttpPacket
		{

		public:

			// Friend classes
			friend class Http;

			////////////////////////////////////////////////////////////////
			/// \brief Function for setting a field in the http packet.
			///
			////////////////////////////////////////////////////////////////
			void SetField( const std::string & p_Key, const std::string & p_Content );

			////////////////////////////////////////////////////////////////
			/// \brief Function for getting a field from the http packet.
			///
			////////////////////////////////////////////////////////////////
			const std::string & GetField( const std::string & p_Key ) const;

		protected:

			// Protected typedefs
			typedef std::unordered_map< std::string, std::string> StringMap;

			// Protected variables
			StringMap m_Fields;		///< Http data fields.

		};

		////////////////////////////////////////////////////////////////
		/// \brief Http request class
		///
		////////////////////////////////////////////////////////////////
		class BIT_API Request : public HttpPacket
		{

		public:

			// Friend classes
			friend class Http;

			////////////////////////////////////////////////////////////////
			/// \brief Constructor
			///
			////////////////////////////////////////////////////////////////
			Request(	const eMethod p_Method,
						const std::string & p_Path = "",
						const std::string & p_Protocol = "HTTP/1.1" );

			////////////////////////////////////////////////////////////////
			/// \brief Sets the method
			///
			////////////////////////////////////////////////////////////////
			void SetMethod( const eMethod p_Method );

			////////////////////////////////////////////////////////////////
			/// \brief Sets the path
			///
			////////////////////////////////////////////////////////////////
			void SetPath( const std::string & p_Path );

			////////////////////////////////////////////////////////////////
			/// \brief Sets the protocol
			///
			////////////////////////////////////////////////////////////////
			void SetProtocol( const std::string & p_Protocol );

			////////////////////////////////////////////////////////////////
			/// \brief Gets the method
			///
			////////////////////////////////////////////////////////////////
			eMethod GetMethod( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Gets the path
			///
			////////////////////////////////////////////////////////////////
			const std::string & GetPath( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Gets the protocol
			///
			////////////////////////////////////////////////////////////////
			const std::string & GetProtocol( ) const;

		private:

			// Private variables
			eMethod m_Method;		///< Method used.
			std::string m_Path;		///< The request path.
			std::string m_Protocol;	///< Http version, for example: HTTP/1.1

		};

		////////////////////////////////////////////////////////////////
		/// \brief Http response class
		///
		////////////////////////////////////////////////////////////////
		class BIT_API Response : public HttpPacket
		{

		public:

			// Friend classes
			friend class Http;

			////////////////////////////////////////////////////////////////
			/// \brief Constructor
			///
			////////////////////////////////////////////////////////////////
			Response(	const std::string & p_Protocol = "",
						const eCode p_StatusCode = NoCode,
						const std::string & p_Body = "" );

			////////////////////////////////////////////////////////////////
			/// \brief Sets the protocol
			///
			////////////////////////////////////////////////////////////////
			void SetProtocol( const std::string & p_Protocol );

			////////////////////////////////////////////////////////////////
			/// \brief Sets the status code
			///
			////////////////////////////////////////////////////////////////
			void SetStatusCode( const eCode p_StatusCode );

			////////////////////////////////////////////////////////////////
			/// \brief Sets the body.
			///
			////////////////////////////////////////////////////////////////
			void SetBody( const std::string & p_Body );

			////////////////////////////////////////////////////////////////
			/// \brief Gets the protocol
			///
			////////////////////////////////////////////////////////////////
			const std::string & GetProtocol( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Gets the status code
			///
			////////////////////////////////////////////////////////////////
			eCode GetStatusCode( ) const;

			////////////////////////////////////////////////////////////////
			/// \brief Gets the body.
			///
			////////////////////////////////////////////////////////////////
			const std::string & GetBody( ) const;

		private:

			std::string m_Protocol;
			eCode m_StatusCode;
			std::string m_Body;

		};

		////////////////////////////////////////////////////////////////
		/// \brief Constructor.
		///
		////////////////////////////////////////////////////////////////
		Http(	const Uint16 p_Port = 80,
				const Time & p_Timeout = Time::Infinite );

		////////////////////////////////////////////////////////////////
		/// \brief Sets the port(80 by default)
		///
		////////////////////////////////////////////////////////////////
		void SetPort( const Uint16 p_Port );

		////////////////////////////////////////////////////////////////
		/// \brief Sets the receive timeout
		///
		/// A input value of 0 will disable the timeout functionality.
		///
		/// \param p_Timeout Time in milliseconds.
		///
		////////////////////////////////////////////////////////////////
		void SetTimeout( const Time & p_Timeout );

		////////////////////////////////////////////////////////////////
		/// \brief Send request packet
		///
		/// Send a HTTP request and receive a response from the server.
		///
		/// \param p_Request The request packet to send to the server.
		/// \param p_Response The received response from the server.
		/// \param p_Address The address of the server.
		///
		////////////////////////////////////////////////////////////////
		Bool SendRequest( const Request & p_Request, Response & p_Response, const Address & p_Address );

		////////////////////////////////////////////////////////////////
		/// \brief Parses a HTTP response packet.
		///
		/// \param p_Data The text data to parse into a Packet.
		/// \param p_Response The parsed data packet.
		///
		////////////////////////////////////////////////////////////////
		static Bool ParseResponsePacket( const std::string * p_pData, HttpPacket & p_Packet );

		////////////////////////////////////////////////////////////////
		/// \brief Create a request string out of a request packet.
		///
		/// \param p_Request The request packet.
		/// \param p_StringStream The output stringstream(holding the string).
		///
		////////////////////////////////////////////////////////////////
		static void CreateRequestString( const Request & p_Request, std::stringstream & p_StringStream );

	private:

		// Private variables
		Uint16 m_Port;
		Time m_Timeout;

	};

}

#endif
