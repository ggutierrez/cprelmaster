#include <boost/test/unit_test.hpp>
#include <vector>
#include <cprel/tuple.hh>
#include <bdddomain/encoding.hh>
#include <bdddomain/rel-impl.hh>

BOOST_AUTO_TEST_SUITE(BddRelSuite )


BOOST_AUTO_TEST_CASE(EncodingBasic) {
  using namespace MPG::CPRel;
  using namespace MPG::CPRel::VarImpl;

  Tuple t0(1,2);

  // Test0: the encoding of a tuple must produce the same results as the conjunction
  //        of encoding all the members separatedly
  DdNode *et0 = encode(t0);
  // the number of minterms in the bdd should be 1 that corresponds to the [1,2]
  BOOST_CHECK_EQUAL(Cudd_CountMinterm(dd(),et0,2<<bbv()), 1);
  // Encode 2 for the column 0
  DdNode *c0 = encode(2,0);
  // Encode 1 for column 1
  DdNode *c1 = encode(1,1);
  DdNode *tuple = Cudd_bddAnd(dd(),c0,c1);
  Cudd_Ref(tuple);
  Cudd_RecursiveDeref(dd(),c0);
  Cudd_RecursiveDeref(dd(),c1);
  BOOST_CHECK_EQUAL(Cudd_CountMinterm(dd(),tuple,2<<bbv()), 1);
  BOOST_CHECK_EQUAL(tuple,et0);
  Cudd_RecursiveDeref(dd(),et0);

  // Test1: a relation created form the bdd representing the tuple must contain
  //        only one tuple
  RelationImpl x = RelationImpl::create_fromBdd(tuple,2);
  Cudd_RecursiveDeref(dd(),tuple);
  x.remove(x);
  BOOST_CHECK_EQUAL(x.cardinality(),0);
}

BOOST_AUTO_TEST_CASE(RelBasic) {
  using namespace MPG::CPRel;
  using namespace MPG::CPRel::VarImpl;

  RelationImpl r(2);
  RelationImpl empty(2);
  RelationImpl full = RelationImpl::create_full(2);

  BOOST_CHECK_EQUAL(r,empty);
  BOOST_CHECK(r.equal(empty));
  BOOST_CHECK(empty.equal(r));

  empty.complement();
  BOOST_CHECK_EQUAL(empty,full);
  empty.complement();
  full.complement();
  BOOST_CHECK_EQUAL(empty,full);
  full.complement();

  RelationImpl x = intersect(empty,r);
  BOOST_CHECK_EQUAL(x,empty);
  BOOST_CHECK_EQUAL(x.cardinality(),0);

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
