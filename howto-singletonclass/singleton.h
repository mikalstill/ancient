// This is the class declaration for the singleton class

class sing{
 public:
  sing(){}
  static sing& grab();

  int getFoo();
  void setFoo(int foo);

 private:
  // People cannot construct us, or copy us
  sing(const sing& other){}

  int m_foo;
  static sing *m_theSing;
};
