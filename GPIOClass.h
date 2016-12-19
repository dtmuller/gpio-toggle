#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

class GPIOClass {
public:
  GPIOClass(const string& x, const string& dir);
  virtual ~GPIOClass();

public:
  void Write(const string& value);

private:
  void Configure(const string& name, const string& value);

private:
  string _num;
  int _gpio;
};
