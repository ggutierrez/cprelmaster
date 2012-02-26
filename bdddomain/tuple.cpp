#include <bdddomain/tuple.hh>
#include <bdddomain/encoding.hh>
#include <bdddomain/bdd.hh>
#include <vector>

using std::vector;

namespace MPG {
  using namespace VarImpl;

  Tuple::Tuple(BDD data) : data_(data) {}

  Tuple::Tuple(const Tuple& t)
    : data_(t.data_) {
    // t is a constructed object so the checks on it were already
    // performed.
  }

  Tuple::~Tuple(void) {}

  bool Tuple::operator == (const Tuple& t) const {
    return data_ == t.data_;
  }

  BDD Tuple::encodeElement(int p, int a) {
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

  vector<int> Tuple::value(int arity) const {
    std::vector<int> tuple(arity,-1);
    typedef std::vector<std::vector<std::pair<int,int> > > branch_contents;
    auto f = [&](const branch_contents& r) {
      
      assert(static_cast<unsigned int>(arity) < r.size() && "Unexpected branch length");
      
      for (auto i = arity; i--;) {
        const auto& domainI = r.at(i);
        assert(domainI.size() == 1 &&
               "A range in the representation of a tuple");
        const auto& elem = domainI.at(0);
        assert(elem.first == elem.second &&
               "A range in the representation of a tuple");
        tuple[arity -1 - i] = elem.first;
      }
    };
    traverseSet(factory(), data_, f);
    return tuple;
  }

  void Tuple::output(std::ostream &os, int arity, char sep, char op, char cl) const {
    const auto& v = value(arity);
    os << op;
    for (auto e = begin(v); e != end(v);) {
      os << *e;
      ++e;
      if (e != end(v))
        os << sep;
    }
    os << cl;
  }
}
