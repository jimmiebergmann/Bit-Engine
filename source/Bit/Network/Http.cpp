// ///////////////////////////////////////////////////////////////////////////
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

#include <Bit/Network/Http.hpp>
#include <Bit/System/SmartMutex.hpp>
#include <algorithm>
#include <iostream>
#include <Bit/System/MemoryLeak.hpp>

// Undefine the SetPort definition for win32 platform
#ifdef BIT_PLATFORM_WINDOWS
#undef SetPort
#endif

namespace Bit
{

	// Global variables
	static const std::string g_EmptyString = "";
	static const std::string g_MethodStrings[Http::Connect + 1] =
	{
		"", "OPTIONS", "GET", "POST", "PUT", "DELETE", "TRACE", "CONNECT"
	};
	static const SizeType g_ResponseBufferSize = 65535 + 32;
	static Uint8 g_ResponseBuffer[g_ResponseBufferSize];

	// Global functions
	static std::string GetWord(const std::string & p_Line, const SizeType p_DataPosition)
	{
		std::string word;

		for (SizeType i = p_DataPosition; i < p_Line.size(); i++)
		{
			if (p_Line[i] == ' ' || p_Line[i] == '\r')
			{
				word.append(p_Line.begin() + p_DataPosition, p_Line.begin() + i);
				break;
			}
		}

		return word;
	}

	static std::string GetLine(const std::string & p_Data, const SizeType p_DataPosition)
	{
		std::string line;

		for (SizeType i = p_DataPosition; i < p_Data.size() - 1; i++)
		{
			if (p_Data[i] == '\r' && p_Data[i + 1] == '\n')
			{
				line.append(p_Data.begin() + p_DataPosition, p_Data.begin() + i);
				break;
			}
		}

		return line;
	}

	static std::string GetLine(const Uint8 * p_pData, const SizeType p_DataSize)
	{
		std::string line;

		for (SizeType i = 0; i < p_DataSize - 1; i++)
		{
			if (p_pData[i] == '\r' && p_pData[i + 1] == '\n')
			{
				line.append(reinterpret_cast<const char*>(p_pData), i);
				break;
			}
		}

		return line;
	}

	static Bool ParseBodyProtocolLine(const std::string & p_Line, Http::Response & p_Response)
	{
		// Parse the protocol
		std::string protocol = GetWord(p_Line, 0);

		// Error check the protocol
		if (protocol.size() != 8 || protocol[0] != 'H' || protocol[1] != 'T' ||
			protocol[2] != 'T' || protocol[3] != 'P')
		{
			return false;
		}

		// Get the status code
		//index += protocol.size( ) + 1;
		const SizeType protocolVersionIndex = protocol.size() + 1;
		std::string version = GetWord(p_Line, protocolVersionIndex);

		// Error check the version
		if (version.size() != 3)
		{
			return false;
		}

		// Set the version
		p_Response.SetStatusCode(static_cast<Http::eCode>(atoi(version.c_str())));

		// Succeeded.
		return true;
	}

	static Bool ParseBodyLine(const std::string & p_Line, Http::Response & p_Response)
	{
		// Get the key.
		std::string key = GetWord(p_Line, 0);
		if (key.size() == 0 || key[key.size() - 1] != ':')
		{
			return false;
		}

		// Get the field data.
		const SizeType contentIndex = key.size() + 1;

		// Make sure there's any data to copy.
		if (contentIndex >= p_Line.size())
		{
			return false;
		}

		// Get the content and error check it.
		std::string content = p_Line.substr(contentIndex, p_Line.size() - contentIndex);
		if (content.size() == 0)
		{
			return false;
		}

		// Transform the key to lowercase
		std::transform(key.begin(), key.end(), key.begin(), ::tolower);

		// Set the field.
		key.resize(key.size() - 1);
		p_Response.SetField(key, content);

		// Succeeded.
		return true;
	}

	static Bool EndOfBodyData(const std::string & p_Data, const SizeType p_DataPosition)
	{
		// Make sure the data size is valid.
		if (p_DataPosition + 2 > p_Data.size())
		{
			return false;
		}

		// This is a new line.
		if (p_Data[p_DataPosition] == '\r' && p_Data[p_DataPosition + 1] == '\n')
		{
			return true;
		}

		// This is not a new line(not end of body data).
		return false;
	}

	// Http packet base class
	void Http::HttpPacket::SetField(const std::string & p_Key, const std::string & p_Content)
	{
		// Check if the field already exists
		StringMap::iterator it = m_Fields.find(p_Key);

		// Add the field if it wasn't found
		if (it == m_Fields.end())
		{
			m_Fields[p_Key] = p_Content;

			/// Terminate the function, we are done here.
			return;
		}

		// Set the already existing field
		it->second = p_Content;
	}

	void Http::HttpPacket::RemoveField(const std::string & p_Key)
	{
		StringMap::iterator it = m_Fields.find(p_Key);

		if (it != m_Fields.end())
		{
			m_Fields.erase(it);
		}
	}

	const std::string & Http::HttpPacket::GetField(const std::string & p_Key) const
	{
		// Find the field in the hash map.
		StringMap::const_iterator it = m_Fields.find(p_Key);

		// Return the field if it was found.
		if (it != m_Fields.end())
		{
			return it->second;
		}

		// Return nothing if it wasn't found.
		return g_EmptyString;
	}

	// Request class
	Http::Request::Request(const eMethod p_Method,
		const std::string & p_Path,
		const Uint16 p_RemotePort,
		const std::string & p_Protocol) :
		m_Method(p_Method),
		m_Path(p_Path),
		m_RemotePort(p_RemotePort ),
		m_Protocol(p_Protocol)
	{
	}

	void Http::Request::SetMethod(const eMethod p_Method)
	{
		m_Method = p_Method;
	}

	void Http::Request::SetPath(const std::string & p_Path)
	{
		m_Path = p_Path;
	}

	void Http::Request::SetProtocol(const std::string & p_Protocol)
	{
		m_Protocol = p_Protocol;
	}

	Http::eMethod Http::Request::GetMethod() const
	{
		return m_Method;
	}

	const std::string & Http::Request::GetPath() const
	{
		return m_Path;
	}

	const std::string & Http::Request::GetProtocol() const
	{
		return m_Protocol;
	}

	// Response class
	Http::Response::Response(const std::string & p_Protocol,
		const eCode p_StatusCode,
		const std::string & p_Body) :
		m_Protocol(p_Protocol),
		m_StatusCode(p_StatusCode),
		m_Body(p_Body)
	{
	}

	void Http::Response::SetProtocol(const std::string & p_Protocol)
	{
		m_Protocol = p_Protocol;
	}

	void Http::Response::SetStatusCode(const eCode p_StatusCode)
	{
		m_StatusCode = p_StatusCode;
	}

	void Http::Response::SetBody(const std::string & p_Body)
	{
		m_Body = p_Body;
	}

	const std::string & Http::Response::GetProtocol() const
	{
		return m_Protocol;
	}

	Http::eCode Http::Response::GetStatusCode() const
	{
		return m_StatusCode;
	}

	const std::string & Http::Response::GetBody() const
	{
		return m_Body;
	}

	// Http class
	Http::Http(const Uint16 p_Port,
		const Time & p_Timeout) :
		m_Port(p_Port),
		m_Timeout(p_Timeout),
		m_FileSize(0),
		m_DownloadedSize(0),
		m_Mutex()
	{
	}

	void Http::SetPort(const Uint16 p_Port)
	{
		m_Port = p_Port;
	}

	void Http::SetTimeout(const Time & p_Timeout)
	{
		m_Timeout = p_Timeout;
	}

	Bool Http::SendRequest(const Request & p_Request, Response & p_Response, const Address & p_Address)
	{
		// Clear some old variables
		m_Mutex.Lock();
		m_FileSize = 0;
		m_DownloadedSize = 0;
		m_Mutex.Unlock();

		// Get the request string
		std::stringstream requestSs;
		CreateRequestString(p_Request, requestSs);
		const Uint32 requestSize = requestSs.str().size();

		// Connect to the server
		Bit::TcpSocket tcp;
		tcp.SetBlocking(false);
		if (tcp.Connect(p_Address, m_Port, m_Timeout, p_Request.m_RemotePort) == false)
		{
			std::cout << "[Http::SendRequest] Could not connect to the server." << std::endl;
			return false;
		}

		// Send the request
		if (tcp.Send(reinterpret_cast<const void *>(requestSs.str().c_str()), requestSize) != requestSize)
		{
			std::cout << "[Http::SendRequest] Could not send the request." << std::endl;
			return false;
		}

		// Receive and handling the response
		// Kepp on trying to receive data until we've downloaded the entire header file.
		// Then download the rest of the file while tracking download speed and download completion.
		Int32 receiveSize = 0;				///< The size of any received message.
		Bool headerComplete = false;		///< Flag for checking if the header is fully downloaded.
		Bool failedHeaderOnce = false;		///< Flag for checking if we've failed the header parsing once.
		Bool firstHeaderLine = true;		///< Indicate if we are handling the first header line(protocol).
		SizeType bodySize = 0;				///< The expected size of the body. 0 if unknown.
		std::string headerData;				///< Temporary string for the header data.
		std::string headerLine;				///< Temporary string for the current header line. From the (headerData) string.
		SizeType currentDataPosition = 0;	///< Store the current data index for (headerData), sometimes we need to read the data in middle of the data buffer.

		// Handle header
		while (headerComplete == false) // Keep on handling the header until we're done/fail.
		{
			// Receive header data
			receiveSize = tcp.Receive(g_ResponseBuffer, g_ResponseBufferSize);

			// Check if we received any data at all.
			if (receiveSize <= 0)
			{
				std::cout << "[Http::SendRequest] Could not receive the header data." << std::endl;
				return false;
			}

			// Append the received data to the temporary header data string.
			headerData.append((char*)g_ResponseBuffer, receiveSize);

			// Go throguh the data line by line.
			while (1)
			{
				// Check if this is the end of the body data( newline at current line ).
				if (EndOfBodyData(headerData, currentDataPosition))
				{
					headerComplete = true;
					break;
				}

				// Get the current header line.
				headerLine = GetLine(headerData, currentDataPosition);

				// Error check the line.
				if (headerLine.size() == 0)
				{
					if (failedHeaderOnce)
					{
						return false;
					}

					// Set the failedHeaderOnce flag and break this loop, try again later when receiving more data.
					failedHeaderOnce = true;
					break;
				}

				// Set the new data position
				currentDataPosition += static_cast<SizeType>(headerLine.size()) + 2; // + 2 because of newline.

				// Parse the line and add it the the response parameter variable.
				if (firstHeaderLine)
				{
					if (ParseBodyProtocolLine(headerLine, p_Response) == false)
					{
						std::cout << "[Http::SendRequest] Could not parse the protocol." << std::endl;
						return false;
					}

					// Mark the first line as done
					firstHeaderLine = false;
				}
				else if (ParseBodyLine(headerLine, p_Response) == false)
				{
					std::cout << "[Http::SendRequest] Could not parse the body data." << std::endl;
					return false;
				}
			}

		}

		if (headerComplete == false)
		{
			// Disconnect and return false
			std::cout << "[Http::SendRequest] Could not receive the entire header." << std::endl;
			tcp.Disconnect();
			return false;

		}

		// Check the way we should download the data.
		// This is chunked data
		if (p_Response.GetField("transfer-encoding") == "chunked")
		{
			ReceiveChunkedBody(tcp, p_Response.m_Body);
		}
		// The size is known.
		else if (p_Response.GetField("content-length").size())
		{
			// Make sure to copy the rest of the "header data" to the body.
			p_Response.m_Body.append(headerData.begin() + currentDataPosition + 2, headerData.end());

			// Receive the rest of the data.
			const std::string sizeString = p_Response.GetField("content-length");
			ReceiveContentLengthBody(tcp, sizeString.size() ? atoi(sizeString.c_str()) : 0, p_Response.m_Body);
		}
		// Unknown size
		else
		{
			// Make sure to copy the rest of the "header data" to the body.
			p_Response.m_Body.append(headerData.begin() + currentDataPosition + 2, headerData.end());

			// Receive the rest of the data.
			ReceiveUnknownLengthBody(tcp, p_Response.m_Body);
		}

		// Disconnect from the server
		tcp.Disconnect();

		// Succeeded
		return true;


		/*
		// Make sure to copy the rest of the "header data" to the body.
		p_Response.m_Body.append(headerData.begin() + currentDataPosition + 2, headerData.end());

		// Get the expected size of the body if possible
		const std::string sizeString = p_Response.GetField("Content-Length");
		const SizeType contentSize = sizeString.size() ? atoi(sizeString.c_str()) : 0;

		// Set the file size, to let the client find out about the progress.
		// Also, set the downloaded size(Rest data from header packet).
		m_Mutex.Lock();
		m_DownloadedSize += static_cast<Uint64>(p_Response.m_Body.size());
		m_FileSize = static_cast<Uint64>(contentSize);
		m_Mutex.Unlock();

		// Download the rest of the data(the body data)
		while (true)
		{
			// Finish the download if we've download the entire file.
			if (contentSize && p_Response.m_Body.size() == contentSize)
			{
				return true;
			}

			// Receive body data.
			receiveSize = tcp.Receive(g_ResponseBuffer, g_ResponseBufferSize);

			// Break if the packet is invalid.( or lost connection )
			if (receiveSize <= 0)
			{
				// The content size is known and the file is not fully downloaded yet, we lost the connection.
				if (contentSize)
				{
					return false;
				}

				// Else, we either lost the connection or the download is finished.
				// It's hard to tell when the content's size is unknown to us.
				return true;
			}

			// Append the body data.
			p_Response.m_Body.append(reinterpret_cast<char*>(g_ResponseBuffer), receiveSize);

			// Increase the downloaded size.
			m_Mutex.Lock();
			m_DownloadedSize += static_cast<Uint64>(receiveSize);
			m_Mutex.Unlock();
		}*/

		
	}

	Uint64 Http::GetFileSize()
	{
		SmartMutex smartMutex(m_Mutex);
		smartMutex.Lock();
		return m_FileSize;
	}

	Uint64 Http::GetDownloadedSize()
	{
		SmartMutex smartMutex(m_Mutex);
		smartMutex.Lock();
		return m_DownloadedSize;
	}

	void Http::CreateRequestString(const Http::Request & p_Request, std::stringstream & p_StringStream)
	{
		// Clear the string stream
		if (p_StringStream.str().size())
		{
			p_StringStream.clear();
		}

		// Add the request line
		p_StringStream << g_MethodStrings[static_cast<SizeType>(p_Request.GetMethod())];
		p_StringStream << " ";
		p_StringStream << p_Request.GetPath();
		p_StringStream << " ";
		p_StringStream << p_Request.GetProtocol();
		p_StringStream << "\r\n";

		// Add the fields
		for (HttpPacket::StringMap::const_iterator it = p_Request.m_Fields.begin();
			it != p_Request.m_Fields.end();
			it++)
		{
			p_StringStream << it->first;
			p_StringStream << ": ";
			p_StringStream << it->second;
			p_StringStream << "\r\n";
		}

		// Add another newline
		p_StringStream << "\r\n";
	}

	bool Http::ReceiveChunkedBody(Bit::TcpSocket & p_Tcp, std::string & p_BodyString)
	{
		SizeType sizeLeftOfChunk = 0;

		// Download the rest of the data(the body data)
		while (true)
		{
			// Receive body data.
			SizeType receiveSize = p_Tcp.Receive(g_ResponseBuffer, g_ResponseBufferSize);
			SizeType dataPos = 0;

			// Loop until we've handled all the received data
			while (dataPos < receiveSize)
			{
				// Used for correcting the size left of chunk.
				SizeType chunkHeaderLength = 0;

				if (sizeLeftOfChunk == 0)
				{
					// Get chunk header
					std::string chunkHeader = GetLine(g_ResponseBuffer + dataPos, receiveSize);

					// Increase data pos and chunk length 
					dataPos += chunkHeader.size() + 2;
					chunkHeaderLength = dataPos;

					// Remove parameters from the chunk header line
					size_t semicolonPos = chunkHeader.find(';');
					if (semicolonPos != std::string::npos)
					{
						chunkHeader.resize(semicolonPos);
					}

					// Error check the chunk header size.
					if (chunkHeader.size() == 0)
					{
						return false;
					}

					// Get the hex chunk size as an integer.
					std::stringstream ss;
					ss << std::hex << chunkHeader;
					ss >> sizeLeftOfChunk;

					// Is this the last chunk, then we are done!
					if (sizeLeftOfChunk == 0)
					{
						return true;
					}
				}

				// Calculate how much data we should append.
				SizeType appendSize = sizeLeftOfChunk < receiveSize ? sizeLeftOfChunk : receiveSize;


				// Append the body data.
				p_BodyString.append(reinterpret_cast<char*>(g_ResponseBuffer)+dataPos, appendSize);

				// Calcualte new size of size left of chunk.
				sizeLeftOfChunk -= appendSize;

				// Move data pos
				dataPos += appendSize;

				// Move to next chunk(add 2) if the chunk is done.
				if (sizeLeftOfChunk == 0)
				{
					dataPos += 2;
				}
			}

			// Correct the size left of chunk
			sizeLeftOfChunk += dataPos - receiveSize;

		}

		return true;
	}

	bool Http::ReceiveContentLengthBody(Bit::TcpSocket & p_Tcp, const SizeType p_ContentSize, std::string & p_BodyString)
	{
		// Download the rest of the data(the body data)
		while (true)
		{
			// Finish the download if we've download the entire file.
			if (p_BodyString.size() == p_ContentSize)
			{
				return true;
			}

			// Receive body data.
			SizeType receiveSize = p_Tcp.Receive(g_ResponseBuffer, g_ResponseBufferSize);

			// Break if the packet is invalid.( or lost connection )
			if (receiveSize <= 0)
			{
				// Else, we either lost the connection or the download is finished.
				// It's hard to tell when the content's size is unknown to us.
				return true;
			}

			// Append the body data.
			p_BodyString.append(reinterpret_cast<char*>(g_ResponseBuffer), receiveSize);

			// Increase the downloaded size.
			m_Mutex.Lock();
			m_DownloadedSize += static_cast<Uint64>(receiveSize);
			m_Mutex.Unlock();
		}

		return true;
	}

	bool Http::ReceiveUnknownLengthBody(Bit::TcpSocket & p_Tcp, std::string & p_BodyString)
	{
		// Download the rest of the data(the body data)
		while (true)
		{
			// Receive body data.
			SizeType receiveSize = p_Tcp.Receive(g_ResponseBuffer, g_ResponseBufferSize);

			// Break if the packet is invalid.( or lost connection )
			if (receiveSize <= 0)
			{
				// We either lost the connection or the download is finished.
				// It's hard to tell when the content's size is unknown to us.
				break;
			}

			// Append the body data.
			p_BodyString.append(reinterpret_cast<char*>(g_ResponseBuffer), receiveSize);

			// Increase the downloaded size.
			m_Mutex.Lock();
			m_DownloadedSize += static_cast<Uint64>(receiveSize);
			m_Mutex.Unlock();
		}

		return true;
	}

}
