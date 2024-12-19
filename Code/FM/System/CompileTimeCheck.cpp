#include <cstdint>

//----------------
// Size check
//----------------
static_assert(sizeof(uintptr_t) == 8, "uintptr_t's size is not 8 byte long");
static_assert(sizeof(intptr_t) == 8, "intptr_t's size is not 8 byte long");
static_assert(sizeof(ptrdiff_t) == 8, "ptrdiff_t's size is not 8 byte long");
