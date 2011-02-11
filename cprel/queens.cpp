
#include <rel.hh>
#include <gecode/driver.hh>
#include <gecode/int.hh>
#include <gecode/minimodel.hh>


using namespace Gecode;

class Queens : public Script {
public:
  /// Position of queens on boards
  IntVarArray q;
  /// The actual problem
  Queens(const SizeOptions& opt)
    : q(*this,opt.size(),0,opt.size()-1) {
    const int n = q.size();
    for (int i = 0; i<n; i++)
      for (int j = i+1; j<n; j++) {
        rel(*this, q[i] != q[j]);
        rel(*this, q[i]+i != q[j]+j);
        rel(*this, q[i]-i != q[j]-j);
      }
    branch(*this, q, INT_VAR_SIZE_MIN, INT_VAL_MIN);
  }

  /// Constructor for cloning \a s
  Queens(bool share, Queens& s) : Script(share,s) {
    q.update(*this, share, s.q);
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
    for (int i = 0; i < q.size(); i++) {
      os << q[i] << ", ";
      if ((i+1) % 10 == 0)
        os << std::endl << "\t";
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
  opt.iterations(500);
  opt.size(100);
  opt.parse(argc,argv);
  Script::run<Queens,DFS,SizeOptions>(opt);
  return 0;
}

// STATISTICS: example-any

