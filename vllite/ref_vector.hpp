#ifndef VLLITE_REF_VECTOR_HPP_INCLUDED
#define VLLITE_REF_VECTOR_HPP_INCLUDED

#include <vllite/common_vector.hpp>
#include <vllite/memory_block.hpp>
#include <vllite/slice.hpp>

#include <stdexcept>
#include <utility>

namespace vllite
{

template <typename T>
class ref_vector : public common_vector<ref_vector<T>>
{
  template <typename> friend class ref_vector;

public:
  using value_type = T;

  using iterator = value_type*;

  using const_iterator = const value_type*;

  using size_type = std::size_t;

  using difference_type = std::ptrdiff_t;

  ref_vector(memory_block<value_type> block)
    : block_{std::move(block)}
  {
  }

  ~ref_vector() = default;

  template <typename U>
  auto operator=(const ref_vector<U>& source) -> ref_vector&
  {
    static_assert(std::is_assignable_v<value_type&, const U&>,
                  "incompatible assignment");

    if (source.size() != block_.size())
      throw std::runtime_error{"sizes mismatch"};

    std::copy(source.begin(), source.end(), begin());
    return *this;
  }

  template <typename U> auto operator=(const U& source) -> ref_vector&
  {
    static_assert(std::is_assignable<value_type&, U>::value,
                  "incompatible assignment");
    std::fill(begin(), end(), source);
    return *this;
  }

  auto operator=(const ref_vector& source) -> ref_vector&
  {
    if (source.size() != block_.size())
      throw std::runtime_error{"sizes mismatch"};

    std::copy(source.begin(), source.end(), this->begin());
    return *this;
  }

  operator ref_vector<const value_type>() const
  {
    return {{data(), size()}};
  }

  auto operator[](size_type i) -> value_type&
  {
    assert(i < size());
    return data()[i];
  }

  auto operator[](size_type i) const -> const value_type&
  {
    assert(i < size());
    return data()[i];
  }

  auto operator[](every_index) -> ref_vector<value_type>
  {
    return *this;
  }

  auto operator[](every_index) const -> ref_vector<const value_type>
  {
    return *this;
  }

  auto operator[](slice s) -> ref_vector<value_type>
  {
    assert(s.start < size());
    assert(s.start + s.size <= size());
    return {{data() + s.start, s.size}};
  }

  auto operator[](slice s) const -> ref_vector<const value_type>
  {
    assert(s.start < size());
    assert(s.start + s.size <= size());
    return {{data() + s.start, s.size}};
  }

  auto begin() noexcept -> iterator { return data(); }
  auto end() noexcept -> iterator { return data() + size(); }

  auto begin() const noexcept -> const_iterator { return cbegin(); }
  auto end() const noexcept -> const_iterator { return cend(); }

  auto cbegin() const noexcept -> const_iterator { return data(); }
  auto cend() const noexcept -> const_iterator
  {
    return data() + size();
  }

  auto size() const noexcept { return block_.size(); }

protected:
  auto data() -> value_type* { return block_.data(); }

  auto data() const -> const value_type* { return block_.data(); }

  ref_vector() = default;

  ref_vector(const ref_vector& source) = default;
  ref_vector(ref_vector&& source) noexcept = default;

  memory_block<value_type> block_;
};

} // namespace vllite

#endif // VLLITE_REF_VECTOR_HPP_INCLUDED
