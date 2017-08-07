#ifndef VLLITE_SLICE_HPP_INCLUDED
#define VLLITE_SLICE_HPP_INCLUDED

#include <cstddef>

namespace vllite
{

struct slice
{
  slice() = delete;

  constexpr slice(std::size_t start, std::size_t size) noexcept
    : start{start}
    , size{size}
  {
  }

  std::size_t start;
  std::size_t size;
};

struct every_index
{
};

static constexpr auto every = every_index{};

} // namespace vllite

#endif // VLLITE_SLICE_HPP_INCLUDED
