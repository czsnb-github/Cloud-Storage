#include <bits/stdc++.h>

template <class T>
T FastPow(T base, size_t exponent, const T& one) {
  T res{one};
  for (; exponent > 0; exponent /= 2) {
    if (exponent % 2) {
      res *= base;
    }
    base *= base;
  }
  return res;
}

template <class T>
T FastPow(T base, size_t exponent) {
  return FastPow<T>(base, exponent, 1);
}

template <class T>
struct BiggerInteger {
  using Type = __int128_t;
};

template <>
struct BiggerInteger<int32_t> {
  using Type = int64_t;
};

template <class T, T Mod, bool Safer = true>
class ModInteger final {
  static_assert(Mod > 0);

 public:
  using Type = ModInteger<T, Mod>;
  using BiggerType = ModInteger<typename BiggerInteger<T>::Type, Mod>;

  constexpr ModInteger(T val = 0) : val_(val) { TuneValue(); }

  constexpr ModInteger(const Type& mod_value) : val_(mod_value.val_) {}

  template <class U>
  constexpr ModInteger(ModInteger<U, Mod> mod_value) : ModInteger(mod_value.val()) {}

  constexpr Type& operator=(Type rhs) {
    this->val_ = rhs.val_;
    return *this;
  }

  constexpr void TuneValue() {
    while (val_ < 0) {
      val_ += kMaxMultiple;
    }
    if (val_ >= Mod) {
      val_ %= Mod;
    }
  }

  constexpr T val() const { return val_; }

  constexpr Type operator+(const Type& rhs) const { return val_ + rhs.val_; }

  constexpr Type operator-(const Type& rhs) const { return val_ - rhs.val_; }

  constexpr Type operator*(const Type& rhs) const {
    if constexpr (Safer) {
      if (rhs.val_ != 0 && val_ < std::numeric_limits<T>::max() / rhs.val_) {
        return val_ * rhs.val_;
      }
      return BiggerType(val_) * BiggerType(rhs.val_);
    } else {
      return val_ * rhs.val_;
    }
  }

  constexpr Type operator/(const Type& rhs) const { return *this * (rhs ^ (Mod - 2)); }

  constexpr Type operator^(size_t exponent) const { return FastPow(*this, exponent); }

  constexpr Type& operator+=(const Type& rhs) { return *this = *this + rhs; }

  constexpr Type& operator-=(const Type& rhs) { return *this = *this - rhs; }

  constexpr Type& operator*=(const Type& rhs) { return *this = *this * rhs; }

  constexpr Type& operator/=(const Type& rhs) { return *this = *this / rhs; }

  constexpr Type& operator^=(const Type& rhs) { return *this = *this ^ rhs; }

  template <class OStream>
  friend OStream& operator<<(OStream& output_stream, const Type& mod_value) {
    return output_stream << mod_value.val_;
  }

 private:
  static constexpr T kMaxMultiple = std::numeric_limits<T>::max() / Mod * Mod;

  T val_ = 0;
};

template <class T>
struct AutoExtension {
  using UnaryCallback = std::function<T(int, const T&)>;

  T operator()(size_t n) {
    while (val.size() <= n) {
      val.emplace_back(unary_callback(val.size(), val.back()));
    }
    return val[n];
  }

  std::vector<T> val;
  UnaryCallback unary_callback;
};

template <class T>
class Factorial {
 public:
  Factorial() {
    val_extension_.val = {1};
    val_extension_.unary_callback = [](int n, const T& lst) { return lst * n; };
  }
  T operator()(size_t n) { return val_extension_(n); }

 private:
  AutoExtension<T> val_extension_;
};

struct Prime {
  Prime(int n) : n(n) { GeneratePrime(); }

  void GeneratePrime() {
    is_prime.resize(n + 1, true);
    prime.clear();
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i <= n; ++i) {
      if (is_prime[i]) {
        prime.emplace_back(i);
      }
      for (int j : prime) {
        if (i * j > n) {
          break;
        }
        is_prime[i * j] = false;
        if (i % j == 0) {
          break;
        }
      }
    }
  }

  int n = 0;
  std::vector<bool> is_prime;
  std::vector<int> prime;
};

template <int32_t Mod>
using ModInt = ModInteger<int32_t, Mod>;

template <int64_t Mod>
using ModLL = ModInteger<int64_t, Mod>;

using Z = ModInt<998244353>;

Factorial<Z> Fac;

Z Perm(size_t n, size_t m) { return Fac(n) / Fac(n - m); }

Z Comb(size_t n, size_t m) { return Fac(n) / (Fac(n - m) * Fac(m)); }

template <class T>
constexpr T Ceil(T u, T v) {
  static_assert(std::is_integral_v<T>);
  return (u + v - 1) / v;
}

template <size_t TableBits>
class PopCountCalculator {
 public:
  using PopCountType = uint8_t;

  static constexpr size_t kUpperBound = 1ull << TableBits;

  template <class T>
  constexpr PopCountType operator()(T num) const {
    constexpr size_t kNumDigits = std::numeric_limits<T>::digits;

    PopCountType res = 0;
    PopCountType base = 0;
    for (int i = 0; i < Ceil(kNumDigits, TableBits); ++i) {
      res += one_table_[(num >> base) & (kUpperBound - 1)];
      base += TableBits;
    }
    return res;
  }

 private:
  static constexpr PopCountType PopCount(size_t i) {
    PopCountType res = 0;
    for (; i > 0; i /= 2) {
      res += i % 2;
    }
    return res;
  }

  static constexpr std::array<PopCountType, kUpperBound> CreatePopCountTable() {
    std::array<PopCountType, kUpperBound> res{};
    for (size_t i = 0; i < kUpperBound; ++i) {
      res[i] = PopCount(i);
    }
    return res;
  }

  static constexpr std::array<PopCountType, kUpperBound> one_table_{CreatePopCountTable()};
};

template <class T>
class BitReference {
 public:
  BitReference(T* num, size_t bit_pos) : num_(num), bit_pos_(bit_pos) {}

  operator bool() const { return GetValue(); }

  bool operator~() const { return !GetValue(); }

  BitReference& operator=(bool x) {
    SetValue(x);
    return *this;
  }

  BitReference& operator=(const BitReference& x) {
    SetValue(bool(x));
    return *this;
  }

 private:
  bool GetValue() const { return ((*num_) & (1 << bit_pos_)) != 0; }

  void SetValue(bool x) {
    if (x) {
      *num_ |= 1 << bit_pos_;
    } else {
      *num_ &= ~(1 << bit_pos_);
    }
  }

  T* num_ = nullptr;
  size_t bit_pos_ = 0;
};

template <class T = uint64_t>
class BitSet {
  static_assert(std::is_unsigned_v<T>);

 public:
  BitSet(size_t n) { Resize(n); }

  void Resize(size_t n) {
    n_ = n;
    nums_.resize(Ceil(n_, kDigits));
  }

  void Flip(size_t i) { FlipBit(i % kDigits, &nums_[i / kDigits]); }

  void Flip(size_t l, size_t r) {
    if (l / kDigits == r / kDigits) {
      FlipBits(l % kDigits, r % kDigits, &nums_[l / kDigits]);
      return;
    }
    FlipBits(l % kDigits, kDigits - 1, &nums_[l / kDigits]);
    for (size_t i = l / kDigits + 1; i + 1 <= r / kDigits; ++i) {
      FlipNum(&nums_[i]);
    }
    FlipBits(0, r % kDigits, &nums_[r / kDigits]);
  }

  size_t CountOne(size_t l, size_t r) {
    if (l / kDigits == r / kDigits) {
      return pop_count_calculator_(SubNum(nums_[l / kDigits], l % kDigits, r % kDigits));
    }
    size_t res = pop_count_calculator_(SubNum(nums_[l / kDigits], l % kDigits, kDigits - 1)) +
                 pop_count_calculator_(SubNum(nums_[r / kDigits], 0, r % kDigits));
    for (size_t i = l / kDigits + 1; i + 1 <= r / kDigits; ++i) {
      res += pop_count_calculator_(nums_[i]);
    }
    return res;
  }

  BitReference<T> operator[](size_t index) {
    return BitReference<T>(&nums_[index / kDigits], index % kDigits);
  }

 private:
  static constexpr size_t kDigits = std::numeric_limits<T>::digits;
  static constexpr size_t kPopCountTableDigits = 16;
  static constexpr T kOne{1};
  static constexpr T kZero{0};

  void FlipBit(size_t i, T* num) { *num = (*num) ^ (kOne << i); }

  void FlipBits(size_t l, size_t r, T* num) { *num = *num ^ GetConsecutiveOnes(l, r); }

  void FlipNum(T* num) { *num = ~(*num); }

  constexpr T GetConsecutiveOnes(size_t l, size_t r) const {
    if (l == 0 && r + 1 == kDigits) {
      return ~kZero;
    }
    return ((kOne << (r - l + 1)) - 1) << l;
  }

  constexpr T SubNum(T num, size_t l, size_t r) const {
    return (num & GetConsecutiveOnes(l, r)) >> l;
  }

  std::vector<T> nums_;

  static constexpr PopCountCalculator<kPopCountTableDigits> pop_count_calculator_{};

  size_t n_ = 0;
};

template <class T, class First, class... Rest>
auto CreateVector(First first_size, Rest... rest_sizes) {
  static_assert(std::is_convertible_v<First, std::size_t>);

  if constexpr (sizeof...(Rest) == 0) {
    return std::vector<T>(first_size);
  } else {
    auto sub_vector = CreateVector<T>(rest_sizes...);
    return std::vector<decltype(sub_vector)>(first_size, sub_vector);
  }
}

template <class T>
class MemorySlice {
 public:
  MemorySlice(T* start, int size, int step = 1) : start_(start), size_(size), step_(step) {}

  T& operator[](int index) const { return *(start_ + index * step_); }

 private:
  T* start_ = nullptr;
  const int size_ = 0;
  const int step_ = 1;
};

template <class T>
class Matrix {
 public:
  Matrix(int n, int m) : n_(n), m_(m) { mat_.resize(n * m); }

  template <class... Args>
  constexpr Matrix(int n, int m, Args&&... args) : n_(n), m_(m) {
    mat_.reserve(n * m);
    (mat_.emplace_back(std::forward<Args>(args)), ...);
  }

  Matrix operator*(const Matrix& rhs) const {
    Matrix res(n_, rhs.m_);
    for (int i = 0; i < res.n_; ++i) {
      for (int j = 0; j < res.m_; ++j) {
        for (int k = 0; k < m_; ++k) {
          res[i][j] += (*this)[i][k] * rhs[k][j];
        }
      }
    }
    return res;
  }

  Matrix& operator*=(const Matrix& rhs) { return *this = *this * rhs; }

  Matrix operator^(size_t exponent) const { return FastPow(*this, exponent, One(n_)); }

  MemorySlice<T> operator[](int row_index) { return MemorySlice(mat_.data() + row_index * m_, m_); }
  MemorySlice<const T> operator[](int row_index) const {
    return MemorySlice(mat_.data() + row_index * m_, m_);
  }

  static Matrix One(int n) {
    Matrix res(n, n);
    for (int i = 0; i < n; ++i) {
      res[i][i] = 1;
    }
    return res;
  }

  int n() const { return n_; }
  int m() const { return m_; }

 private:
  int n_ = 0;
  int m_ = 0;
  std::vector<T> mat_;
};

template <int n, int m, class... Args>
constexpr auto MakeMatrix(Args&&... args) {
  static_assert(n * m == sizeof...(Args));
  using Type = typename std::tuple_element_t<0, std::tuple<Args...>>;
  static_assert((std::is_same_v<Type, Args> && ...));

  return Matrix<Type>(n, m, std::forward<Args>(args)...);
}

int main() {
  auto a = MakeMatrix<2, 2>(1, 1, 1, 0);
  auto b = MakeMatrix<2, 1>(1, 1);
  for (int i = 0; i <= 10; ++i) {
    std::cout << ((a ^ i) * b)[0][0] << '\n';
  }
}
