#ifndef VLLITE_VECTOR_FUNCTIONAL_H
#define VLLITE_VECTOR_FUNCTIONAL_H

#include <vllite/binary_expr_vector.hpp>
#include <vllite/unary_expr_vector.hpp>

#include <cassert>
#include <functional>

namespace vllite
{

template <typename Underlying, typename Op>
static auto apply(const common_vector<Underlying>& operand, Op op)
{
  return unary_expr_vector(operand.begin(), operand.end(),
                           std::move(op), operand.size());
}

template <typename UnderlyingA, typename UnderlyingB, typename Op>
static auto apply(const common_vector<UnderlyingA>& lhs,
                  const common_vector<UnderlyingB>& rhs, Op op)
{
  assert(lhs.size() == rhs.size());
  return binary_expr_vector(lhs.begin(), lhs.end(), rhs.begin(),
                            rhs.end(), std::move(op), lhs.size());
}

#define OPERATIONS_LIST                                              \
  UNARY_OPERATIONS_LIST                                              \
  BINARY_OPERATIONS_LIST

#define BINARY_OPERATIONS_LIST                                       \
  BINARY_COMBINATIONS(+, std::plus<>)                                \
  BINARY_COMBINATIONS(-, std::minus<>)                               \
  BINARY_COMBINATIONS(*, std::multiplies<>)                          \
  BINARY_COMBINATIONS(/, std::divides<>)                             \
  BINARY_COMBINATIONS(%, std::modulus<>)                             \
  BINARY_COMBINATIONS(&&, std::logical_and<>)                        \
  BINARY_COMBINATIONS(||, std::logical_or<>)                         \
  BINARY_COMBINATIONS(==, std::equal_to<>)                           \
  BINARY_COMBINATIONS(!=, std::not_equal_to<>)                       \
  BINARY_COMBINATIONS(<, std::less<>)                                \
  BINARY_COMBINATIONS(<=, std::less_equal<>)                         \
  BINARY_COMBINATIONS(>, std::greater<>)                             \
  BINARY_COMBINATIONS(>=, std::greater_equal<>)

#define BINARY_COMBINATIONS(OP__, FUNCTOR__)                         \
  BINARY_OPERATION(OP__, FUNCTOR__)                                  \
  BINARY_RIGHT_TYPE_OPERATION(OP__, FUNCTOR__)                       \
  BINARY_LEFT_TYPE_OPERATION(OP__, FUNCTOR__)

#define BINARY_OPERATION(OP__, FUNCTOR__)                            \
  template <typename UnderlyingA, typename UnderlyingB>              \
  auto operator OP__(const common_vector<UnderlyingA>& lhs,          \
                     const common_vector<UnderlyingB>& rhs)          \
  {                                                                  \
    return apply(lhs, rhs, (FUNCTOR__){});                           \
  }

#define BINARY_RIGHT_TYPE_OPERATION(OP__, FUNCTOR__)                 \
  template <typename Underlying, typename T,                         \
            typename = std::enable_if_t<                             \
              std::negation_v<is_common_vector<T>>>>                 \
  auto operator OP__(const common_vector<Underlying>& lhs, T rhs)    \
  {                                                                  \
    return apply(lhs, [ rhs = std::move(rhs),                        \
                        op = (FUNCTOR__){} ](auto&& value) {         \
      return op(std::forward<decltype(value)>(value), rhs);          \
    });                                                              \
  }

#define BINARY_LEFT_TYPE_OPERATION(OP__, FUNCTOR__)                  \
  template <typename T, typename Underlying,                         \
            typename = std::enable_if_t<                             \
              std::negation_v<is_common_vector<T>>>>                 \
  auto operator OP__(T lhs, const common_vector<Underlying>& rhs)    \
  {                                                                  \
    return apply(rhs, [ lhs = std::move(lhs),                        \
                        op = (FUNCTOR__){} ](auto&& value) {         \
      return op(lhs, std::forward<decltype(value)>(value));          \
    });                                                              \
  }

#define UNARY_OPERATIONS_LIST                                        \
  UNARY_OPERATION(-, std::negate<>)                                  \
  UNARY_OPERATION(!, std::logical_not<>)

#define UNARY_OPERATION(OP__, FUNCTOR__)                             \
  template <typename T>                                              \
  auto operator OP__(const common_vector<T>& operand)                \
  {                                                                  \
    return apply(operand, (FUNCTOR__){});                            \
  }

OPERATIONS_LIST

#undef UNARY_OPERATION
#undef UNARY_OPERATIONS_LIST

#undef BINARY_OPERATION
#undef BINARY_LEFT_TYPE_OPERATION
#undef BINARY_RIGHT_TYPE_OPERATION
#undef BINARY_COMBINATIONS
#undef BINARY_OPERATIONS_LIST

#undef OPERATIONS_LIST

} // namespace vllite

#endif // VLLITE_VECTOR_FUNCTIONAL_H
