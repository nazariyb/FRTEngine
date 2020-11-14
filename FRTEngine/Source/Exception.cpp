#include "Exception.h"
#include <sstream>


namespace frt
{
	using std::ostringstream;
	using std::endl;

	Exception::Exception(int line, const char* file) noexcept
		: line(line), file(file)
	{}

	const char* Exception::What() const noexcept
	{
		return what();
	}

	const char* Exception::what() const noexcept
	{
		ostringstream oss;
		oss << GetType() << endl << GetOriginString();
		whatBuffer = oss.str();
		return whatBuffer.c_str();
	}

	const char* Exception::GetType() const noexcept
	{
		return "FRT Base Exception";
	}

	int Exception::GetLine() const noexcept
	{
		return line;
	}

	const std::string& Exception::GetFile() const noexcept
	{
		return file;
	}

	std::string Exception::GetOriginString() const noexcept
	{
		ostringstream oss;
		oss << "[File]: " << file << endl
			<< "[Line]: " << line;
		return oss.str();
	}
}