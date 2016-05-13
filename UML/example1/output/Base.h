// Abstract
/// class Base - 
class Base {
  // Attributes
public:
  static int dummy;
private:
  int x;
  // Operations
public:
  virtual int foo (int a = 0) = 0;
protected:
  virtual int bar (int x = 0, int y = 0) = 0;
};

