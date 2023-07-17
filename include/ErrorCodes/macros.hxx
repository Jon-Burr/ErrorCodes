#ifndef ERRORCODES_MACROS_HXX
#define ERRORCODES_MACROS_HXX

#include "ErrorCodes/CheckedValue.hxx"
#include <concepts>

/// @brief Check the value of an expression and return if it is not a success
/// @param EXP The expression to check
/// @param TRAIT The trait class to use
/// @param STREAM The output stream to use to send failures
/// @param FAIL_RET The value to return on a failure
#define ERRORCODES_CHECK_RET(EXP, TRAIT, STREAM, FAIL_RET)                                         \
    do {                                                                                           \
        TRAIT::code_t code(EXP);                                                                   \
        if (!TRAIT::isSuccess(code)) {                                                             \
            STREAM << "Call '" << #EXP << "' failed with error: " << TRAIT::decode(code)           \
                   << std::endl;                                                                   \
            return FAIL_RET;                                                                       \
        }                                                                                          \
    } while (false)

/// @brief Check the value of an expression and return the code if it is not a success
/// @param EXP The expression to check
/// @param TRAIT The trait class to use
/// @param STREAM The output stream to use to send failures
#define ERRORCODES_CHECK(EXP, TRAIT, STREAM)                                                       \
    do {                                                                                           \
        TRAIT::code_t code(EXP);                                                                   \
        if (!TRAIT::isSuccess(code)) {                                                             \
            STREAM << "Call '" << #EXP << "' failed with error: " << TRAIT::decode(code)           \
                   << std::endl;                                                                   \
            return ErrorCodes::detail::FailureWrapper(code);                                       \
        }                                                                                          \
    } while (false)

/// @brief Check if a CheckedValue contains a correct value and return if not
/// @param VALUE The value to check
/// @param MSG An explanatory message
/// @param STREAM The output stream to use to send failures
/// @param FAIL_RET The value to return on a failure
#define ERRORCODES_CHECK_VALUE_RET(VALUE, MSG, STREAM, FAIL_RET)                                   \
    do {                                                                                           \
        if (!VALUE) {                                                                              \
            STREAM << MSG << " with error " << VALUE.decodeError() << std::endl;                   \
            return FAIL_RET;                                                                       \
        }                                                                                          \
    } while (false)

/// @brief Check if a CheckedValue contains a correct value and return its code if not
/// @param VALUE The value to check
/// @param MSG An explanatory message
/// @param STREAM The output stream to use to send failures
#define ERRORCODES_CHECK_VALUE(VALUE, MSG, STREAM)                                                 \
    do {                                                                                           \
        if (!VALUE) {                                                                              \
            STREAM << MSG << " with error " << VALUE.decodeError() << std::endl;                   \
            return ErrorCodes::detail::FailureWrapper(VALUE.code());                               \
        }                                                                                          \
    } while (false)

#endif //> !ERRORCODES_MACROS_HXX