/**
 * @file ErrorInfo.hxx
 * @author Jon Burr (jon.burr@cern.ch)
 * @brief Helper classes for interacting with traits
 * @version 0.1
 * @date 2023-05-10
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef ERRORCODES_ERRORINFO_HXX
#define ERRORCODES_ERRORINFO_HXX

#include <concepts>
#include <string>

namespace ErrorCodes {
    /// @brief Describe a concept for an error trait
    /// @tparam T The trait type itself
    ///
    /// In order to be a valid trait a type must provide
    /// - a code_t type that is the type represent error codes
    /// - a static isSuccess function taking a code_t as input and return true if that code
    ///   represents a success
    /// - a static success function that returns a code_t for which isSuccess returns true
    /// - a decode function that returns a string description of an error code
    template <typename T>
    concept ErrorInfoTrait = requires {
        typename T::code_t;
        { T::success() } -> std::convertible_to<typename T::code_t>;
    } && requires(typename T::code_t code) {
        { T::isSuccess(code) } -> std::convertible_to<bool>;
        { T::decode(code) } -> std::convertible_to<std::string>;
    };
} // namespace ErrorCodes

#endif //> !ERRORCODES_ERRORINFO_HXX