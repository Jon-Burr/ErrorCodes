#ifndef ERRORCODES_CHECKEDVALUE_HXX
#define ERRORCODES_CHECKEDVALUE_HXX

#include "ErrorCodes/ErrorInfo.hxx"
#include <concepts>
#include <utility>
#include <variant>

namespace ErrorCodes {
    /// @brief Tag to indicate that the success version of a CheckedValue should be constructed
    struct success_t {};
    inline constexpr success_t success{};
    /// @brief Tag to indicate that the failure version of a CheckedValue should be constructed
    struct failure_t {};
    inline constexpr failure_t failure{};
    /// @brief Object that can either hold a returned value or an error code indicating why a value
    ///        could not be returned
    /// @tparam E The error info trait
    /// @tparam T The value held on success
    template <ErrorInfoTrait E, typename T> class CheckedValue {
    public:
        using info_t = E;
        using type = T;
        using code_t = typename E::code_t;

        /// @brief Create a failed value
        /// @param code The error code
        static CheckedValue<E, T> failure(code_t code) { return CheckedValue(ErrorCodes::failure, code); }

        /// @brief Create a failed value
        /// @param code The error code
        ///
        /// This version is only available if there is no ambiguity between code_t and T
        CheckedValue(code_t code)
            requires(!std::convertible_to<code_t, T> && !std::convertible_to<T, code_t>)
                : CheckedValue(ErrorCodes::failure, code) {}

        /// @brief Create a successful value
        /// @param value The created value
        CheckedValue(const T &value) : CheckedValue(success, value) {}

        /// @brief Create a successful value
        /// @param value The created value
        ///
        /// This version is only available if there is no ambiguity between code_t and T
        CheckedValue(T &&value)
            requires(
                    !std::convertible_to<code_t, T> && !std::convertible_to<T, code_t> &&
                    std::move_constructible<T>)
                : CheckedValue(success, std::move(value)) {}

        /// @brief Create a failed value
        /// @param code The error code
        CheckedValue(failure_t, code_t code) : m_value(std::in_place_index<0>, code) {}

        /// @brief Create a successful value
        /// @param args Arguments to construct the value in place
        template <typename... Args>
        CheckedValue(success_t, Args &&...args)
            requires(std::constructible_from<T, Args && ...>)
                : m_value(std::in_place_index<1>, std::forward<Args>(args)...) {}

        explicit CheckedValue(std::variant<code_t, T> &&value) : m_value(std::move(value)) {}

        /// @brief Is the value valid
        constexpr bool hasValue() const { return m_value.index() == 1; }

        /// @brief The code (success if there is a value)
        code_t code() const { return hasValue() == 1 ? E::success() : std::get<0>(m_value); }

        /// @brief The decoded error string
        std::string decodeError() const { return E::decode(code()); }

        /// @name Get the held value. Will throw an error if there was a failure
        /// @{
        const T &value() const { return std::get<1>(m_value); }
        T &value() { return std::get<1>(m_value); }
        /// @}

        explicit operator bool() const { return hasValue(); }
        const T *operator->() const { return &value(); }
        T *operator->() { return &value(); }
        const T &operator*() const { return value(); }
        T &operator*() { return value(); }

    private:
        std::variant<code_t, T> m_value;
    };

    template <typename E> class CheckedCode : public CheckedValue<E, typename E::code_t> {
        using value_t = std::variant<typename E::code_t, typename E::code_t>;

    public:
        CheckedCode(typename E::code_t code)
                : CheckedValue<E, typename E::code_t>(
                          E::isSuccess(code) ? value_t(std::in_place_index<1>, code)
                                             : value_t(std::in_place_index<0>, code)) {}
    };
} // namespace ErrorCodes

#endif //> !ERRORCODES_CHECKEDVALUE_HXX