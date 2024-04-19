#include <bits/stdc++.h>

#define CHECK(expr) assert(expr)

namespace io {

constexpr size_t kInputBufferSize = 1 << 20;
constexpr size_t kOutputBufferSize = 1 << 20;
constexpr size_t kTemporaryBufferSize = 1 << 10;

constexpr double kUsedMemoryInMB =
    (kInputBufferSize + kOutputBufferSize + kTemporaryBufferSize) / 1024. / 1024.;

char input_buffer[kInputBufferSize];
char* current_input_pos = input_buffer;
char* input_end_pos = input_buffer;
char ch;

char output_buffer[kInputBufferSize];
char* current_output_pos = output_buffer;

char temporary_buffer[kTemporaryBufferSize];

// Input Functions

char GetChar() {
  if (current_input_pos == input_end_pos) {
    input_end_pos = input_buffer + std::fread(input_buffer, 1, kInputBufferSize, stdin);
    current_input_pos = input_buffer;
  }
  CHECK(current_input_pos < input_end_pos);
  return *current_input_pos++;
}

char ReadChar() {
  while (std::isspace(ch = GetChar())) {
  }
  return ch;
}

std::string ReadString() {
  std::string str;
  char* temp_buffer_pos = temporary_buffer;
  *temp_buffer_pos++ = ReadChar();
  while (!std::isspace(ch = GetChar())) {
    if (temp_buffer_pos == temporary_buffer + kTemporaryBufferSize) {
      str.append(temporary_buffer, kTemporaryBufferSize);
      temp_buffer_pos = temporary_buffer;
    }
    *temp_buffer_pos++ = ch;
  }
  str.append(temporary_buffer, temp_buffer_pos - temporary_buffer);
  return str;
}

template <class T>
T ReadUnsignedNumber() {
  ch = ReadChar();
  CHECK(std::isdigit(ch));
  T num = ch - '0';
  while (std::isdigit(ch = GetChar())) {
    num = num * 10 + (ch - '0');
  }
  return num;
}
auto ReadUint32 = ReadUnsignedNumber<uint32_t>;
auto ReadUint64 = ReadUnsignedNumber<uint64_t>;

template <class T>
T ReadSignedNumber() {
  bool negative = false;
  int num = 0;
  if ((ch = ReadChar()) == '-') {
    negative = true;
  } else {
    CHECK(std::isdigit(ch));
    num = ch - '0';
  }
  while (std::isdigit(ch = GetChar())) {
    num = num * 10 + (ch - '0');
  }
  return negative ? -num : num;
}
auto ReadInt32 = ReadSignedNumber<int32_t>;
auto ReadInt64 = ReadSignedNumber<int64_t>;

// Output Functions

void Flush() {
  std::fwrite(output_buffer, 1, current_output_pos - output_buffer, stdout);
  current_output_pos = output_buffer;
}

void PrintToBuffer(const char* src, size_t size) {
  if (current_output_pos + size > output_buffer + kOutputBufferSize) {
    Flush();
  }
  std::memcpy(current_output_pos, src, size);
  current_output_pos += size;
}

void PutChar(char ch) { PrintToBuffer(&ch, 1); }

void Write(int x) {
  char* temp_buffer_pos = temporary_buffer + kTemporaryBufferSize;
  bool negative = false;
  if (x < 0) {
    CHECK(x != std::numeric_limits<int>::min());
    negative = true;
    x = -x;
  }
  while (x >= 10) {
    *--temp_buffer_pos = x % 10 + '0';
    x /= 10;
  }
  *--temp_buffer_pos = x + '0';
  if (negative) {
    *--temp_buffer_pos = '-';
  }
  PrintToBuffer(temp_buffer_pos, temporary_buffer + kTemporaryBufferSize - temp_buffer_pos);
}

void Write(std::string_view str_view) { PrintToBuffer(str_view.data(), str_view.size()); }

template <class T>
void Writeln(T value) {
  Write(value);
  PutChar('\n');
}

}  // namespace io

int main() { std::cout << io::ReadString(); }
