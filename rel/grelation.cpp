#include <rel/grelation.hh>
#include <bdddomain/rel-impl.hh>

namespace MPG {

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

  void GRelation::add(const std::vector<Tuple>& s) {
    std::for_each(s.begin(), s.end(),
		  [=](const Tuple& t) { pimpl_->add(t); });
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

  GRelation GRelation::permute(const std::vector<std::pair<int,int>>& desc) const {
    typedef boost::error_info<struct tag_perm_descriptor,std::string>
      perm_descriptor;
    /*    if (!desc.valid(arity()))
      throw InvalidPermDescriptor()
	<< errno_code(errno)
	<< perm_descriptor("Invalid permutation description used at: GRelation::permute");
    */
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

  GRelation GRelation::timesU(int n, bool left) const {
    /// \todo Temporal: make two different methods that call the
    /// varimp accordingly
    return
      GRelation(left ? Impl(new RelationImpl(pimpl_->timesULeft(n)))
		:  Impl(new RelationImpl(pimpl_->timesURight(n)))
		);
  }

  GRelation GRelation::join(int j,const GRelation& r) const {
    typedef boost::error_info<struct tag_invalid_join,std::string>
      invalid_join;

    if (arity() < j || r.arity() < j)
      throw InvalidJoin()
	<< errno_code(errno)
	<< invalid_join("There are not enough columns for the join");

    return
      GRelation(
		Impl(new RelationImpl(pimpl_->join(j, *(r.pimpl_))))
		);
  }

  GRelation GRelation::follow(int f,const GRelation& right) const {
    /// \todo handle arity errors with exceptions

    //  if (arity() < j || r.arity() < j)
    //    throw InvalidJoin()
    //      << errno_code(errno)
    //      << invalid_join("There are not enough columns for the join");

    return
      GRelation(
		Impl(new RelationImpl(pimpl_->follow(f, *(right.pimpl_))))
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

  GRelation GRelation::forall(int c) const {
    return
      GRelation(
		Impl(new RelationImpl(pimpl_->forall(c)))
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

  Tuple GRelation::pickOneTuple(void) const {
    /// \todo throw an exception
    assert(!empty() && "Relation is empty, nothing to return");
    return
      pimpl_->pickOneTuple();
  }

  void GRelation::print(std::ostream& os) const {
    pimpl_->print(os);
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
    /*
      if (!is.good() || is.fail()) {
      throw InvalidRelationSource()
      << errno_code(errno)
      << invalid_source("Invalid source describing relation");
	
      }
    */

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
    r.print(os);
    return os;
  }

  std::string GRelationIO::curr_value_start_ = "";
  std::string GRelationIO::curr_value_end_ = "";
  std::string GRelationIO::curr_start_ = "";
  std::string GRelationIO::curr_end_ = "";

  std::ostream& operator<< (std::ostream& os, const GRelationIO& f) {
    GRelationIO::curr_value_start_ = f.value_start_;
    GRelationIO::curr_value_end_ = f.value_end_;
    GRelationIO::curr_start_ = f.start_;
    GRelationIO::curr_end_ = f.end_;
    return os;
  }

}
