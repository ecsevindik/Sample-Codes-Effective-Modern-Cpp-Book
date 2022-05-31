#include <cstdint>

enum class Status { good = 0,
                    failed = 1,
                    incomplete = 100,
                    corrupt = 200,
                    audited = 500,
                    indeterminate = 1000
};

enum Color : std::uint8_t { pink, brown, cyan };