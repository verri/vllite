#include <vllite/vector.hpp>

#include <iostream>

using namespace vllite;

auto main() -> int
{
  const auto v1 = vector(10.0, 3u);
  const auto v2 = vector(v1[every]);
  const auto v3 = vector{1.0, 2.0, 3.0};
  const auto v4 = vector(-v1 + v2 / v3);

  std::cout << vector{v1, v2, v3, v4} << '\n';
}
