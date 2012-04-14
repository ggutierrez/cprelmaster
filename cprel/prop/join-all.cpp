#include <cprel/cprel.hh>

#undef fail

namespace MPG {
  using namespace CPRel;
  using std::vector;
  using std::pair;
  
  //put the joined part on the right
  vector<pair<int,int>> putJoinedPartOnRight(int j, int m)  {
    vector<pair<int,int>> result;
    if (j >= m - j) {
      for (int i = 0; i < m - j  ; i++)
        result.push_back(std::make_pair(i,i + j));
    }
    else {
      for (int i = 0; i <= j - 1 ; i++)
        result.push_back(std::make_pair(i,i + m - j ));
    }
    return result;
  }
  
  //create the permutation description needed to reorder one columns if j!= m - j
  //indexStart is the number of the column that will be reorder
  //indexEnd is the number of the column to which the column indexStart will go
  std::vector<pair<int,int>> permJoinReorderOne(int indexStart, int indexEnd)  {
  vector<pair<int,int>> result;
  for (int i = indexStart; i < indexEnd ; i++)
    result.push_back(std::make_pair(i,i+1));
  return result;
}

  //create the permutation description needed to reorder columns if j!= m - j
  vector<pair<int,int>> permJoinReorder(int j, int m)  {
    vector<pair<int,int>> result;
    vector<pair<int,int>> temp;  
    
    //if the joined part is smaller
    if(j < m - j) {
      for(int i = m - j - 1 ; i >= j ; i--) {
        temp = permJoinReorderOne(i,i+j);
        copy (temp.begin(),temp.end(),back_inserter(result));
      }
    }
    //if the joined part is bigger
    else if(j > m - j) {
      for(int i = m - j - 1 ; i >= 0 ; i--) {
        temp = permJoinReorderOne(i,i + j - 1);
        copy (temp.begin(),temp.end(),back_inserter(result));
      }
    }
    return result;
  }
  
  vector<pair<int,int>> permJoin(int m, int j) {
    // | n-j | j | m-j |
    // ->
    // | n-j | m-j | j |
    vector<pair<int,int>> result; 
    vector<pair<int,int>> temp;
    
    //first we put the joined column on the right
    result = putJoinedPartOnRight(j,m);
    /*
      for(auto i = result.begin() ; i!=result.end() ; ++i)
      cout << i->first << "|" << i->second << endl;
    */
    //now we adapat if needed
    if (j!=m) {
      temp = permJoinReorder(j,m);
      copy (temp.begin(),temp.end(),back_inserter(result));
    }
    /*
      for(auto i = result.begin() ; i!=result.end() ; ++i)
      cout << i->first << "|" << i->second << endl;
    */
    return result;
  }

  
  void joinAll(Gecode::Space& home, CPRelVar A, int j, CPRelVar B, CPRelVar C) {
    if (home.failed()) return;
    
    /// \todo Change the type of the exception to match the constraint being posted.
    typedef boost::error_info<struct tag_invalid_follow,std::string>
      invalid_joinAll;

    int n = A.arity();
    int m = B.arity();
    int k = C.arity();

    int arityJoin = n + m - j;
    int arityFollow = arityJoin - j;

    if (j > A.arity() ||
        j > B.arity() ||
        C.arity() != arityJoin)
      throw InvalidFollow()
        << errno_code(errno)
        << invalid_joinAll("Invalid arity of the variables.");

    // put the constraint in terms of other constraints existent in the system
    CPRelVar joinResult(home, GRelation(arityJoin), GRelation::create_full(arityJoin));
    join(home,A,j,B,joinResult);    

    CPRelVar followAllResult(home,GRelation(arityFollow),GRelation::create_full(arityFollow));
    followAll(home,A,j,B,followAllResult);
    
    CPRelVar u(home,GRelation::create_full(j),GRelation::create_full(j));
    CPRelVar times(home,GRelation(arityJoin),GRelation::create_full(arityJoin));
    join(home,followAllResult,0,u,times);
    
    CPRelVar permJoinResult(home,GRelation(arityJoin),GRelation::create_full(arityJoin));
    permutation(home,joinResult,permJoinResult,permJoin(m,j));

    CPRelVar permC(home,GRelation(k),GRelation::create_full(k));
    permutation(home,C,permC,permJoin(m,j));
    
    intersect(home,times,permJoinResult,permC);
  }
}
