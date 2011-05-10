#include <cprel/grelation.hh>
#include <bdddomain/rel-impl.hh>

namespace MPG { namespace CPRel {

using namespace VarImpl;

GRelation::GRelation(int a) {
  pimpl_ = boost::shared_ptr<RelationImpl>(new RelationImpl(a));
}

GRelation::GRelation(const GRelation &r)
  : pimpl_(r.pimpl_) { }

GRelation& GRelation::operator =(GRelation& right) {
  pimpl_.swap(right.pimpl_);
  return *this;
}

GRelation::~GRelation(void) {
//  std::cerr << "Disposing grelation" << std::endl;
  pimpl_.reset();
//  std::cerr << "--Disposing grelation" << std::endl;
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

bool GRelation::subset(const GRelation& r) const {
  return VarImpl::subset(*pimpl_,*r.pimpl_);
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

GRelation create(const std::vector<Tuple>& dom) {
  std::vector<Tuple>::const_iterator c = dom.begin();
  GRelation r(c->arity());
  for (; c != dom.end(); ++c) r.add(*c);
  return r;
}

std::ostream& operator<< (std::ostream& os, const GRelation& r) {
  os << "RelationR";
  return os;
}

}}
