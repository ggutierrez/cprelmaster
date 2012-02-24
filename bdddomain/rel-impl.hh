#ifndef __CPREL_BDDDOMAIN_REL_IMPL_HH__
#define __CPREL_BDDDOMAIN_REL_IMPL_HH__


#include <bdddomain/manager.hh>
#include <bdddomain/tuple.hh>
#include <bdddomain/exception.hh>
#include <bdddomain/perm-descriptor.hh>
#include <bdddomain/bdd.hh>

namespace MPG { namespace VarImpl {
    /**
     * \defgroup DomRepr Domain representation
     *
     * This module provides the support to represent relation domains.
     */
    /// Stores the representation of a relation using BDDs
    class RelationImpl {
    private:
      /// The BDD with the representation
      BDD bdd_;
      /// The arity of the represented relation
      int arity_;
      /// Avoid default construction
      RelationImpl(void);
      /// Constructor from anexisting bdd
      RelationImpl(BDD n, int a);
    public:
      /// \name Constructors, destructor and assignement
      //@{
      /// Default constructor for an empty relation of arity \a a
      explicit RelationImpl(int a);
      /// Copy constructor
      RelationImpl(const RelationImpl& r);
      /// Asignement operator
      RelationImpl& operator=(const RelationImpl& right);
      /// Swap
      void swap(const RelationImpl& r);
      /// Create a full relation of arity \a a
      static RelationImpl create_full(int a);
      /**
       * \brief Creates a relation from a raw Bdd \a b and arity \a a.
       *
       * \warning The constructor that allows the creation from a raw bdd is private
       * for a good reason: it does not perform any check on the bdd or the given
       * arity. The idea behind this method is to bypass this restriction but it should
       * be used carefully.
       */
      static RelationImpl create_fromBdd(BDD b, int a);
      /// Destructor
      ~RelationImpl(void);
      //@}
      /// \name Modification
      //@{
      /// Adds tuple \a t to the relation.
      void add(const Tuple& t);
      /// Removes tuple \a t from the relation
      void remove(const Tuple& t);
      /// Adds relation \a r to this relation
      void add(const RelationImpl& r);
      /// Removes relation \a r to this relation
      void remove(const RelationImpl& r);
      /// Intersects the relation with relation \a r
      void intersect(const RelationImpl& r);
      /// Complement the represented relation
      void complement(void);
      //@}
      /// \name Information
      //@{
      /// Tests if relation \a r represents the same elements
      bool equal(const RelationImpl& r) const;
      /// Returns the current cardinality of the relation
      double cardinality(void) const;
      /// Returns the arity of the relation
      int arity(void) const;
      /// Tests if the represented relation is empty
      bool empty(void) const;
      /// Tests if the represented relation is the universe
      bool universe(void) const;
      //@}
      /// \name Existential quantification
      //@{
      /**
       * \brief Returns the relation resulting from existencially quantifying the
       * column \a c of the represented relation.
       */
      RelationImpl exists(int c) const;
      /**
       * \brief Returns the relation resulting from existentially
       * quantifying the column that correspond to indices in \a c of
       * this relation.
       */
      RelationImpl exists(const std::vector<int>& c) const;
      //@}
      /// \name Unique quantification
      //@{
      /**
       * \brief Returns the relation resulting from uniquely quantifying the column
       * \a c of the represented relation.
       */
      RelationImpl unique(int c) const;
      /**
       * \brief Returns the relation resulting from uniquely quantifying the columns
       * in \a c of the represented relation.
       */
      RelationImpl unique(const std::vector<int>& c) const;
      //@}
      /// \name Universal quantification
      //@{
      /**
       * \brief Returns the relation resulting from universaly quantifying on column
       * \a c
       *
       * \param c a column: \f$ 0 \leq c < \text{arity}(\text{this})\f$
       */
      RelationImpl forall(int c) const;
      //@}
      /// \name Column permutation
      //@{
    private:
      /**
       * \brief Returns a new relation implementation by performing
       * the specified replacement of columns indicated by \a pairing.
       *
       * Every component of \a pairning consists of two column indices
       * specifying: 1) the column to be replaced and 2) the column to
       * replace it with.
       *
       * This method overrides the data stored by the first column of
       * the pair. Also, the replacement is done at once and not
       * incrementally for every pair. This causes for instance the
       * pairing {<0,1>,<1,0>} two swap columns 1 and 0.
       */
      RelationImpl replace(const std::vector<std::pair<int,int> >& pairing);
      /**
       * \brief Returns a new variable implementation by swapping the
       * columns as indicated by \a pairing.
       *
       */
      RelationImpl swap(const std::vector<std::pair<int,int> >& pairing);
    public:
      /**
       * \brief Returns the relation resulting from permuting \a this
       * according with \a permDesc.
       *
       */
      RelationImpl permute(const std::vector<std::pair<int,int> >& perm) const;
      //@}
      /// \name Cross product
      //@{
      /// Returns U_{n} \times this
      RelationImpl timesULeft(int n) const;
      /// Returns this \times U_{n}
      RelationImpl timesURight(int n) const;      
      //@}
      /// \name Column manipulation
      //@{
      /**
       * \brief Returns a relation which is equivalent to discard the
       * columns in the range \a start (inclusive) to \a end
       * (exclusive) of this relation.
       */
      RelationImpl discard(int start, int end) const;
      /** 
       * \brief Returns the relation equivalent to shift this relation
       * \a n columns to the right.
       *
       * This implies that the right most \a n columns are discarded.
       */
      RelationImpl shiftRight(int n) const;
      //@}
      /// \name Relational algebra operations
      //@{
      /**
       * \brief Performs \f$ this \bowtie_{j} r \f$
       *
       */
      RelationImpl join(int j, const RelationImpl& r) const;
      /**
       * \brief Performs \f$ this_{\smile_{f}} r \f$
       *
       */
      RelationImpl follow(int f, const RelationImpl& r) const;
      /**
       * \brief Returns \f$ \Pi_{\{0,\ldots,p-1\}}\text{this}\f$.
       *
       * The projection of this on the \a p rightmost columns.
       */
      RelationImpl project(int p) const;
      //@}
      /// \name Relation output
      //@{
      /// Outputs the contents of the relation to \a os
      void print(std::ostream& os) const;
      //@}
      /// \name Element access
      //@{
      /// Returns a tuple in the relation
      Tuple pickOneTuple(void) const;
      /**
       * \brief Visits all the tuples of the relation and applies \a f.
       *
       * The functor must take as argument an std::vector<int> as a
       * constant reference. The size of the vector is the arity of
       * the relation.
       *
       * The vector has to be reversed for the values to reflect the
       * corresponding column indices.
       */
      template <typename Functor>
      void visit(Functor& fc) const;
      //@}     
    };
    
    namespace detail {
      typedef std::vector<std::vector<std::pair<int,int> > > branch;
      template <typename Functor>
      void flat(const branch& tuple, int i, std::vector<int>& val, int arity, Functor& fc) {
        if (i >= arity) {
          fc(val);
        return;
        }
        
        for (const auto& j : tuple.at(i)) {
          for (int e = j.first; e <= j.second; e++) {
            //int original = val.at(i);
            val[i] = e;
            flat(tuple,i+1,val,arity,fc);
            //val[i] = original;
          }
        }
      }
    }

    template <typename Functor>
    void RelationImpl::visit(Functor& fc) const {
      auto predicate = [=](const detail::branch& tuple) {
        std::vector<int> val(arity(),-2);
        detail::flat(tuple,0,val,arity(),fc);
      };
      traverseSet(VarImpl::factory(),bdd_,predicate);
    }
    
    /**
     * \brief Tests whether two relations are the same.
     * \ingroup DomRepr
     */
    inline
    bool operator==(const RelationImpl& r, const RelationImpl& s) {
      return r.equal(s);
    }

    /**
     * \brief Tests whether two relations are different
     * \ingroup DomRepr
     */
    inline
    bool operator!=(const RelationImpl& r, const RelationImpl& s) {
      return !r.equal(s);
    }

    /**
     * \brief Returns the union of relations \a r and \a s
     * \ingroup DomRepr
     */
    inline
    RelationImpl Union(const RelationImpl& r, const RelationImpl& s) {
      assert(r.arity() == s.arity());
      RelationImpl u(r);
      u.add(s);
      return u;
    }

    /**
     * \brief Returns the difference between relations \a r and \a s
     * \ingroup Domrepr
     */
    inline
    RelationImpl difference(const RelationImpl& r, const RelationImpl& s) {
      assert(r.arity() == s.arity());
      RelationImpl d(r);
      d.remove(s);
      return d;
    }

    /**
     * \brief Returns the intersection between relations \a r and \a s
     * \ingroup DomRepr
     */
    inline
    RelationImpl intersect(const RelationImpl& r, const RelationImpl& s) {
      assert(r.arity() == s.arity());
      RelationImpl i(r);
      i.intersect(s);
      return i;
    }

    /**
     * \brief Subset: \f$ r \subseteq s \f$
     * \ingroup DomRepr
     */
    inline
    bool subsetEq(const RelationImpl& r, const RelationImpl& s) {
      assert(r.arity() == s.arity());
      return intersect(r,s) == r;
    }

    /**
     * \brief Superset: \f$ r \supseteq s \f$
     * \ingroup DomRepr
     */
    inline
    bool supersetEq(const RelationImpl& r, const RelationImpl& s) {
      return subsetEq(s,r);
    }

    /**
     * \brief Proper subset: \f$ r \subset s \f$
     * \ingroup DomRepr
     */
    inline
    bool subset(const RelationImpl& r, const RelationImpl& s) {
      assert(r.arity() == s.arity());
      return subsetEq(r,s) && r != s;
    }

    /**
     * \brief Proper superset: \f$ r \supset s \f$
     * \ingroup DomRepr
     */
    inline
    bool superset(const RelationImpl& r, const RelationImpl& s) {
      return subset(s,r);
    }

    /**
     * \brief Disjoint: \f$ r \cap s = \emptyset \f$
     * \ingroup DomRepr
     */
    inline
    bool disjoint(const RelationImpl& r, const RelationImpl& s) {
      return intersect(r,s).empty();
    }

    /**
     * \brief Returns the complement of relation \a r
     * \ingroup DomRepr
     */
    inline
    RelationImpl complement(const RelationImpl& r) {
      RelationImpl c(r);
      c.complement();
      return c;
    }

    /**
     * \brief Output a relation implementation \a r in tuple format to \a os
     * \ingroup DomRepr
     */
    std::ostream& operator << (std::ostream& os, const RelationImpl& r);
  }
}

#endif
