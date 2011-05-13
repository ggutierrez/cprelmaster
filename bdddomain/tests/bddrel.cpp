#include <boost/test/unit_test.hpp>
#include <vector>
#include <cprel/tuple.hh>
#include <bdddomain/rel-impl.hh>

BOOST_AUTO_TEST_SUITE(BddRelSuite )

BOOST_AUTO_TEST_CASE(RelBasic) {
  using namespace MPG::CPRel;
  using namespace MPG::CPRel::VarImpl;

  RelationImpl r(2);
  RelationImpl empty(2);

  BOOST_CHECK_EQUAL(r,empty);

  RelationImpl x = intersect(empty,r);
  BOOST_CHECK_EQUAL(x,empty);
  BOOST_CHECK_EQUAL(x.cardinality(),0);

  RelationImpl full = RelationImpl::create_full(2);
  BOOST_CHECK_EQUAL(complement(full),empty);

  r.add(Tuple(2,3));

  BOOST_CHECK_EQUAL(r.cardinality(),1);

  BOOST_CHECK(subset(empty,r));
  BOOST_CHECK(!subset(r,empty));

  RelationImpl s(2);
  s.add(Tuple(0,0));
  BOOST_CHECK_EQUAL(intersect(r,s),empty);
  s.add(r);
  BOOST_CHECK_EQUAL(s.arity(),2);
  BOOST_CHECK_EQUAL(intersect(s,r).cardinality(),1);
  BOOST_CHECK_EQUAL(intersect(s,r),r);
  BOOST_CHECK(subset(r,s));
  BOOST_CHECK(r!=s);
  BOOST_CHECK(subset(r,full));

}

BOOST_AUTO_TEST_SUITE_END()
