#include "bsp.h"
#include <QFile>



Bsp::Bsp()
{

}



void Bsp::ExportPin (int pin)
{
    QFile expFile(PATH_EXPORT_GPIO);
    if (expFile.open(QIODevice::WriteOnly)) {
        expFile.write(QString::number(pin).toUtf8());
        expFile.close();
    }
}



void Bsp::SetGpioDirection (int pin, PinDirection direction)
{
    QFile dirFile(PATH_GPIO + QString::number(pin) + "/direction");
    if (dirFile.open(QIODevice::WriteOnly)) {
        if (direction == pdIn) {
            dirFile.write("in");
        } else {
            dirFile.write("out");
        }
        dirFile.close();
    }
}



void Bsp::SetGpioValue (int pin, int value)
{
    QFile dirFile(PATH_GPIO + QString::number(pin) + "/value");
    if (dirFile.open(QIODevice::WriteOnly)) {
        if (value > 0) {
            dirFile.write("1");
        } else {
            dirFile.write("0");
        }
        dirFile.close();
    }
}



void Bsp::SetGpioEdge (int pin, PinEdge edge)
{
    QFile dirFile(PATH_GPIO + QString::number(pin) + "/edge");
    if (dirFile.open(QIODevice::WriteOnly)) {
        if (edge == peNone) {
            dirFile.write("none");
        } else if (edge == peRising) {
            dirFile.write("rising");
        } else if (edge == peFalling) {
            dirFile.write("falling");
        } else if (edge == peBoth) {
            dirFile.write("both");
        }
        dirFile.close();
    }
}



void Bsp::SetGpioActiveLow (int pin, bool activeLow)
{
    QFile dirFile(PATH_GPIO + QString::number(pin) + "/active_low");
    if (dirFile.open(QIODevice::WriteOnly)) {
        if (activeLow) {
            dirFile.write("1");
        } else {
            dirFile.write("0");
        }
        dirFile.close();
    }
}
