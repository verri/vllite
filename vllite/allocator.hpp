#ifndef VLLITE_ALLOCATOR_HPP_INCLUDED
#define VLLITE_ALLOCATOR_HPP_INCLUDED

#include <vllite/memory_block.hpp>

#include <memory>
#include <type_traits>

namespace vllite
{

template <typename T> struct allocator
{
  using value_type = T;
  using storage_type =
    std::aligned_storage_t<sizeof(value_type), alignof(value_type)>;
  using block_type = memory_block<value_type>;

  auto allocate(std::size_t size) const -> block_type
  {
    return {reinterpret_cast<value_type*>(new storage_type[size]),
            size};
  }

  auto deallocate(block_type block) const noexcept -> void
  {
    delete[] reinterpret_cast<storage_type*>(block.data());
  }

  auto construct(block_type block) const
    noexcept(std::is_nothrow_constructible_v<value_type>) -> void
  {
    std::uninitialized_value_construct_n(block.data(), block.size());
  }

  auto construct(block_type block, const value_type& value) const
    noexcept(std::is_nothrow_copy_constructible_v<value_type>) -> void
  {
    std::uninitialized_fill_n(block.data(), block.size(), value);
  }

  template <
    typename It,
    typename R = typename std::iterator_traits<It>::value_type>
  auto construct(block_type block, It begin) const
    noexcept(noexcept(new (std::declval<void*>()) value_type(*begin)))
      -> void
  {
    static_assert(
      std::is_constructible<value_type, decltype(*begin)>());
    std::uninitialized_copy_n(begin, block.size(), block.data());
  }

  auto destroy(block_type block) const noexcept -> void
  {
    std::destroy_n(block.data(), block.size());
  }
};

} // namespace vllite

#endif // VLLITE_ALLOCATOR_HPP_INCLUDED
