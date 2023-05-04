#include <cstdint>
#include <fstream>
#include <functional>
#include <string>
#include <unistd.h>
#include <unordered_map>

static const std::string c_SecondScreenBrightnessPath =
    "/sys/class/backlight/asus-screenpad/brightness";

static const std::string c_SecondScreenEnabledPath =
    "/sys/class/backlight/asus-screenpad/bl_power";

static constexpr uint32_t c_MinBrightness = 0;
static constexpr uint32_t c_MaxBrightness = 255;
static constexpr uint32_t c_ScreenOn = 0;
static constexpr uint32_t c_ScreenOff = 4;

static void SetSecondScreenBrightness(const std::string& brightness)
{
    std::ofstream setBrightnessStream(c_SecondScreenBrightnessPath);
    if (setBrightnessStream.is_open())
    {
        setBrightnessStream << brightness;
    }
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
    std::ofstream setScreenEnabledStream(c_SecondScreenEnabledPath);
    if (setScreenEnabledStream.is_open())
    {
        setScreenEnabledStream << screenEnabled;
    }
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
    std::ifstream enabledStream(c_SecondScreenEnabledPath);
    if (enabledStream.is_open())
    {
        int32_t secondScreenIsEnabled = -1;
        enabledStream >> secondScreenIsEnabled;

        if (secondScreenIsEnabled != c_ScreenOn)
        {
            SetSecondScreenEnabled(std::to_string(c_ScreenOn));
            SetSecondScreenBrightness(std::to_string(c_MaxBrightness));
        }
        else
        {
            std::ifstream brightnessStream(c_SecondScreenBrightnessPath);
            int32_t screenBrightness = 0;
            brightnessStream >> screenBrightness;

            if (screenBrightness > 0)
            {
                SetSecondScreenBrightness(std::to_string(c_MinBrightness));
            }
            else
            {
                SetSecondScreenEnabled(std::to_string(c_ScreenOff));
            }
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

