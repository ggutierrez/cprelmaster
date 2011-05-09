#include <cprel/grelation.hh>
#include <bdddomain/rel-impl.hh>

namespace CPRel {

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
  pimpl_.reset();
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

bool GRelation::unionAssign(const GRelation &r) {
  RelationImpl old = *pimpl_;
  pimpl_->add(*(r.pimpl_));
  return old != *pimpl_;
}


GRelation create(const std::vector<Tuple>& dom) {
  std::vector<Tuple>::const_iterator c = dom.begin();
  GRelation r(c->arity());
  for (; c != dom.end(); ++c) r.add(*c);
  return r;
}
}
