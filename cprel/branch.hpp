namespace MPG {
  class NoneAny : public Brancher {
  protected:
    ViewArray<Rel::RelView> x;
    class PosVal : public Choice {
    public:
      int pos; int val;
      PosVal(const NoneAny& b, int p, int v);
      virtual size_t size(void) const;
    };
  public:
    /// \name Brancher creation
    //@{
    /// Constructor
    NoneAny(Home home, ViewArray<Rel::RelView>& x0);
    /// Creates a branch on the variables in x
    static void post(Home home, ViewArray<Rel::RelView>& x);
    //@}
    /// \name Cloning
    //@{
    /// Copy constructor
    NoneAny(Space& home, bool share, NoneAny& b);
    /// Copy
    virtual Brancher* copy(Space& home, bool share);
    //@}
    /// \name Branching
    //@{
    /// Tests if there are more variables in \a x to branch on
    virtual bool status(const Space& home) const;
    /// Choice point creation
    virtual Choice* choice(Space& home);
    /// Choice application
    virtual ExecStatus commit(Space& home, const Choice& c, unsigned int a);
    //@}
  };

  void branch(Home home, const RelVarArgs& x);
}

#include <cprel/branch/none-any.hpp>
