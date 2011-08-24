namespace MPG { namespace CPRel { namespace Prop {

template <typename View0, typename View1, typename View2>
Intersect<View0,View1,View2>::Intersect(Gecode::Home home, View0 a, View1 b,View2 c)
  : Gecode::Propagator(home), a_(a), b_(b), c_(c) {
  a_.subscribe(home,*this,CPRel::PC_CPREL_BND);
  b_.subscribe(home,*this,CPRel::PC_CPREL_BND);
  c_.subscribe(home,*this,CPRel::PC_CPREL_BND);
}

template <typename View0, typename View1, typename View2>
Gecode::ExecStatus
Intersect<View0,View1,View2>::post(Gecode::Home home, View0 a, View1 b, View2 c) {
  /// \todo This propagator will benefit from the existence of the same method
  /// for different kind of views. Right now these metods are not implemented
  /*
  if (Gecode::same(a,b)) {
    return Equal<View0,View2>::post(home,a,c);
  }
  if (Gecode::same(a,c)) {
    return Equal<View1,View2>::post(home,b,c);
  }
  if (Gecode::same(b,c)) {
    return Equal<View1,View2>::post(home,a,c);
  }
  */
  (void) new (home) Intersect(home,a,b,c);
  return Gecode::ES_OK;
}

template <typename View0, typename View1, typename View2>
size_t Intersect<View0,View1,View2>::dispose(Gecode::Space& home) {
  a_.cancel(home,*this,CPRel::PC_CPREL_BND);
  b_.cancel(home,*this,CPRel::PC_CPREL_BND);
  c_.cancel(home,*this,CPRel::PC_CPREL_BND);

  (void) Propagator::dispose(home);
  return sizeof(*this);
}

template <typename View0, typename View1, typename View2>
Intersect<View0,View1,View2>::Intersect(Gecode::Space& home, bool share, Intersect& p)
  : Gecode::Propagator(home,share,p), a_(p.a_), b_(p.b_), c_(p.c_) {
  a_.update(home,share,p.a_);
  b_.update(home,share,p.b_);
  c_.update(home,share,p.c_);
}

template <typename View0, typename View1, typename View2>
Gecode::Propagator*
Intersect<View0,View1,View2>::copy(Gecode::Space& home, bool share) {
  return new (home) Intersect(home,share,*this);
}

template <typename View0, typename View1, typename View2>
Gecode::PropCost
Intersect<View0,View1,View2>::cost(const Gecode::Space&,
                              const Gecode::ModEventDelta&) const {
  return Gecode::PropCost::binary(Gecode::PropCost::LO);
}

template <typename View0, typename View1, typename View2>
Gecode::ExecStatus
Intersect<View0,View1,View2>::propagate(Gecode::Space& home,
                                     const Gecode::ModEventDelta&)  {

  // glb(c_) \subseteq glb(a_)  \cap glb(b_)
  {
    GRelation r(a_.glb().intersect(b_.glb()));
    GECODE_ME_CHECK(c_.include(home,r));
  }

  // lub(_a) \cap lub(b) \subseteq lub(_c)
  {
    GRelation r(a_.lub().intersect(b_.lub()).intersect(c_.lub()));
    GECODE_ME_CHECK(c_.exclude(home,r.complement()));
  }

  // c_ \subseteq a_
  GECODE_ME_CHECK(a_.include(home,c_.glb()));

  // c_ \subseteq b_
  GECODE_ME_CHECK(b_.include(home,c_.glb()));

  // lub(b_) \subseteq glb(a_) \ lub(c_)
  GECODE_ME_CHECK(b_.exclude(home, a_.glb().difference(c_.lub())));

  // lub(a_) \subseteq glb(b_) \ lub(c_)
  GECODE_ME_CHECK(a_.exclude(home, b_.glb().difference(c_.lub())));


  // Propagator subsumpiton
  if (a_.assigned() && b_.assigned() && c_.assigned())
    return home.ES_SUBSUMED(*this);

  return Gecode::ES_FIX;
}

}}}
