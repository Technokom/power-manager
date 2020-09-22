#ifndef BSP_H
#define BSP_H



#define PATH_EXPORT_GPIO        "/sys/class/gpio/export"
#define PATH_GPIO               "/sys/class/gpio/gpio"

#define IN_MINUS_1              20
#define IN_MINUS_2              45
#define IN_PLUS_1               106
#define IN_PLUS_2               105
#define RPM_PIN                 104


enum PinDirection { pdIn = 0, pdOut };
enum PinEdge { peNone, peRising, peFalling, peBoth };


class Bsp
{
public:
    Bsp();
    static void ExportPin (int pin);
    static void SetGpioDirection (int pin, PinDirection direction);
    static void SetGpioValue (int pin, int value);
    static void SetGpioEdge (int pin, PinEdge edge);
    static void SetGpioActiveLow (int pin, bool activeLow);
};

#endif // BSP_H
