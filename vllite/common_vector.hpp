#ifndef VLLITE_COMMON_VECTOR_BASE_HPP_INCLUDED
#define VLLITE_COMMON_VECTOR_BASE_HPP_INCLUDED

#include <iosfwd>
#include <type_traits>

namespace vllite
{

template <typename Underlying> class common_vector
{
public:
  auto size() const noexcept { return self().size(); }

  auto begin() noexcept { return self().begin(); }

  auto end() noexcept { return self().end(); }

  auto begin() const noexcept { return self().begin(); }

  auto end() const noexcept { return self().end(); }

  auto cbegin() const noexcept { return self().cbegin(); }

  auto cend() const noexcept { return self().cend(); }

protected:
  constexpr common_vector() noexcept = default;

private:
  constexpr auto self() const noexcept -> const Underlying&
  {
    return static_cast<const Underlying&>(*this);
  }

  constexpr auto self() noexcept -> Underlying&
  {
    return static_cast<Underlying&>(*this);
  }
};

template <typename> class vector;
template <typename> class ref_vector;
template <typename, typename> class unary_expr_vector;
template <typename, typename, typename> class binary_expr_vector;

template <typename T> struct is_common_vector : std::false_type
{
};

template <typename Underlying>
struct is_common_vector<common_vector<Underlying>> : std::true_type
{
};

template <typename T>
struct is_common_vector<vector<T>> : std::true_type
{
};

template <typename T>
struct is_common_vector<ref_vector<T>> : std::true_type
{
};

template <typename T, typename Op>
struct is_common_vector<unary_expr_vector<T, Op>> : std::true_type
{
};

template <typename T, typename U, typename Op>
struct is_common_vector<binary_expr_vector<T, U, Op>> : std::true_type
{
};

template <typename Underlying>
auto operator<<(std::ostream& os,
                const common_vector<Underlying>& vec) -> std::ostream&
{
  for (const auto& value : vec)
    os << value << ' ';
  return os;
}

} // namespace vllite

#endif // VLLITE_COMMON_VECTOR_BASE_HPP_INCLUDED
