#ifndef ERRORCODES_FAILUREWRAPPER_HXX
#define ERRORCODES_FAILUREWRAPPER_HXX

#include "ErrorCodes/CheckedValue.hxx"

namespace ErrorCodes { namespace detail {
    /// @brief Wrapper class to aid automatic conversion in macros
    /// @tparam Code The failure code
    ///
    /// This struct is intended mainly for internal use for the macros to indicate failure
    template <typename Code> struct FailureWrapper {
        FailureWrapper(Code code) : code(code) {}
        operator Code() { return code; }
        template <ErrorInfoTrait Trait, typename T>
            requires std::convertible_to<typename Trait::code_t, Code>
        operator CheckedValue<Trait, T>() {
            return CheckedValue<Trait, T>(failure, code);
        }
        template <ErrorInfoTrait Trait>
            requires std::convertible_to<typename Trait::code_t, Code>
        operator CheckedCode<Trait>() {
            return code;
        }

        Code code;
    };
}} // namespace ErrorCodes::detail

#endif //> !ERRORCODES_FAILUREWRAPPER_HXX