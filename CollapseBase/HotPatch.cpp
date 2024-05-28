#include "pch.h"
#include "HotPatch.h"

void Collapse::HotPatchX64::unprotectMemory()
{
	if (!VirtualProtect(target, 
		sizeof(PatchCode), 
		PAGE_EXECUTE_READWRITE, 
		&protectAttribute)) {
		throw PatchFailedException(identifier, "unprotect failed");
	}
}

void Collapse::HotPatchX64::protectMemory()
{
	if (!VirtualProtect(target,
		sizeof(PatchCode),
		protectAttribute,
		&protectAttribute)) {
		throw PatchFailedException(identifier, "protect failed");
	}
}

void Collapse::HotPatchX64::saveOldCode()
{
	memcpy(&original, target, sizeof(PatchCode));
}

void Collapse::HotPatchX64::resumeOldCode()
{
	memcpy(target, &original, sizeof(PatchCode));
}

Collapse::HotPatchX64::HotPatchX64(const std::string& identifier, FARPROC target, void* replacement)
	: identifier(identifier), target(target)
{
	uint64_t position = reinterpret_cast<uint64_t>(replacement);
	patch.lowAddress = position >> 32;
	patch.highAddress = position;
}

Collapse::PatchFailedException::PatchFailedException(const std::string& identifier, const std::string& cause)
	: reason("cannot install patch \"" + identifier + "\" ,reason : " + cause)
{
}
