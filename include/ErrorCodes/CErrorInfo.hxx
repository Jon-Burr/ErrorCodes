#ifndef ERRORCODES_CERRORINFO_HXX
#define ERRORCODES_CERRORINFO_HXX

#include "ErrorCodes/CheckedValue.hxx"
#include <cstring>

namespace ErrorCodes {
    /// @brief Trait class for c error codes
    struct CErrorInfo {
        using code_t = int;
        static constexpr code_t success() { return 0; }
        static constexpr bool isSuccess(code_t code) { return code >= 0; }
        static std::string decode(code_t code) {
            return isSuccess(code) ? std::string("success") : std::strerror(errno);
        }
    };

    using CheckedCCode = CheckedCode<CErrorInfo>;
    template <typename T> using CheckedCValue = CheckedValue<CErrorInfo, T>;
} // namespace ErrorCodes

#endif //> !ERROCODES_CERRORINFO_HXX