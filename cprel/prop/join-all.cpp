#include <cprel/cprel.hh>

#undef fail

namespace MPG {
  using namespace CPRel;
  
  // Find the permutation for the result of the join
  std::vector<std::pair<int,int>> permJoin(int n, int m, int j, int k) {
    // | n-j | j | m-j |
    // ->
    // | n-j | m-j | j |
    std::vector<std::pair<int,int>> result;
    
    for (int i = 0; i < m-j; i++)
      result.push_back(std::make_pair(i,i+j));
    int s = 0;
    for (int i = m-j; i < m; i++)  {
      result.push_back(std::make_pair(i,s));
      s++;
    }
    return result;
  }
  
  // Find the permutation for the result of the join
  std::vector<std::pair<int,int>> permC(int n, int m, int j, int k) {
    // | n-j | m-j | j |
    // ->
    // | n-j | j | m-j |
    std::vector<std::pair<int,int>> result;
    
    for (int i = 0; i < j; i++)
      result.push_back(std::make_pair(i,i+m-j));
    int s = 0;
    for (int i = j; i < m; i++)  {
      result.push_back(std::make_pair(i,s));
      s++;
    }
    return result;
  }


  void restrJoinAll(Gecode::Space& home, CPRelVar A, int j, CPRelVar B, CPRelVar C) {
    if (home.failed()) return;
    
    /// \todo Change the type of the exception to match the constraint being posted.
    typedef boost::error_info<struct tag_invalid_follow,std::string>
      invalid_restrJoinAll;

    int arityJoin = A.arity() + B.arity() - j;
    int arityFollow = arityJoin - j;
    if (j > A.arity() ||
        j > B.arity() ||
        C.arity() != arityJoin)
      throw InvalidFollow()
        << errno_code(errno)
        << invalid_restrJoinAll("Invalid arity of the variables.");


    
    CPRelVar joinResult(home,GRelation(arityJoin),GRelation::create_full(arityJoin));
    join(home,A,j,B,joinResult);    

    equal(home,C,joinResult);
    /*
    CPRelVar followAllResult(home,GRelation(arityFollow),GRelation::create_full(arityFollow));
    followAll(home,A,j,B,followAllResult);
    
    CPRelVar permJoinResult(home,GRelation(arityJoin),GRelation::create_full(arityJoin));
    auto pj = permJoin(A.arity(), B.arity(), j, C.arity());
    permutation(home,joinResult,permJoinResult,pj);

    CPRelVar Cp(home,GRelation(C.arity()),GRelation::create_full(C.arity()));
    auto pc = permC(A.arity(), B.arity(), j, C.arity());
    permutation(home,C,Cp,pc);

    CPRelVar universeJ(home,GRelation::create_full(j),GRelation::create_full(j));
    CPRelVar crossp(home,GRelation(arityFollow + j),GRelation::create_full(arityFollow + j));
    join(home,followAllResult,0,universeJ,crossp);
    intersect(home,crossp,permJoinResult,Cp);
    */
  }
}
