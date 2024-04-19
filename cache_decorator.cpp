#include <iostream>
#include <unordered_map>
#include <utility>

template <class T>
size_t HashCombine(size_t seed, const T& value) {
  return seed ^ (std::hash<T>{}(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2));
}

template <class... Types>
struct TupleHash {
  size_t operator()(const std::tuple<Types...>& tuple) const {
    size_t res = 0;
    std::apply([&res](auto&&... args) { ((res = HashCombine(res, args)), ...); }, tuple);
    return res;
  }
};

template <class R, class... Args>
class CacheDecorator {
  using TupleT = std::tuple<Args...>;

 public:
  CacheDecorator(R (*func)(Args...)) : func_(func) {}

  const auto& operator()(Args&&... args) {
    const auto args_tuple = std::make_tuple(std::forward<Args>(args)...);
    auto iter = value_map_.find(args_tuple);
    if (iter != value_map_.end()) {
      return iter->second;
    }
    return value_map_.emplace(args_tuple, func_(std::forward<Args>(args)...)).first->second;
  }

 private:
  R (*func_)(Args...);

  std::unordered_map<TupleT, R, TupleHash<Args...>> value_map_;
};

int Fib(int x) { return x <= 1 ? x : Fib(x - 1) + Fib(x - 2); }

int Combination(int n, int k) {
  return k == 0 || n == k ? 1 : Combination(n - 1, k - 1) + Combination(n - 1, k);
}

int64_t Sum(int n) { return n == 0 ? 0 : n + Sum(n - 1); }

int main() {
  auto fib = CacheDecorator(Fib);
  std::cout << fib(5) << '\n';

  auto c = CacheDecorator(Combination);
  std::cout << c(5, 3) << '\n';

  auto sum = CacheDecorator(Sum);
  std::cout << sum(114514);
}
