#ifndef __CPREL_BDDDOMAIN_REL_IMPL_HH__
#define __CPREL_BDDDOMAIN_REL_IMPL_HH__


#include <bdddomain/manager.hh>
#include <bdddomain/tuple.hh>
#include <bdddomain/exception.hh>
#include <bdddomain/perm-descriptor.hh>

namespace MPG { namespace VarImpl {
    /**
     * \defgroup DomRepr Domain representation
     *
     * This module provides the support to represent relation domains.
     */
    class RelationImplIter;
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

      //@}
      /// \name Column permutation
      //@{
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
      RelationImpl replace(const std::vector<std::pair<int,int>>& pairing);
      /**
       * \brief Returns a new variable implementation by swapping the
       * columns as indicated by \a pairing.
       *
       */
      RelationImpl swap(const std::vector<std::pair<int,int>>& pairing);
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
      RelationImpl discard(int start, int end) const;
      //@}
      /// \name Operations
      /**
       * \brief Returns the relation resulting from permuting \a this
       * according with \a permDesc.
       *
       */
      RelationImpl permute(const PermDescriptor& permDesc) const;

      RelationImpl shiftLeft(int n) const;
      RelationImpl shiftRight(int n) const;
      /**
       * \brief Computes the cross product of \a this with \f$ \mathcal{U}_n \f$.
       *
       * As the cross product operation is not conmutative, the \a left parameter
       * indicates whether to perform  \f$ \mathcal{U}_n \times \mathrm{this}\f$ if
       * \a left is  \a true or \f$ \mathrm{this} \times \mathcal{U}_n\f$ otherwise.
       */
      RelationImpl timesU(int n, bool left) const;
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
       * \brief Returns the relation resulting from uniquely quantifying the column
       * \a c of the represented relation.
       */
      RelationImpl unique(int c) const;
      /**
       * \brief Returns the relation resulting from uniquely quantifying the columns
       * in \a c of the represented relation.
       */
      RelationImpl unique(const std::vector<int>& c) const;
      /**
       * \brief Returns the relation resulting from universaly quantifying on column
       * \a c
       *
       * \param c a column: \f$ 0 \leq c < \text{arity}(\text{this})\f$
       */
      RelationImpl forall(int c) const;
      /**
       * \brief Returns \f$ \Pi_{\{0,\ldots,p-1\}}\text{this}\f$.
       *
       * The projection of this on the \a p rightmost columns.
       */
      RelationImpl project(int p) const;
      /**
       * \brief Returns \f$\Pi_{\overline{c}}this \f$.
       *
       * The projection of \a this on all its columns but \a c
       */
      //bin/RelationImpl projectBut(int c) const;
      /// \name Output
      void print(std::ostream& os) const;
    };

    
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
