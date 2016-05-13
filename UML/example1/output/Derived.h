#include "Base.h"
#include "PureAbstract.h"

/// class Derived - 
class Derived : public Base, public PureAbstract {
  // Operations
public:
  int foo (int a = 0);
  static int newfoo ();
  int otherfoo ();
protected:
  /// 
  /// @param x		(???) 
  /// @param y		(???) 
  int bar ();
};

