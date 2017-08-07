#ifndef VLLITE_VECTOR_HPP_INCLUDED
#define VLLITE_VECTOR_HPP_INCLUDED

#include <vllite/allocator.hpp>
#include <vllite/functional.hpp>
#include <vllite/ref_vector.hpp>

#include <iosfwd>

namespace vllite
{

template <typename T>
class vector : public ref_vector<T>, private allocator<T>
{
  template <typename> friend class vector;

public:
  using value_type = T;

  using iterator = value_type*;

  using const_iterator = const value_type*;

  using size_type = std::size_t;

  using difference_type = std::ptrdiff_t;

  explicit vector(std::size_t size = 0)
    : ref_vector<value_type>{this->allocate(size)}
  {
    try
    {
      this->construct(this->block_);
    }
    catch (...)
    {
      this->deallocate(this->block_);
      throw;
    }
  }

  vector(const value_type& value, std::size_t size)
    : ref_vector<value_type>{this->allocate(size)}
  {
    try
    {
      this->construct(this->block_, value);
    }
    catch (...)
    {
      this->deallocate(this->block_);
      throw;
    }
  }

  vector(std::initializer_list<value_type> values)
    : ref_vector<value_type>{this->allocate(values.size())}
  {
    try
    {
      this->construct(this->block_, values.begin());
    }
    catch (...)
    {
      this->deallocate(this->block_);
      throw;
    }
  }

  template <typename Underlying>
  vector(const common_vector<Underlying>& other)
    : ref_vector<value_type>{this->allocate(other.size())}
  {
    static_assert(
      std::is_constructible_v<T, typename Underlying::value_type>);
    try
    {
      this->construct(this->block_, other.begin());
    }
    catch (...)
    {
      this->deallocate(this->block_);
      throw;
    }
  }

  ~vector() noexcept
  {
    this->destroy(this->block_);
    this->deallocate(this->block_);
  }

  vector(const vector& source)
    : ref_vector<value_type>{this->allocate(source.size())}
  {
    try
    {
      this->construct(this->block_, source.data());
    }
    catch (...)
    {
      this->deallocate(this->block_);
      throw;
    }
  }

  auto operator=(const vector& source) -> vector&
  {
    auto new_block = this->allocate(source.size());

    try
    {
      this->construct(new_block, source.data());
    }
    catch (...)
    {
      this->deallocate(new_block);
      throw;
    }

    this->destroy(this->block_);
    this->deallocate(this->block_);

    this->block_ = new_block;

    return *this;
  }

  vector(vector&& source) noexcept
    : ref_vector<value_type>{std::exchange(source.block_, {})}
  {
  }

  auto operator=(vector&& source) noexcept -> vector&
  {
    std::swap(this->block_, source.block_);
    return *this;
  }

  auto data() noexcept -> value_type* { return this->block_.data(); }

  auto data() const noexcept -> const value_type*
  {
    return this->block_.data();
  }

  using ref_vector<T>::size;
  using ref_vector<T>::begin;
  using ref_vector<T>::end;
  using ref_vector<T>::cbegin;
  using ref_vector<T>::cend;
};

#ifndef __clang__
template <typename T> vector(const T&, std::size_t)->vector<T>;

template <typename T> vector(std::initializer_list<T>)->vector<T>;

template <typename Underlying>
vector(common_vector<Underlying>)
  ->vector<std::decay_t<typename Underlying::value_type>>;
#endif

template <typename T> constexpr auto ref(vector<T>& vec)
{
  return vec[every];
}

template <typename T> constexpr auto ref(const vector<T>& vec)
{
  return vec[every];
}

} // namespace vllite

#endif // VLLITE_VECTOR_HPP_INCLUDED
