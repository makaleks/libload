#ifndef TEST_SHARED_H_INCLUDED
#define TEST_SHARED_H_INCLUDED 0

#ifdef __cplusplus
extern "C" {
#endif

// See (theory)
// https://docs.microsoft.com/en-us/cpp/build/walkthrough-creating-and-using-a-dynamic-link-library-cpp
// https://gcc.gnu.org/wiki/Visibility
// and (practic)
// https://github.com/glfw/glfw/blob/master/include/GLFW/glfw3.h
// for examples about how do deal with Windows headache

// A single define for Windows
#if !defined(_WIN32) && (defined(__WIN32__)\
    || defined(WIN32) || defined(__MINGW32__))
 #define _WIN32
#endif

// Minimal DLL API
#ifndef _WIN32
	// For GCC you can also use
	// __attribute__((visibility("hidden")))
	// and -fvisibility=hidden compilation flag. It is a good 
	// flag, but this example is 'minimal to work'
	#define TEST_SHARED_API
#else
	#ifdef TEST_SHARED_BUILD
		// API to build DLL
		#define TEST_SHARED_API __declspec(dllexport)
	#else
		// API to call DLL
		#define TEST_SHARED_API __declspec(dllimport)
	#endif
#endif

typedef char* (*function_str_str)(char*);

TEST_SHARED_API char* test_shared(char*);

#ifdef __cplusplus
}
#endif

#endif
