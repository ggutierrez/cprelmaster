namespace MPG {

inline
NoneAny::PosVal::PosVal(const NoneAny& b, int p, int v)
  : Choice(b,2), pos(p), val(v) {}

inline
size_t NoneAny::PosVal::size(void) const {
  return sizeof(*this);
  }

inline
NoneAny::NoneAny(Home home, ViewArray<Rel::RelView>& x0)
  : Brancher(home), x(x0) {}

inline
void NoneAny::post(Home home, ViewArray<Rel::RelView>& x) {
  (void) new (home) NoneAny(home,x);
  }

inline
NoneAny::NoneAny(Space& home, bool share, NoneAny& b)
  : Brancher(home,share,b) {
  x.update(home,share,b.x);
  }

inline
Brancher* NoneAny::copy(Space& home, bool share) {
  return new (home) NoneAny(home,share,*this);
  }

inline
bool NoneAny::status(const Space&) const {
  for (int i=0; i<x.size();i++)
    if (!x[i].assigned())
      return true;
  return false;
  }

inline
Choice* NoneAny::choice(Space&) {
  for (int i=0; true; i++)
    if (!x[i].assigned())
      return new PosVal(*this,i,x[i].min());
  GECODE_NEVER;
  return NULL;
  }

inline
ExecStatus NoneAny::commit(Space& home, const Choice& c, unsigned int a) {
  using namespace Gecode;
  const PosVal& pv = static_cast<const PosVal&>(c);
  int pos = pv.pos, val = pv.val;
  if (a == 0)
    return me_failed(x[pos].lq(home,val)) ? ES_FAILED : ES_OK;
  else
    return me_failed(x[pos].gq(home,val+1)) ? ES_FAILED : ES_OK;
  }

inline
void branch(Home home, const RelVarArgs& x) {
  if (home.failed()) return;
  ViewArray<Rel::RelView> y(home,x);
  NoneAny::post(home,y);
  }

}
