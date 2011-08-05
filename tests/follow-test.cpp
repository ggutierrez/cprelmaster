#include <rel/grelation.hh>

using std::cout;
using std::endl;
using std::pair;
using std::vector;
using std::make_pair;
using std::ifstream;

using namespace MPG;

int main(void) {
  cout << "Follows test starts" << endl;
  {
    GRelation a(3);
    a.add({{0,1,3},{1,2,2},{2,2,2},{3,2,2}});
    cout << "A relation: " << a << endl;

    GRelation b(3);
    b.add({{2,2,9},{2,3,8},{2,2,7}});
    cout << "B relation: " << b << endl;

    GRelation exp(2);
    exp.add({{1,9},{1,7},{2,9},{2,7},{3,9},{3,7}});
    cout << "Expected relation: " << exp << endl;

    GRelation follow = a.follow(2,b);
    cout << "Follow relation: " << follow << endl;

    assert(follow.eq(exp) && "Unexpected returned relation");
  }
  return 0;
}
