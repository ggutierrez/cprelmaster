#ifndef __CPREL_CPREL_TUPLE_HH__
#define __CPREL_CPREL_TUPLE_HH__

#include <cassert>
#include <vector>
#include <iostream>
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
    }
  /**
   * \brief Traits for valid tuple elements.
   */   
   namespace Traits {
    /*
      The only valid elements in tuples are integers. This trait is used
      during the instantiation of the makeTuple function to check that
      property.
    */
    template <typename T>
      struct validTupleElement {
        static const bool value = false;
      };
    /// validTupleElement specialization for int
    template <>
      struct validTupleElement<int> {
        static const bool value = true;
      };
    /// validTupleElement specialization for bool  
    template <>
      struct validTupleElement<bool> {
        static const bool value = true;
      };
    /// validTupleElement specialization for char
    template <>
      struct validTupleElement<char> {
        static const bool value = true;
      };
    /// validTupleElement specialization for unsigned int  
    template <>
      struct validTupleElement<unsigned int> {
        static const bool value = true;
      };
    }
  /**
   * \brief Class to abstract a tuple in a relation
   * \ingroup TupleGroup
   *
   * Tuples are usually inside relations. This implementation uses that to avoid storing
   * arity of the tuple internally. The only state stored is the BDD in which the tuple
   * is represented. This is the reason for the arity as an argument in some operations.
   */
   class Tuple {
   private:
    friend class VarImpl::RelationImpl;
    /// Actual data container
    BDD data_;
    /// Constructor from an existing representation \a t and an arity \a a
    Tuple(BDD data);
    /**
     * \brief Returns a BDD representation for the encoding of \a p in column \a a
     *
     * \warning Throws and exception of type RepresentationOverflow if \a p cannot
     * be represented with the current manager setup.
     */
     static BDD encodeElement(int p, int a);
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
    /// Avoiding Default constructor
    Tuple(void) = delete;
    /// Avoid assignement
    Tuple& operator = (const Tuple&) = delete;
    /// Copy constructor
    Tuple(const Tuple& t);
  private:   
    /// Base case for empty tuple construction
    BDD makeTuple() {
      static_assert(true,"Cannot create an empty tuple");
      return MPG::VarImpl::zero();
    }
    /// Tuple construction
    template <typename First, typename ...Rest>
    BDD makeTuple(First f, Rest ...rest) {
      static_assert(Traits::validTupleElement<First>::value,
                    "Invalid type for a tuple element");
      int col = sizeof...(rest);
      if (col > 0) {
        return  encodeElement(f,col) & makeTuple(rest...); 
      }
      else {
        return encodeElement(f, 0);
      }
    }
  public:
    /// Constructs a tuple from the list of arguments \a t
    template <typename First, typename ...Rest>
    Tuple(First f, Rest ...rest)
    : data_(makeTuple(f,rest...)) {}
    /**
     * \brief Construct a tuple with all the elements present in \a c.
     *
     * \a c can be any container in which we can iterate with a random access iterator.
     */
    template <typename C>
     Tuple(const C& c,  
           typename std::enable_if<
           std::is_integral< typename C::iterator::value_type>::value
           >::type* = 0 );
    /// Equality test
     bool operator == (const Tuple& t) const;
    /// Destructor
     ~Tuple(void);
    /**
     * \brief Returns a vector with all the elements in the represented tuple.
     *
     * As a tuple has no arity for space reasons then this method has to take it.
     * It is the callee responsibility to ensure the arity is accurate.
     */
     std::vector<int> value(int arity) const;
    /**
     * \brief Ouput the represented tuple in \a os.
     *
     * The arity of the tuple is not stored therfore it has to be taken as parameter \a arity.
     * The way in which the elements are printed in the tuple can be changed by specifying different
     * values for the spearator \a sep and for the opening \a op and closing \a cl characters.
     */
     void output(std::ostream& os, int arity, char sep = ',', char op = '[', char cl = ']') const;
   };

  template <typename C>
   BDD Tuple::encode(const C& c) const {
    BDD f = VarImpl::one();
    int col = c.size()-1;
    int i = 0;
    for (auto x = begin(c); x != end(c); ++x, i++) {
      f &= encodeElement(*x,col);
      col--;
    }
    return f;
  }

  template <typename C>
  Tuple::Tuple(const C& c,
               typename std::enable_if<
               std::is_integral< typename C::iterator::value_type>::value
               >::type*) 
  : data_(encode(c)) {
    assert(c.size() <= static_cast<unsigned int>(VarImpl::Limits::arity) &&
           "The manager was not configured to support this arity");
  }
}
#endif
