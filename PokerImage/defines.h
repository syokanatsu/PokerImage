#include <string>
#include <functional>


#ifdef _WIN32
#ifdef EXPORT_POKERIMAGE
#define POKER_API __declspec(dllexport)
#else
#define POKER_API __declspec(dllimport)
#endif
#else
#define POKER_API
#endif


using OnStartFunc = void (*)(const char*);
using OnCallbackFunc = void (*)(const char*);

using ReqHandleFunc = std::function<int(const std::string&, OnCallbackFunc)>;
