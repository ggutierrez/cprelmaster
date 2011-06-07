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

bool GRelation::add(const Tuple &t) {
  RelationImpl old = *pimpl_;
  pimpl_->add(t);
  return old != *pimpl_;
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

bool GRelation::unionAssign(const GRelation &r) {
  RelationImpl old = *pimpl_;
  pimpl_->add(*(r.pimpl_));
  return old != *pimpl_;
}

bool GRelation::differenceAssign(const GRelation &r) {
  RelationImpl old = *pimpl_;
  pimpl_->remove(*(r.pimpl_));
  return old != *pimpl_;
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

GRelation GRelation::complement(void) const {
  return
      GRelation(
        Impl(new RelationImpl(VarImpl::complement(*pimpl_)))
        );
}
GRelation GRelation::permute(const PermDescriptor& desc) const {
  return
      GRelation(
        Impl(new RelationImpl(pimpl_->permute(desc)))
        );
}

GRelation GRelation::timesU(int n, bool left) const {
  return
      GRelation(
        Impl(new RelationImpl(pimpl_->timesU(n, left)))
        );
}

GRelation GRelation::join(int j,const GRelation& r) const {
  return
      GRelation(
        Impl(new RelationImpl(pimpl_->join(j, *(r.pimpl_))))
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
  if (!is.good() || is.fail())
    throw InvalidRelationSource("While reading stream:");

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

//  if (r.cardinality() > 1000) {
//    os << "compl(" << r.complement() << ")";
//    return os;
//  }

  for(GRelationIter it(r); it(); ++it)
    os << it.val() << " ";

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
