enum class Status; // scoped enums can be forward declared without specifying underlying type which is default int

enum Color : std::uint8_t; // Cannot be forward declared without specifying underlying type

void continueProcessing(Status s, Color c);