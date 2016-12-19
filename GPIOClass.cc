#include <cassert>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <unistd.h>

#include "GPIOClass.h"

using namespace std;

GPIOClass::GPIOClass(const string& x, const string& dir) :
    _num(string(x))
{
  Configure("/sys/class/gpio/export", _num);
  assert(dir=="in" || dir=="out");
  Configure("/sys/class/gpio/gpio" + _num + "/direction", dir);

  const string name("/sys/class/gpio/gpio" + _num + "/value");
  //const string name("/dev/stdout");
  _gpio = open(name.c_str(), O_WRONLY);
  if (_gpio < 0) {
    throw runtime_error("Failed to open " + name);
  }
}

GPIOClass::~GPIOClass() {
  close(_gpio);
  Configure("/sys/class/gpio/unexport", _num);
}

void GPIOClass::Write(const string& value) {
  Configure("/sys/class/gpio/gpio" + _num + "/value", value);
  if (_gpio < 0) {
    throw runtime_error("GPIO not open for writing");
  }
  write(_gpio, value.c_str(), value  .length());
  //*_gpio << value;
}

void GPIOClass::Configure(const string& name, const string& value) {
  ofstream fd(name.c_str());
  if (fd < 0) {
    throw runtime_error("Failed to open " + name);
  }
  fd << value;
  fd.close();
}
