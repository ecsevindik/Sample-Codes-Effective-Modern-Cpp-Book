
/**
• Lambdas are more readable, more expressive, and may be more efficient than
using std::bind.
• In C++11 only, std::bind may be useful for implementing move capture or
for binding objects with templatized function call operators.
 */

#include <vector>
#include <functional>

#include "Chronometer.h"

using namespace std::chrono;
using namespace std::literals;
using namespace std::placeholders;

using Time = std::chrono::steady_clock::time_point;
using Duration = std::chrono::steady_clock::duration;
enum class Sound { Beep, Siren, Whistle };
enum class Volume { Normal, Loud, LoudPlusPlus };

void SetAlarm1(Time t, Sound s, Duration d) {
    std::cout << "SetAlarm1 is called" << std::endl;
    return;
}
void lambdaAndBindTest() {

    std::cout << "lambdaAndBindTest" << std::endl;
    // Lambda implementation
    auto setSoundL = [](Sound s) {
        SetAlarm1(steady_clock::now() + 1h, s, 30s);
    };

    // Bind implementation
    auto setSoundBWrong = std::bind(SetAlarm1, steady_clock::now() + 1h, _1, 30s); // Wrong because steady_clock evaluated when bind is called.
    auto setSoundB = std::bind(SetAlarm1, std::bind(std::plus<>(), steady_clock::now(), 1h), _1, 30s); // Workaround

    setSoundL(Sound::Beep);
    setSoundB(Sound::Siren);

    std::cout << std::endl;
}

void SetAlarm2(Time t, Sound s, Duration d) {
    std::cout << "SetAlarm2-3 is called" << std::endl;
    return;
}

void SetAlarm2(Time t, Sound s, Duration d, Volume v) {
    std::cout << "SetAlarm2-4 is called" << std::endl;
    return;
}

void overloadingTest() {

    std::cout << "overloadingTest" << std::endl;

    // Lambda implementation
    auto setSoundL = [](Sound s) {
        SetAlarm2(steady_clock::now() + 1h, s, 30s); // Knows which overloaded function to call
    };

    auto setSoundL4 = [](Sound s) {
        SetAlarm2(steady_clock::now() + 1h, s, 30s, Volume::Loud); // Knows which overloaded function to call
    };

    // Bind implementation
    // auto setSoundB = std::bind(SetAlarm2, std::bind(std::plus<>(), steady_clock::now(), 1h), _1, 30s); // gives compile error due to function overload

    // Workaround
    using SetAlarm3ParamType = void(*)(Time t, Sound s, Duration d);
    auto setSoundB = std::bind( static_cast<SetAlarm3ParamType>(SetAlarm2), 
                                std::bind(std::plus<>(), 
                                steady_clock::now(), 1h), _1, 30s); // now works

    setSoundL(Sound::Beep);
    setSoundL4(Sound::Whistle);
    setSoundB(Sound::Siren);
    
    std::cout << std::endl;
}

int main() {
    lambdaAndBindTest();
    overloadingTest();
    return 0;
}