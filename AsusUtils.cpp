#include <cstdint>
#include <functional>
#include <string>
#include <unistd.h>
#include <unordered_map>

const static std::string c_SecondScreenBrightnessPath =
    "/sys/devices/platform/asus-nb-wmi/screenpad_brightness";

const static std::string c_SecondScreenEnabledPath =
    "/sys/devices/platform/asus-nb-wmi/screenpad_enable";

static void SetSecondScreenBrightness(const std::string& brightness)
{
    const std::string command =
        "echo " + brightness + " | tee " + c_SecondScreenBrightnessPath;

    system(command.c_str());
}

static void SetSecondScreenBrightnessCLI(int32_t argc, char* argv[])
{
    if (argc > 2)
    {
        SetSecondScreenBrightness(argv[2]);
    }
}

static void SetSecondScreenEnabled(const std::string& screenEnabled)
{
    const std::string command =
        "echo " + screenEnabled + " | tee " + c_SecondScreenEnabledPath;

    system(command.c_str());
}

static void SetSecondScreenEnabledCLI(int32_t argc, char* argv[])
{
    if (argc > 2)
    {
        SetSecondScreenEnabled(argv[2]);
    }
}

static void CycleSecondScreenModesCLI(int32_t argc, char* argv[])
{
    const std::string secondScreenEnabledCommand = "cat " + c_SecondScreenEnabledPath;
    const int32_t secondScreenIsEnabled = system(secondScreenEnabledCommand.c_str());

    if (secondScreenIsEnabled == 0)
    {
        SetSecondScreenEnabled("1");
        SetSecondScreenBrightness("100");
    }
    else
    {
        const std::string secondScreenBrightnessCommand =
            "cat " + c_SecondScreenBrightnessPath;
        const int32_t screenBrightness = system(secondScreenBrightnessCommand.c_str());

        if (screenBrightness > 0)
        {
            SetSecondScreenBrightness("0");
        }
        else
        {
            SetSecondScreenEnabled("0");
        }
    }
}

static std::unordered_map<std::string, std::function<void(int32_t, char*[])>> s_Commands {
    {"SetSecondScreenBrightness", &SetSecondScreenBrightnessCLI},
    {"SetSecondScreenEnabled", &SetSecondScreenEnabledCLI},
    {"CycleSecondScreenModes", &CycleSecondScreenModesCLI},
};

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

