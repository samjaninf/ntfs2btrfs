module;

#include <string>
#include <memory>

#ifdef _WIN32
#include <windows.h>
#endif

export module win32;

using namespace std;

#ifdef _WIN32

export class last_error : public exception {
public:
    last_error(string_view function, int le) {
        string nice_msg;

        {
            char* fm;

            if (FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr,
                               le, 0, reinterpret_cast<LPSTR>(&fm), 0, nullptr)) {
                try {
                    string_view s = fm;

                    while (!s.empty() && (s[s.length() - 1] == u'\r' || s[s.length() - 1] == u'\n')) {
                        s.remove_suffix(1);
                    }

                    nice_msg = s;
                } catch (...) {
                    LocalFree(fm);
                    throw;
                }

                LocalFree(fm);
            }
        }

        msg = string(function) + " failed (error " + to_string(le) + (!nice_msg.empty() ? (", " + nice_msg) : "") + ").";
    }

    const char* what() const noexcept {
        return msg.c_str();
    }

private:
    string msg;
};

class handle_closer {
public:
    typedef HANDLE pointer;

    void operator()(HANDLE h) {
        if (h == INVALID_HANDLE_VALUE)
            return;

        CloseHandle(h);
    }
};

export using unique_handle = unique_ptr<HANDLE, handle_closer>;

#endif
