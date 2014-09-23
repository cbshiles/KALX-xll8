// defines.h - Excel definitions
// Copyright (c) KALX, LLC. All rights reserved. No warranty is made.
// included by xll.h

/// Use #pragma XLLEXPORT in function body instead of DEF file.
/// !!! use constexpr and static_assert to detect if decorated name is stdcall !!!
// constexpr bool is_stdcall(const char* f) { return strchr(f, "@"); } ???
#define XLLEXPORT comment(linker, "/export:" __FUNCDNAME__ "=" __FUNCTION__)

#ifdef XLL_EXPORTS
#define XLL_DLLIMPEXP __declspec(dllexport)
#define XLL_EXTERN
#else
#define XLL_DLLIMPEXP __declspec(dllimport)
#define XLL_EXTERN extern
#endif

// 64-bit uses different name decoration
#ifdef _M_X64 
#define XLL_DECORATE(s,n) s
#define XLL_DECORATE12(s,n) s
#define XLL_X64(x) x
#define XLL_X32(x)
#else
#define XLL_DECORATE(s,n) "_" s "@" #n
#define XLL_DECORATE12(s,n) L"_" s L"@" L#n
#define XLL_X64(x)	
#define XLL_X32(x) x
#endif 

// Used for single source dual use.
#ifdef EXCEL12
#define X_(f) f##12
#define TX_(s) _T(s) _T("12")
#define R_(...) L ## #__VA_ARGS__
#define ExcelX Excel12
#define ExcelXv Excel12v
#else
#define X_(f) f
#define TX_(s) s
// poor man's raw string literal (no commas or parens allowed)
#define R_(...) #__VA_ARGS__
#define ExcelXv Excel4v
#endif

typedef X_(XLOPER) XLOPERX;
typedef X_(XLOPER)* LPXLOPERX;
typedef X_(XLREF) XLREFX;
typedef X_(XLREF)* LPXLREFX;

#define XLL_DECORATEX X_(XLL_DECORATE)

// Excel C data types for xlfRegister/AddIn.
#define XLL_BOOL     "A"  // short int used as logical
#define XLL_DOUBLE   "B"  // double
#define XLL_CSTRING  "C"  // char * to C style NULL terminated string (up to 255 characters)
#define XLL_PSTRING  "D"  // unsigned char * to Pascal style byte counted string (up to 255 characters)
#define XLL_DOUBLE_  "E"  // pointer to double
#define XLL_CSTRING_ "F"  // reference to a null terminated string
#define XLL_PSTRING_ "G"  // reference to a byte counted string
#define XLL_USHORT   "H"  // unsigned 2 byte int
#define XLL_WORD     "H"  // unsigned 2 byte int
#define XLL_SHORT    "I"  // signed 2 byte int
#define XLL_LONG     "J"  // signed 4 byte int
#define XLL_FP       "K"  // pointer to struct FP
#define XLL_BOOL_    "L"  // reference to a boolean
#define XLL_SHORT_   "M"  // reference to signed 2 byte int
#define XLL_LONG_    "N"  // reference to signed 4 byte int
#define XLL_LPOPER   "P"  // pointer to OPER struct (never a reference type).
#define XLL_LPXLOPER "R"  // pointer to XLOPER struct
// Modify add-in function behaviour.
#define XLL_VOLATILE "!"  // called every time sheet is recalced
#define XLL_UNCALCED "#"  // dereferencing uncalced cells returns old value
#define XLL_VOID ">"	// in-place function.
#define XLL_THREAD_SAFE "" // declares function to be thread safe
#define XLL_CLUSTER_SAFE ""	// declares function to be cluster safe
#define XLL_ASYNCHRONOUS ""	// declases function to be asynchronous
 
// Extensions
#define XLL_HANDLE XLL_DOUBLE

#define XLL_BOOL12     L"A"  // short int used as logical
#define XLL_DOUBLE12   L"B"  // double
#define XLL_CSTRING12  L"C%" // XCHAR* to C style NULL terminated unicode string
#define XLL_PSTRING12  L"D%" // XCHAR* to Pascal style byte counted unicode string
#define XLL_DOUBLE_12  L"E"  // pointer to double
#define XLL_CSTRING_12 L"F%" // reference to a null terminated unicode string
#define XLL_PSTRING_12 L"G%" // reference to a byte counted unicode string
#define XLL_USHORT12   L"H"  // unsigned 2 byte int
#define XLL_WORD12     L"J"  // signed 4 byte int
#define XLL_SHORT12    L"I"  // signed 2 byte int
#define XLL_LONG12     L"J"  // signed 4 byte int
#define XLL_FP12       L"K%" // pointer to struct FP
#define XLL_BOOL_12    L"L"  // reference to a boolean
#define XLL_SHORT_12   L"M"  // reference to signed 2 byte int
#define XLL_LONG_12    L"N"  // reference to signed 4 byte int
#define XLL_LPOPER12   L"Q"  // pointer to OPER struct (never a reference type).
#define XLL_LPXLOPER12 L"U"  // pointer to XLOPER struct
// Modify add-in function behaviour.
#define XLL_VOLATILE12 L"!"  // called every time sheet is recalced
#define XLL_UNCALCED12 L"#"  // dereferencing uncalced cells returns old value
#define XLL_THREAD_SAFE12 L"$" // declares function to be thread safe
#define XLL_CLUSTER_SAFE12 L"&"	// declares function to be cluster safe
#define XLL_ASYNCHRONOUS12 L"X"	// declases function to be asynchronous
#define XLL_VOID12     L">"	// return type to use for asynchronous functions

#define XLL_HANDLE12 XLL_DOUBLE12

#define XLL_BOOLX     X_(XLL_BOOL)
#define XLL_DOUBLEX   X_(XLL_DOUBLE)
#define XLL_CSTRINGX  X_(XLL_CSTRING)
#define XLL_PSTRINGX  X_(XLL_PSTRING)
#define XLL_DOUBLE_X  X_(XLL_DOUBLE_)
#define XLL_CSTRING_X X_(XLL_CSTRING_)
#define XLL_PSTRING_X X_(XLL_PSTRING_)
#define XLL_USHORTX   X_(XLL_USHORT)
#define XLL_WORDX     X_(XLL_WORD)
#define XLL_SHORTX    X_(XLL_SHORT)
#define XLL_LONGX     X_(XLL_LONG)
#define XLL_FPX       X_(XLL_FP)
#define XLL_BOOL_X    X_(XLL_BOOL_)
#define XLL_SHORT_X   X_(XLL_SHORT_)
#define XLL_LONG_X    X_(XLL_LONG_)
#define XLL_LPOPERX   X_(XLL_LPOPER)
#define XLL_LPXLOPERX X_(XLL_LPXLOPER)
#define XLL_VOLATILEX X_(XLL_VOLATILE)
#define XLL_UNCALCEDX X_(XLL_UNCALCED)
#define XLL_THREAD_SAFEX X_(XLL_THREAD_SAFE)
#define XLL_CLUSTER_SAFEX X_(XLL_CLUSTER_SAFE)
#define XLL_VOIDX X_(XLL_VOID)

#define XLL_HANDLEX X_(XLL_HANDLE)

// Convenience wrappers for Excel calls
#define XLL_XLF(fn, ...) xll::Excel<XLOPERX>(xlf##fn, __VA_ARGS__)
#define XLL_XLC(fn, ...) xll::Excel<XLOPERX>(xlc##fn, __VA_ARGS__)
#define XLL_XL_(fn, ...) xll::Excel<XLOPERX>(xl##fn, __VA_ARGS__)

#ifdef XLL_EXPORTS
#define DECLSPEC_IMPEXP //__declspec(dllexport)
#else
#define DECLSPEC_IMPEXP __declspec(dllimport)
#endif

#include "traits.h"
