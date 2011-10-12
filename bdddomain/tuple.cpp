#include <bdddomain/tuple.hh>
#include <vector>

using std::vector;

namespace MPG {
  using namespace VarImpl;

  
  Tuple::Tuple(const std::vector<int>& v)
    : data_(domain().represent(begin(v), end(v))), arity_(v.size()) {}

  Tuple::Tuple(const std::initializer_list<int> l)
    : data_(domain().represent(l)), arity_(l.size()) {
  }

  Tuple::Tuple(const Tuple& t)
    : data_(t.data_), arity_(t.arity_) {}

  Tuple::~Tuple(void) {}

  Tuple& Tuple::operator = (const Tuple& t) {
    if (this == &t) return *this;
    /// \todo FIXME!
    assert(false && "FIXME!!!");
    std::cerr << t.arity();
    arity_ = t.arity_;
    data_ = t.data_;
    return *this;
  }

  bool Tuple::equal(const Tuple& t) const {
    return arity_ == t.arity_ ? (data_ == t.data_) : false;
  }

  Bdd Tuple::getBDD(void) const {
    return data_;
  }

  vector<int> Tuple::value(void) const {
    assert(false && "Not yet implemented");
    /*
      const int cubeSize = 1<<(Limits::bbv + Limits::ba);
      int cube_[cubeSize];
      int *cube = cube_;
      CUDD_VALUE_TYPE val;

      DdGen* gen = Cudd_FirstCube(dd(),data_,&cube,&val);
      assert(gen != NULL);
      vector<int> tuple = decodeCube(cube,arity_);
      Cudd_GenFree(gen);

      return tuple;
    */
    std::vector<int> tuple(arity_,-1);
    return tuple;
  }

  void Tuple::print(std::ostream& os) {
    domain().print(os,data_,arity_);
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
    /// \todo remove this const cast
    auto x = const_cast<Tuple&>(t);
    x.print(os);
    return os;
  }

  bool operator == (const Tuple& a, const Tuple& b) {
    return a.equal(b);
  }

  bool operator != (const Tuple& a, const Tuple& b) {
    return ! a.equal(b);
  }
}
