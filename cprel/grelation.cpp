#include <cprel/grelation.hh>
#include <bdddomain/rel-impl.hh>

namespace MPG { namespace CPRel {

using namespace VarImpl;

GRelation::GRelation(Impl impl)
  : pimpl_(impl) {}

GRelation::GRelation(int a)
  : pimpl_(Impl(new RelationImpl(a)))
{}

GRelation::GRelation(const GRelation &r)
  : pimpl_(new RelationImpl(*(r.pimpl_))) { }

GRelation& GRelation::operator =(GRelation& right) {
  pimpl_.swap(right.pimpl_);
  return *this;
}

GRelation::~GRelation(void) {}

GRelation GRelation::create_full(int a) {
  RelationImpl full = RelationImpl::create_full(a);
  return
      GRelation(
        Impl(new RelationImpl(full))
        );
}

void GRelation::add(const Tuple &t) {
  pimpl_->add(t);
}

int GRelation::arity(void) const {
  return pimpl_->arity();
}

double GRelation::cardinality(void) const {
  return pimpl_->cardinality();
}

bool GRelation::subsetEq(const GRelation& r) const {
  return VarImpl::subsetEq(*pimpl_,*r.pimpl_);
}

bool GRelation::superset(const GRelation& r) const {
  return VarImpl::superset(*pimpl_,*r.pimpl_);
}

bool GRelation::disjoint(const GRelation& r) const {
  return VarImpl::disjoint(*pimpl_,*r.pimpl_);
}

bool GRelation::eq(const GRelation& r) const {
  return *pimpl_ == *(r.pimpl_);
}

bool GRelation::empty(void) const {
  return pimpl_->empty();
}

bool GRelation::universe(void) const {
  return pimpl_->universe();
}

void GRelation::unionAssign(const GRelation &r) {
  pimpl_->add(*(r.pimpl_));
}

void GRelation::differenceAssign(const GRelation &r) {
  pimpl_->remove(*(r.pimpl_));
}

GRelation GRelation::difference(const GRelation &r) const {
  return
      GRelation(
        Impl(new RelationImpl(VarImpl::difference(*pimpl_,*(r.pimpl_))))
        );
}

GRelation GRelation::intersect(const GRelation &r) const {
  return
      GRelation(
        Impl(new RelationImpl(VarImpl::intersect(*pimpl_,*(r.pimpl_))))
        );
}

GRelation GRelation::Union(const GRelation &r) const {
  return
      GRelation(
        Impl(new RelationImpl(VarImpl::Union(*pimpl_,*(r.pimpl_))))
        );
}

GRelation GRelation::complement(void) const {
  return
      GRelation(
        Impl(new RelationImpl(VarImpl::complement(*pimpl_)))
        );
}

GRelation GRelation::permute(const PermDescriptor& desc) const {
  typedef boost::error_info<struct tag_perm_descriptor,std::string>
      perm_descriptor;

  if (!desc.valid(arity()))
    throw InvalidPermDescriptor()
      << errno_code(errno)
      << perm_descriptor("Invalid permutation description used at: GRelation::permute");
  return
      GRelation(
        Impl(new RelationImpl(pimpl_->permute(desc)))
        );
}

GRelation GRelation::shiftRight(int n) const {
  return
      GRelation(
        Impl(new RelationImpl(pimpl_->shiftRight(n)))
        );
}

GRelation GRelation::shiftLeft(int n) const {
  /// \todo Throw an exception if there are not enough room in the manager to
  /// perform the operation
  return
      GRelation(
        Impl(new RelationImpl(pimpl_->shiftLeft(n)))
        );
}

GRelation GRelation::timesU(int n, bool left) const {
  return
      GRelation(
        Impl(new RelationImpl(pimpl_->timesU(n, left)))
        );
}

GRelation GRelation::join(int j,const GRelation& r) const {
  typedef boost::error_info<struct tag_invalid_join,std::string>
      invalid_join;

//  std::cout << "Middle join call r cardinality " << r.cardinality() << std::endl;
  if (arity() < j || r.arity() < j)
    throw InvalidJoin()
      << errno_code(errno)
      << invalid_join("There are not enough columns for the join");

  return
      GRelation(
        Impl(new RelationImpl(pimpl_->join(j, *(r.pimpl_))))
        );
}

GRelation GRelation::times(const GRelation& r) const {
  return join(0,r);
}

GRelation GRelation::exists(int c) const {
  return
      GRelation(
        Impl(new RelationImpl(pimpl_->exists(c)))
        );
}

GRelation GRelation::unique(int c) const {
  return
      GRelation(
        Impl(new RelationImpl(pimpl_->unique(c)))
        );
}

GRelation GRelation::unique(const std::vector<int>& c) const {
  return
  GRelation(
            Impl(new RelationImpl(pimpl_->unique(c)))
            );
}

GRelation GRelation::project(int p) const {
  typedef boost::error_info<struct tag_projection,std::string>
      projection;

  if(p <= 0 || p > arity()) {
    throw InvalidProjection()
        << errno_code(errno)
        << projection("Invalid columns to project on");
  }
  return
      GRelation(
        Impl(new RelationImpl(pimpl_->project(p)))
        );
}

GRelation create(const std::vector<Tuple>& dom) {
  std::vector<Tuple>::const_iterator c = dom.begin();
  GRelation r(c->arity());
  for (; c != dom.end(); ++c) r.add(*c);
  return r;
}

GRelation create_full(int a) {
  return GRelation(a).complement();
}

GRelation read(std::istream& is, int arity) {
  typedef boost::error_info<struct tag_invalid_source,std::string>
      invalid_source;

  if (arity > Limits::arity)
    throw InvalidAritySource()
        << errno_code(errno)
        << invalid_source("Invalid arity to read the relation");

  if (!is.good() || is.fail()) {
    throw InvalidRelationSource()
        << errno_code(errno)
        << invalid_source("Invalid source describing relation");
  }

  GRelation r(arity);
  std::vector<int> tuple_values;
  tuple_values.reserve(arity);
  std::string line;
  int numLines = 0;
  while (is.good()) {
    std::getline(is, line);
    numLines++;
    if (line.empty()) continue;
    std::stringstream st(line);
    while (st.good()) {
      int v; st >> v;
      tuple_values.push_back(v);
    }
    assert(static_cast<int>(tuple_values.size()) == arity);
    // At this point tuple_values contains all the elements of the tuple
    r.add(Tuple(tuple_values));
    tuple_values.clear();
  }
  return r;
}

std::ostream& operator<< (std::ostream& os, const GRelation& r) {
  if (r.universe()) {
    os << "U";
    return os;
  }
  if (r.empty()) {
    os << "E";
    return os;
  }

  if (r.cardinality() > 1000) {
    //os << "compl(" << r.complement() << ")";
    os << "BIGsc(" << r.complement().cardinality() << ")";
    return os;
  }

  // output of the relation
  os << GRelationIO::curr_start_;
  for(GRelationIter it(r); it(); ++it) {
    os << it.val() << GRelationIO::curr_row_separator_;
  }
  os << GRelationIO::curr_end_;

  return os;
}

std::string GRelationIO::curr_row_separator_ = "";
std::string GRelationIO::curr_value_separator_ = "";
std::string GRelationIO::curr_start_ = "";
std::string GRelationIO::curr_end_ = "";

std::ostream& operator<< (std::ostream& os, const GRelationIO& f) {
  GRelationIO::curr_row_separator_ = f.row_separator_;
  GRelationIO::curr_value_separator_ = f.value_separator_;
  GRelationIO::curr_start_ = f.start_;
  GRelationIO::curr_end_ = f.end_;
  return os;
}


GRelationIter::GRelationIter(const GRelation& r)
  : pimpl_(new VarImpl::RelationImplIter(r.pimpl_->tuples()))
  , valid_(pimpl_->operator ()()) {

  if (valid_)
    current_ = TuplePtr(new Tuple(pimpl_->val()));
}

GRelationIter::GRelationIter(const GRelationIter& r)
  : pimpl_(r.pimpl_), current_(r.current_), valid_(r.valid_) {}

GRelationIter::~GRelationIter(void) {
  pimpl_.reset();
}

bool GRelationIter::operator ()(void) const {
  return valid_;
}

Tuple GRelationIter::val() const {
  return *current_;
}

void GRelationIter::operator ++(void) {
  if(! pimpl_->operator ()())
    valid_ = false;
  else
    current_ = TuplePtr(new Tuple(pimpl_->val()));
}

}}
