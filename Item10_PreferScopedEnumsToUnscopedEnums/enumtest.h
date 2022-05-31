enum class Status;

enum Color : std::uint8_t; // Cannot be forward declared without specifying underlying type

void continueProcessing(Status s, Color c);