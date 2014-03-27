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

namespace Bit
{

	////////////////////////////////////////////////////////////////
	/// \ingroup Network
	/// \brief HTTP protocol class
	///
	/// Documentation:	http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html
	///					http://www.w3.org/Protocols/rfc2616/rfc2616-sec9.html
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
			RequestUriTooLong		= 414,	///< 
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

	private:

	};

}

#endif