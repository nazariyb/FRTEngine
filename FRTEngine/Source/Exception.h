#pragma once
#include <exception>
#include <string>


namespace frt
{
	class Exception : public std::exception
	{
	public:
		Exception(int line, const char* file) noexcept;
		const char* What() const noexcept;
		virtual const char* GetType() const noexcept;
		int GetLine() const noexcept;
		const std::string& GetFile() const noexcept;
		std::string GetOriginString() const noexcept;

	private:
		int line;
		std::string file;

	protected:
		mutable std::string whatBuffer;
		virtual const char* what() const noexcept override;
	};
}

