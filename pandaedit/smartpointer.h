#ifndef SMARTPOINTER_H
#define SMARTPOINTER_H

template < class T > class smartPointer
{
public:
  smartPointer ()
  {
    m_refs = 0;
  }
  smartPointer (const smartPointer < T > &other)
  {

  }

  ~smartPointer ();
  smartPointer < T > operator= (const smartPointer < T > &other);

  const T *get ()
  {
    return m_ptr;
  }
  void set (const T * val);

private:
  int m_refs;
};

#endif
