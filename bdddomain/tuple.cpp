#include <bdddomain/tuple.hh>
#include <bdddomain/encoding.hh>
#include <bdddomain/bdd.hh>
#include <vector>

using std::vector;

namespace MPG {
  using namespace VarImpl;

  Tuple::Tuple(const std::vector<int>& v)
    : data_(encode(v)), arity_(v.size()) {}

  Tuple::Tuple(const std::initializer_list<int> l)
    : arity_(l.size()) {
    /** \todo This is not as efficient as it can be. The best thing is for encode
     * to get two iterators and this will avoid copying to a temporal vector
     */
    std::vector<int> v(l.begin(),l.end());
    // The representation from "encode" is already referenced.
    data_ = encode(v);
  }

  Tuple::Tuple(const Tuple& t)
    : data_(t.data_), arity_(t.arity_) {}

  Tuple::~Tuple(void) {}

  Tuple& Tuple::operator = (const Tuple&) {
    assert(false && "This method has not been implemented");
    return *this;
  }

  bool Tuple::operator == (const Tuple& t) const {
    return arity_ == t.arity_ ? (data_ == t.data_) : false;
  }

  BDD Tuple::encode(int p, int a) {
    BDD f = one();
    for (int i = Limits::bitsPerInteger; i--;) {
      BDD v = factory().bddVar((i << Limits::ba)+a);
      if (p & 1)
	f &= v;
      else
	f &= !v;
      p >>= 1;
    }
    return f;
  }

  BDD Tuple::encode(const std::vector<int>& v) const {
    BDD f = one();
    int c = v.size()-1;
    for (unsigned int i = 0; i < v.size(); i++) {
      f &= encode(v.at(i),c);
      c--;
    }
    return f;
  }

  BDD Tuple::getBDD(void) const {
    return data_;
  }

  vector<int> Tuple::value(void) const {
    std::vector<int> tuple(arity_,-1);
    bool first = false;
    auto f = [&](const std::vector<std::vector<int>>& r) {
      if (!first) {
	for (auto i = 0; i < arity_; i++) {
	  // there should not be ranges because this is a tuple
	  assert(r.at(i).size() == 1);
	  tuple[i] = r.at(i).at(0);
	}
	first = false;
      } else {
	assert(false && "Unexpected branch for a tuple");
      }
    };
    traverseSet(factory(), data_, f);
    return tuple;
  }

  std::string TupleIO::curr_value_separator_ = ",";
  std::string TupleIO::curr_start_ = "[";
  std::string TupleIO::curr_end_ = "]";

  std::ostream& operator<< (std::ostream& os, const TupleIO& f) {
    TupleIO::curr_value_separator_ = f.value_separator_;
    TupleIO::curr_start_ = f.start_;
    TupleIO::curr_end_ = f.end_;
    return os;
  }

  std::ostream& operator << (std::ostream& os, const Tuple& t) {
    os << TupleIO::curr_start_;
    const std::vector<int>& v = t.value();
    for (unsigned int i = 0; i < v.size(); i++) {
      os << v.at(i);
      if (i < v.size()-1)
	os << TupleIO::curr_value_separator_;
    }
    os << TupleIO::curr_end_;
    return os;

  }

}
