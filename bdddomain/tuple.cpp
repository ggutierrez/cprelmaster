#include <bdddomain/tuple.hh>
#include <bdddomain/encoding.hh>
#include <bdddomain/bdd.hh>
#include <vector>

using std::vector;

namespace MPG {
  using namespace VarImpl;

  Tuple::Tuple(BDD data, int arity) : data_(data), arity_(arity) {
    assert(Limits::checkSafeArity(arity) && 
           "The manager was not configured to support this arity");
  }

  Tuple::Tuple(const std::initializer_list<int> l)
    : arity_(l.size()) {
    assert(Limits::checkSafeArity(l.size()) && 
           "The manager was not configured to support this arity");
    data_ = encode(l);
  }

  Tuple::Tuple(const Tuple& t)
    : data_(t.data_), arity_(t.arity_) {
    // t is a constructed object so the checks on it were already
    // performed.
  }

  Tuple::~Tuple(void) {}

  Tuple& Tuple::operator = (const Tuple&) {
    assert(false && "This method has not been implemented");
    return *this;
  }

  bool Tuple::operator == (const Tuple& t) const {
    return arity_ == t.arity_ ? (data_ == t.data_) : false;
  }

  BDD Tuple::encode(int p, int a) {
    assert(Limits::checkSafeValue(p) &&
           "The manager was not configured to support this value");
    assert(Limits::checkSafeArity(a) && 
           "The manager was not configured to support this arity");

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

  BDD Tuple::getBDD(void) const {
    return data_;
  }

  vector<int> Tuple::value(void) const {
    std::vector<int> tuple(arity_,-1);
    typedef std::vector<std::vector<std::pair<int,int>>> branch_contents;
    auto f = [&](const branch_contents& r) {
      
      assert(static_cast<unsigned int>(arity_) < r.size() && "Unexpected branch length");
      
      for (auto i = arity_; i--;) {
	const auto& domainI = r.at(i);
	assert(domainI.size() == 1 &&
	       "A range in the representation of a tuple");
	const auto& elem = domainI.at(0);
	assert(elem.first == elem.second &&
	       "A range in the representation of a tuple");
	tuple[arity_ -1 - i] = elem.first;
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
    const auto& v = t.value();
    for (auto e = begin(v); e != end(v);) {
      os << *e;
      ++e;
      if (e != end(v))
	os << TupleIO::curr_value_separator_;
    }
    os << TupleIO::curr_end_;
    return os;

  }

}
