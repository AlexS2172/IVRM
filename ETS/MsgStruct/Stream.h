#ifndef __STREAM_H__
#define __STREAM_H__

// Utility class to safely parse input stream
class CStreamReader
{
public:
	// Reads string from stream. Updates iterator position
	static std::string GetASCIIZString( const std::vector<BYTE>& stream, std::vector<BYTE>::const_iterator& it )
		{
		std::string str;
		str.reserve(32);

		while( it != stream.end() && *it != 0 )
			{
			str.append( 1, (char)*it );
			it++;
			}

		// We are behind the stream end
		if( it == stream.end() )
			throw _com_error( E_INVALIDARG );

		// Skip zero byte
		it++;

		return str;
		}

	// Reads float from stream. Updates iterator position
	static float GetFloat( const std::vector<BYTE>& stream, std::vector<BYTE>::const_iterator& it )
		{
		if( it + sizeof(float) - 1 == stream.end() )
			throw _com_error( E_INVALIDARG );

		float f = *(float*)it;
		it += sizeof(float);

		return f;
		}

	// Reads BYTE from stream. Updates iterator position
	static BYTE GetBYTE( const std::vector<BYTE>& stream, std::vector<BYTE>::const_iterator& it )
		{
		if( it + sizeof(BYTE) - 1 == stream.end() )
			throw _com_error( E_INVALIDARG );

		BYTE b = *(BYTE*)it;
		it += sizeof(BYTE);

		return b;
		}

	// Reads WORD from stream. Updates iterator position
	static WORD GetWORD( const std::vector<BYTE>& stream, std::vector<BYTE>::const_iterator& it )
		{
		if( it + sizeof(WORD) - 1 == stream.end() )
			throw _com_error( E_INVALIDARG );

		WORD w = *(WORD*)it;
		it += sizeof(WORD);

		return w;
		}

	// Reads DWORD from stream. Updates iterator position
	static DWORD GetDWORD( const std::vector<BYTE>& stream, std::vector<BYTE>::const_iterator& it )
		{
		if( it + sizeof(DWORD) - 1 == stream.end() )
			throw _com_error( E_INVALIDARG );;

		DWORD dw = *(DWORD*)it;
		it += sizeof(DWORD);

		return dw;
		}
};


// Utility class to safely write into output sream
class CStreamWriter
{
public:
	// Writes string
	static void PutASCIIZString( std::vector<BYTE>& response, const char* szStr, bool atEnd = true )
		{
		std::vector<BYTE>::iterator iPos = atEnd ? response.end() : response.begin();

		response.insert( iPos, (BYTE*)szStr, (BYTE*)szStr + strlen(szStr) + 1 );
		}

	// Writes DWORD
	static void PutDWORD( std::vector<BYTE>& response, DWORD dwValue, bool atEnd = true )
		{
		std::vector<BYTE>::iterator iPos = atEnd ? response.end() : response.begin();

		response.insert( iPos, (BYTE*)&dwValue, (BYTE*)&dwValue + sizeof(DWORD) );
		}

	// Writes WORD
	static void PutWORD( std::vector<BYTE>& response, WORD wValue, bool atEnd = true )
		{
		std::vector<BYTE>::iterator iPos = atEnd ? response.end() : response.begin();

		response.insert( iPos, (BYTE*)&wValue, (BYTE*)&wValue + sizeof(WORD) );
		}


	// Writes BYTE
	static void PutBYTE( std::vector<BYTE>& response, BYTE bValue, bool atEnd = true )
		{
		std::vector<BYTE>::iterator iPos = atEnd ? response.end() : response.begin();

		response.insert( iPos, bValue );
		}

	// Writes float
	static void PutFloat( std::vector<BYTE>& response, float fValue, bool atEnd = true )
		{
		std::vector<BYTE>::iterator iPos = atEnd ? response.end() : response.begin();

		response.insert( iPos, (BYTE*)&fValue, (BYTE*)&fValue + sizeof(float) );
		}
};

#endif //__STREAM_H__