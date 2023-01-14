#pragma once

namespace GHUD {
	using uint8 = unsigned char;
	using int8 = signed char;

	using uint16 = unsigned short;
	using int16 = signed short;

	using uint32 = unsigned int;
	using int32 = signed int;

#ifdef _MSC_VER
	using uint64 = unsigned long long;
	using int64 = signed long long;
#else
	using uint64 = unsigned long;
	using int64 = signed long;
#endif
}