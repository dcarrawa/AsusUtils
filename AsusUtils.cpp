#include <cstdint>
#include <fstream>
#include <functional>
#include <string>
#include <unistd.h>
#include <unordered_map>

static const std::string c_SecondScreenBrightnessPath =
    "/sys/class/backlight/asus_screenpad/brightness";

static const std::string c_SecondScreenEnabledPath =
    "/sys/class/backlight/asus_screenpad/bl_power";

static constexpr int32_t c_MinBrightness = 0;
static constexpr int32_t c_MaxBrightness = 255;
static constexpr int32_t c_BrightnessIncrement = 10;
static constexpr int32_t c_ScreenEnabled = 0;
static constexpr int32_t c_ScreenDisabled = 4;

static int32_t GetSecondScreenBrightness()
{
    int32_t screenBrightness = 0;

    std::ifstream brightnessStream(c_SecondScreenBrightnessPath);
    if (brightnessStream.is_open())
    {
        brightnessStream >> screenBrightness;
    }

    return screenBrightness;
}

static void SetSecondScreenBrightness(const std::string& brightness)
{
    std::ofstream setBrightnessStream(c_SecondScreenBrightnessPath);
    if (setBrightnessStream.is_open())
    {
        setBrightnessStream << brightness;
    }
}

static bool GetSecondScreenEnabled()
{
    bool isEnabled = false;

    std::ifstream enabledStream(c_SecondScreenEnabledPath);
    if (enabledStream.is_open())
    {
        int32_t secondScreenIsEnabled = 0;
        enabledStream >> secondScreenIsEnabled;
        isEnabled = (secondScreenIsEnabled == c_ScreenEnabled);
    }

    return isEnabled;
}

static void SetSecondScreenEnabled(const std::string& screenEnabled)
{
    std::ofstream setScreenEnabledStream(c_SecondScreenEnabledPath);
    if (setScreenEnabledStream.is_open())
    {
        setScreenEnabledStream << screenEnabled;
    }
}

static void SetSecondScreenBrightnessCLI(int32_t argc, char* argv[])
{
    if (argc > 2)
    {
        SetSecondScreenBrightness(argv[2]);
    }
}

static void IncreaseSecondScreenBrightnessCLI(int32_t argc, char* argv[])
{
    SetSecondScreenBrightness(std::to_string(
        std::min(GetSecondScreenBrightness() + c_BrightnessIncrement, c_MaxBrightness)));
}

static void DecreaseSecondScreenBrightnessCLI(int32_t argc, char* argv[])
{
    SetSecondScreenBrightness(std::to_string(
        std::max(GetSecondScreenBrightness() - c_BrightnessIncrement, c_MinBrightness)));
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
    // Replicates the functionality found in the Windows driver
    // On -> Hidden -> Off -> On -> ...
    if (!GetSecondScreenEnabled())
    {
        SetSecondScreenEnabled(std::to_string(c_ScreenEnabled));
        SetSecondScreenBrightness(std::to_string(c_MaxBrightness));
    }
    else
    {
        if (GetSecondScreenBrightness() > c_MinBrightness)
        {
            SetSecondScreenBrightness(std::to_string(c_MinBrightness));
        }
        else
        {
            SetSecondScreenEnabled(std::to_string(c_ScreenDisabled));
        }
    }
}

static const std::unordered_map<std::string, std::function<void(int32_t, char*[])>>
    s_Commands {
        {"SetSecondScreenBrightness", &SetSecondScreenBrightnessCLI},
        {"SetSecondScreenEnabled", &SetSecondScreenEnabledCLI},
        {"IncreaseSecondScreenBrightness", &IncreaseSecondScreenBrightnessCLI},
        {"DecreaseSecondScreenBrightness", &DecreaseSecondScreenBrightnessCLI},
        {"CycleSecondScreenModes", &CycleSecondScreenModesCLI},
    };

int32_t main(int32_t argc, char* argv[])
{
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

