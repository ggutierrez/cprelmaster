#ifndef __CPREL_CPREL_TUPLE_HH__
#define __CPREL_CPREL_TUPLE_HH__

#include <cassert>
#include <vector>
#include <iostream>
#include <initializer_list>
#include <utility>
#include <type_traits>
#include <bdddomain/manager.hh>

namespace MPG {
  /**
   * \defgroup TupleGroup Tuples
   *
   * Tuples are the abstraction for the elements of relations.
   */
  namespace VarImpl {
    /*
      These forward declarations are needed because of the friend relation between
      RelationImpl and RelationImplIter with the Tuple class. I do not like declaring
      friends just for these classes to access the encode function of the tuple, it
      would be better to provide a conversion operator from Tuple to DdNode for this
      purpose.
    */
    class RelationImpl;
    class RelationImplIter;
  }
  /**
   * \brief Class to abstract a tuple in a relation
   * \ingroup TupleGroup
   *
   * \todo For now a tuple is represented as a vector of elements and this can be
   * better if we use pimp and usa abdd for instance to store it.
   */
  class Tuple {
  private:
    friend class VarImpl::RelationImpl;
    /// Actual data container
    BDD data_;
    /// Arity of the tuple
    int arity_;
    /// Avoiding Default constructor
    Tuple(void);
    /// Constructor from an existing representation \a t and an arity \a a
    Tuple(BDD data, int arity);
    /**
     * \brief Returns a BDD representation for the encoding of \a p in column \a a
     *
     * \warning Throws and exception of type RepresentationOverflow if \a p cannot
     * be represented with the current manager setup.
     */
    static BDD encode(int p, int a);
    /**
     * \brief Returns a BDD representing \a this
     *
     * \warning It is the responsability of the calle to handle the reference
     * counting of the returned BDD. For instance, to call Cudd_Ref before using
     * it and Cudd_RecursiveDeref when not needed anymore.
     */
    BDD getBDD(void) const;
    /**
     * \brief Returns a Bdd that represents all the elements in collection \a c.
     *
     * \a C is expected to have a method size that returns the legnth
     * of the collection and also is expected to provide \a begin and
     * \a end iterators.
     */ 
    template <typename C>
    BDD encode(const C& c) const;
  public:
    /**
     * \brief Construct a tuple with all the elements present in \a v. The arity
     * of the tuple is the size of the vector.
     */
    template <typename C>
    Tuple(const C& c,  
          typename std::enable_if<
            std::is_integral< typename C::iterator::value_type>::value
            >::type* = 0 );
    /**
     * \brief Construct a tuple with all the elements present in \a l. The arity
     * of the tuple is the size of the list.
     *
     * The constructor is not explicit to encourage conversion from initializer
     * lists to tuples.
     */
    Tuple(std::initializer_list<int> l);
    //template <typename C>
    //Tuple(const C& c);
    /// Copy constructor
    Tuple(const Tuple& t);
    /// Assignement operator
    Tuple& operator = (const Tuple& t);
    /// Equality test
    bool operator == (const Tuple& t) const;
    /// Destructor
    ~Tuple(void);
    /// Returns a vector with all the elements in the tuple
    std::vector<int> value(void) const;
    /// Arity of the tuple
    int arity(void) const { return arity_; }
  };

  template <typename C>
  BDD Tuple::encode(const C& c) const {
    BDD f = VarImpl::one();
    int col = c.size()-1;
    int i = 0;
    for (auto x = begin(c); x != end(c); ++x, i++) {
      f &= encode(*x,col);
      col--;
    }
    return f;
  }

  template <typename C>
  Tuple::Tuple(const C& c,
          typename std::enable_if<
            std::is_integral< typename C::iterator::value_type>::value
            >::type*) 
    : data_(encode(c)), arity_(c.size()) {
    assert(c.size() <= static_cast<unsigned int>(VarImpl::Limits::arity)
	   && "The manager was not configured to support this arity");
  }

  /// Creates a binary tuple with \a a
  inline
  Tuple make_Tuple(int a) {
    std::vector<int> v;
    v.reserve(1);
    v.push_back(a);
    return Tuple(v);
  }

  /// Creates a binary tuple with \a a and \a b
  inline
  Tuple make_Tuple(int a, int b) {
    std::vector<int> v;
    v.reserve(2);
    v.push_back(a); v.push_back(b);
    return Tuple(v);
  }

  /// Creates a ternary tuple with \a a, \a b and \a c
  inline
  Tuple make_Tuple(int a, int b, int c) {
    std::vector<int> v;
    v.reserve(3);
    v.push_back(a); v.push_back(b); v.push_back(c);
    return Tuple(v);
  }

  /**
   * \brief Iostream printing of tuples
   *
   * Basic properties the influence tuple output.
   */
  class TupleIO {
  private:
    /// String output when starting the printing
    std::string start_;
    /// String output at the end
    std::string end_;
    /// String used to separate the values
    std::string value_separator_;
    /// \name Current values for the output
    //@{
    /// Current string used at the begining of every value
    static std::string curr_start_;
    /// Current string when ending the printing
    static std::string curr_end_;
    /// Current separator for values
    static std::string curr_value_separator_;
    //@}
    // Avoid default construction
    TupleIO(void);
  public:
    /// Constructor
    TupleIO(const char* valStart, const char* valEnd, const char* valSep)
      : start_(valStart), end_(valEnd), value_separator_(valSep) {}
    friend std::ostream& operator<< (std::ostream& os, const Tuple& r);
    friend std::ostream& operator<< (std::ostream& os, const TupleIO& r);
  };

  /// Operator to change the ouput format of ground relations
  std::ostream& operator<< (std::ostream& os, const TupleIO& f);

  /**
   * \brief Outputs tuple \a t to \a os
   * \ingroup TupleGroup
   */
  std::ostream& operator << (std::ostream& os, const Tuple& t);
}
#endif
