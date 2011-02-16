
#include <rel.hh>
#include <gecode/driver.hh>
#include <gecode/int.hh>
#include <gecode/minimodel.hh>


using namespace Gecode;
using namespace MPG;

VarImpDisposer<MPG::Rel::RelVarImp> disp;

class Queens : public Script {
public:
  RelVarArray a;
  /// The actual problem
  Queens(const SizeOptions& opt) : a(*this,2,1,3) {
    std::cout << a[0] << std::endl;
    branch(*this,a);
  }

  /// Constructor for cloning \a s
  Queens(bool share, Queens& s) : Script(share,s) {
    a.update(*this,share,s.a);
  }

  /// Perform copying during cloning
  virtual Space*
  copy(bool share) {
    return new Queens(share,*this);
  }

  /// Print solution
  virtual void
  print(std::ostream& os) const {
    os << "queens\t";
    for (int i = 0; i < a.size(); i++) {
      os << a[i] << ", ";
    }
    os << std::endl;
  }
};

/** \brief Main-function
 *  \relates Queens
 */
int
main(int argc, char* argv[]) {
  SizeOptions opt("Queens");
  opt.solutions(0);
  opt.parse(argc,argv);
  Script::run<Queens,DFS,SizeOptions>(opt);
  return 0;
}
