#pragma once

#include <stdbool.h> 

typedef void* Pointer;
typedef unsigned int uint;
typedef void (*DestroyFunc)(Pointer value);
typedef int (*CompareFunc)(Pointer a, Pointer b);