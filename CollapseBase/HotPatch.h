#pragma once

#include "pch.h"

namespace Collapse {


	class PatchFailedException : public std::exception {
	private:
		std::string reason;

	public:
		PatchFailedException(const std::string& identifier, const std::string& cause);

		_NODISCARD virtual char const* what() const
		{
			return reason.c_str();
		}
	};


	class HotPatchX64 {
	private:
		std::string identifier;
		FARPROC target, replacement;

		DWORD protectAttribute;

#pragma pack(push,1)
		struct PatchCode {
			const uint8_t PUSH = 0x68;
			uint32_t lowAddress;
			const uint8_t MOV_DWORD_PTR[4] = { 0xC7 ,0x44 ,0x24 ,0x04 };
			uint32_t highAddress;
			const uint8_t RET = 0xC3;
		} patch, original;
#pragma pack(pop)

		void unprotectMemory();
		void protectMemory();
		void saveOldCode();
		void resumeOldCode();

	public:
		HotPatchX64(const std::string& identifier, FARPROC target, void* replacement);
		
	};

};