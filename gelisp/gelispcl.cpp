#include <gelisp/gelisp.hh>

using namespace GeLisp;

int main(void) {
  GlSpace *sp = newSpace();
  intVar(*sp,intSet(2,3));
  sp->intBranch();
  searchAll(*sp);
  delete sp;
  return 0;
}
