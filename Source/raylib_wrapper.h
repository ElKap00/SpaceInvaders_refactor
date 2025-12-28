#pragma once

// Wrapper header for raylib to suppress all third-party warnings
#include <codeanalysis\warnings.h>
#pragma warning(push)
#pragma warning(disable: ALL_CODE_ANALYSIS_WARNINGS)
#pragma warning(disable: 4005) // macro redefinition
#pragma warning(disable: 4201) // nonstandard extension used: nameless struct/union
#pragma warning(disable: 4244) // conversion warnings
#pragma warning(disable: 6001) // using uninitialized memory
#pragma warning(disable: 6011) // dereferencing NULL pointer
#pragma warning(disable: 6031) // return value ignored
#pragma warning(disable: 6053) // call may not zero-terminate string
#pragma warning(disable: 6054) // string may not be zero-terminated
#pragma warning(disable: 6255) // _alloca indicates failure
#pragma warning(disable: 6262) // excessive stack usage
#pragma warning(disable: 6385) // reading invalid data
#pragma warning(disable: 6386) // buffer overrun
#pragma warning(disable: 6387) // invalid parameter value
#pragma warning(disable: 26495) // uninitialized member variable
#pragma warning(disable: 26812) // prefer enum class over enum
#include "raylib.h"
#pragma warning(pop)
