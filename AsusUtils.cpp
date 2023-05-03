#include <cstdint>
#include <format>
#include <functional>
#include <string>
#include <unistd.h>
#include <unordered_map>

static void SetSecondScreenBrightness(int32_t argc, char* argv[])
{
    if (argc > 2)
    {
        const std::string command =
            "echo " + std::string(argv[2]) +
            " | tee /sys/devices/platform/asus-nb-wmi/screenpad_brightness";
        system(command.c_str());
    }
}

static std::unordered_map<std::string, std::function<void(int32_t, char*[])>> s_Commands {
    {"SetSecondScreenBrightness", &SetSecondScreenBrightness}};

int32_t main(int32_t argc, char* argv[])
{
    static constexpr uid_t rootUid = 0;
    setuid(rootUid);

    if (argc > 1)
    {
        const auto& command = s_Commands.find(argv[1]);
        if (command != s_Commands.end())
        {
            command->second(argc, argv);
        }
    }

    return 0;
}

