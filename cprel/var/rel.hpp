namespace MPG {

inline
int RelVar::min(void) const {
  return x->min();
}

inline
int RelVar::max(void) const {
  return x->max();
  }

template<class Char, class Traits>
inline
std::basic_ostream<Char,Traits>&
operator <<(std::basic_ostream<Char,Traits>& os, const RelVar& x) {
  std::basic_ostringstream<Char,Traits> s;
  s.copyfmt(os); s.width(0);
  if (x.assigned())
    s << x.min();
  else
    s << '[' << x.min() << ".." << x.max() << ']';
  return os << s.str();
  }


}
