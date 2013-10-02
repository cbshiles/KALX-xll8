// ensure.h - Like assert() but throws a runtime error.
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
#pragma once
#include <stdexcept>

// #ifndef NENSURE
#ifndef ensure

#define ENSURE_HASH_(x) #x
#define ENSURE_STRZ_(x) ENSURE_HASH_(x)
#define ENSURE_FILE "file: " __FILE__
#define ENSURE_FUNC "function: " __FUNCTION__
#define ENSURE_LINE "line: " ENSURE_STRZ_(__LINE__)
#define ENSURE_SPOT ENSURE_FILE "\n" ENSURE_LINE "\n" ENSURE_FUNC
#define ensure(e) if (!(e)) {throw std::runtime_error(ENSURE_SPOT "\nensure: \"" #e "\" failed");}

#endif // ensure
// #endif