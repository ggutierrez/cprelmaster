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

GRelation::~GRelation(void) {
//  std::cout << "Disposing grelation" << std::endl;
  //pimpl_->~RelationImpl();
//  std::cout << "--Disposing grelation" << std::endl;
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
  using VarImpl::difference;
  return
      GRelation(
        Impl(new RelationImpl(difference(*pimpl_,*(r.pimpl_))))
        );
}


GRelation create(const std::vector<Tuple>& dom) {
  std::vector<Tuple>::const_iterator c = dom.begin();
  GRelation r(c->arity());
  for (; c != dom.end(); ++c) r.add(*c);
  return r;
}

std::ostream& operator<< (std::ostream& os, const GRelation& r) {
  for(GRelationIter it(r); it(); ++it)
    os << it.val() << " ";
  return os;
}

GRelationIter::GRelationIter(const GRelation& r)
  : pimpl_(new VarImpl::RelationImplIter(r.pimpl_->tuples()))
  , current_(r.pimpl_->arity())
  , valid_(pimpl_->operator ()()) {

  if (valid_)
    current_ = pimpl_->val();
//  std::cerr << "Initial value " << current_ << std::endl;
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
  return current_;
}

void GRelationIter::operator ++(void) {
  if(! pimpl_->operator ()())
    valid_ = false;
  else {
    current_ = pimpl_->val();
    //std::cerr << "New value ready " << current_ << std::endl;
  }
}

}}
