#ifndef __CPREL_CPREL_GRELATION_HH__
#define __CPREL_CPREL_GRELATION_HH__

#include <boost/shared_ptr.hpp>
#include <bdddomain/tuple.hh>
#include <bdddomain/perm-descriptor.hh>

namespace MPG {
  /**
   * \defgroup GRelation Ground relations
   *
   * Ground relations are the basic construct of the relation
   * domain. The term ground is used to differenciate this relation
   * from a relation represented by a decission variable. This module
   * offers the capability to define and operate on this kind of
   * relations.
   */

  /// Exception indicating invalid stream containing relation
  struct InvalidRelationSource : virtual ExceptionBase {};
  /// Exception indicating invalid arity according to configuration
  struct InvalidAritySource : virtual ExceptionBase {};
  /// Exception indicating invalid projection
  struct InvalidProjection : virtual ExceptionBase {};
  /// Exception indicating invalid join
  struct InvalidJoin: virtual ExceptionBase {};
  /// Exception indicating invalid follow
  struct InvalidFollow: virtual ExceptionBase {};

  namespace VarImpl {
    class RelationImpl;
  }

  class GRelationIter;

  /**
   * \brief Class representing a ground relation.
   * \ingroup GRelation
   *
   * This class provides a way to represent a relation along with
   * operations on it.  In all the documentation of this class \a this
   * refers to the represented relation. There are two basic notions
   * attached to a relation:
   *
   * - Its arity that is the number of elements that each tuple in the
   *   relation has.
   * - Its cardinality, which is the total number of tuple the
   *   relation contains.
   *
   * \f$arity(\text{this})\f$ and \f$|\text{this}|\f$ denote resp. the
   * arity and the cardinality of the represented relation.
   *
   * Relations can be large but finite. The largest possible relation
   * of arity \f$n\f$ is
   * \f$\mathcal{U}_{n}=\mathcal{U}\times\ldots\times\mathcal{U}\f$.
   * and \f$\mathcal{U}=\{x: 0 \leq x \leq k\} \f$ for an arbitrary
   * large \f$k\f$.
   */
  class GRelation {
  private:
    friend class GRelationIter;
    typedef boost::shared_ptr<VarImpl::RelationImpl> Impl;
    Impl pimpl_; ///> Relation storage
    /// Avoid default construction
    GRelation(void);
    /// Constructor taking an implementation
    explicit GRelation(Impl impl);
  public:
    /// \name Constructors, destructors and assignement
    //@{
    /// Constructor for an empty relation of arity \a a
    explicit GRelation(int a);
    /// Copy constructor
    GRelation(const GRelation& r);
    /// Assignment
    GRelation& operator=(GRelation& right);
    void become(const GRelation& other);

    /// Destructor
    ~GRelation(void);
    /// Constructs the relation \f$R=\mathcal{U}_{a}\f$
    static GRelation create_full(int a);
    //@}
    /// \name Modification operations
    //@{
    /**
     * \brief Adds tuple \a t to the relation. If \f$ t \in this \f$ the relation
     * remains unchanged.
     */
    void add(const Tuple& t);
    /**
     * \brief Adds the tuples contained in \a s to the relation.
     */
    void add(const std::vector<Tuple>& s);
    /**
     * \brief Union of relations: \f$ this = this \cup r \f$.
     */
    void unionAssign(const GRelation& r);
    /**
     * \brief Difference of relations: \f$ this = this \setminus r \f$.
     */
    void differenceAssign(const GRelation& r);
    //@}
    /// \name Set operations
    //@{
    /// Computes \f$ this \setminus r \f$
    GRelation difference(const GRelation& r) const;
    /// Computes \f$ this \cap r \f$
    GRelation intersect(const GRelation& r) const;
    /// Computes \f$ this \cup r \f$
    GRelation Union(const GRelation& r) const;
    /// Computes \f$ \overline{this}\f$
    GRelation complement(void) const;
    //@}
    /// \name Column permutation
    //@{
    /**
     * \brief Computes the permutation of \a this according to \a desc.
     *
     * \warning The permutation descriptor has to be valid for the relation. If it
     * is not then a InvalidPermDescriptor exception is thrown.
     */
    GRelation permute(const std::vector<std::pair<int,int>>& desc) const;
    /**
     * \brief Computes the permutation of \a this according to \a desc.
     *
     * \warning The permutation descriptor has to be valid for the relation. If it
     * is not then a InvalidPermDescriptor exception is thrown.
     */
    GRelation permute(const PermDescriptor& desc) const;
    /**
     * \brief Computes the relation resulting by shifting all the columns in \a r
     * \a n possitions to the right.
     *
     * The first \a n columns of \a r does not appear in the final relation.
     */
    GRelation shiftRight(int n) const;
    //@}
    /// \name Cross product
    //@{
    /**
     * \brief Computes \f$ \mathcal{U}_n \times this\f$.
     */
    GRelation timesULeft(int n) const;
    /**
     * \brief Computes \f$ this \times \mathcal{U}_n \f$.
     */
    GRelation timesURight(int n) const;
    /**
     * \brief Computes \f$ this \times r \f$
     */
    GRelation times(const GRelation& r) const;
    //@}
    /// \name Relational algebra operations
    //@{
    /**
     * \brief Returns: \f$ \mathit{this}\;\bowtie_{j}\; r \f$.
     *
     * This is, the result of joining the two relations on the \a j right most
     * columns of \a this and the \a j left most columns of \a r.
     */
    GRelation join(int j,const GRelation& r) const;
    /**
     * \brief Returns: \f$ \mathit{this}_{\smile_{f}}r \f$.
     *
     * \todo documentation
     */
    GRelation follow(int f,const GRelation& r) const;     
    /**
     * \brief Returns: \f$ \Pi_{p} this \f$.
     *
     * This is, the projection of \a this on the \a p rightmost columns.
     *
     * \warning Throws an exception InvalidProjection if \a p is not a valid column
     * in the relation.
     */
    GRelation project(int p) const;
    //@}
    /// \name Quantification
    //@{
    /**
     * \brief Returns the relation resulting from existencially quantifying on
     * column \a c
     *
     * \param c a column: \f$ 0 \leq c < \text{arity}(\text{this})\f$
     */
    GRelation exists(int c) const;
    /**
     * \brief Returns the relation resulting from uniquely quantifying on column
     * \a c
     *
     * \param c a column: \f$ 0 \leq c < \text{arity}(\text{this})\f$
     */
    GRelation unique(int c) const;
    /**
     * \brief Returns the relation resulting from uniquely quantifying on all the
     * columns in \a c.
     *
     * \param c a vector of columns: \f$ \forall_{i \in
     * \{0,\ldots,\text{size}(c)-1\}}: 0 \leq c[i] <
     * \text{arity}(\text{this})\f$
     */
    GRelation unique(const std::vector<int>& c) const;
    /**
     * \brief Returns the relation resulting from universaly quantifying on column
     * \a c
     *
     * \param c a column: \f$ 0 \leq c < \text{arity}(\text{this})\f$
     */
    GRelation forall(int c) const;
    //@}

    /**
     * \brief Computes the relation resulting by shifting all the columns in \a r
     * \a n possitions to the left.
     *
     * The new columns in the resulting relation are existentially quantified.
     */
    //GRelation shiftLeft(int n) const;
    /// Returns the relation \f$ this \times r \f$
    //@}
    /// \name Test operations
    //@{
    /// Tests \f$ this \subseteq r \f$
    bool subsetEq(const GRelation& r) const;
    /// Tests \f$ this \supset r \f$
    bool superset(const GRelation& r) const;
    /// Tests \f$ this \cap r = \emptyset \f$
    bool disjoint(const GRelation& r) const;
    /// Tests whether this represents the same relation as \a r
    bool eq(const GRelation& r) const;
    /// Tests whther the relation is empty
    bool empty(void) const;
    /// Tests whther the relation represents the universe
    bool universe(void) const;
    //@}
    /// \name Relation information
    //@{
    /// Returns the arity (i.e. number of columns) of the relation
    int arity(void) const;
    /// Returns the cardinality (i.e. number of tuples) of the relation
    double cardinality(void) const;
    //@}
    /// \name Constant relations
    //@{
    /// Creates the binary relation \f$ R = \{(x,y) : x = y \} \f$
    //static GRelation equalXY(void);
    //@}
    /// \name Content access
    //@{
    /**
     * \brief Returns one tuple represented by the relation.
     *
     * The only guarantee on the returned tuple is that it belongs to
     * the relation.
     */
    Tuple pickOneTuple(void) const;
    /// type definition for the functor that has to be passed to the visit methos
    typedef void (*Functor)(const std::vector<int>&);
    /// Visit every tuple in a relation and applies functor f on it. 
    void visit(Functor f);
    //@} 
    /// \name Output
    //{@
    void print(std::ostream& os) const;
    //@}
  };

    /**
     * \brief Creates a relation with the elements contained in \a dom.
     * \ingroup GRelation
     *
     * The arity of the created relation corresponds to the number of elements of the
     * provided tuples. All the tuples are assumed to be of the same arity.
     */
    GRelation create(const std::vector<Tuple>& dom);

    /**
     * \brief Creates a full relation of arity \a a.
     * \ingroup GRelation
     */
    GRelation create_full(int a);

    /**
     * \brief Creates a relation of arity \a arity from the contents read from \a is.
     * \ingroup GRelation
     *
     * The expected format in \a is is:
     * - Different tuples are sepparated by end of line (std::endl)
     * - Elements in each tuple are separated by one or more spaces
     * - Tuples are of exactly \a arity elements
     *
     * \warning An exception InvalidRelationSource is thrown if the stream is
     * either empty or failed.
     * \todo Perform some checking to guarantee that the elements that are read are
     * representable.
     * \example
     * \code
     std::ifstream input("/path/r.txt");
     // assumes that input contains a ternary relation
     GRelation ub = read(input,3);
     * \endcode
     */
  GRelation read(std::istream& is, int arity);

  /**
   * \brief Given a relation \a r and a set of columns \a u, return the
   * subrelation that is unique by only taking into account columns in
   * u.
   *
   * This non member function uses unique quantification and
   * internally selects the right columns to quantify on and
   * intersects the results with the input relation. It is a more
   * natural way of finding a subrelation which is unique.
   */
  GRelation unique(const GRelation& r, std::vector<int> u);

  /**
     * \brief Outputs relation \a r to \a os
     * \ingroup GRelation
     *
     * \todo The implementation of this function relies on an iterator on the tuples
     * of \a r. I should offer a better way to print the domain in a compressed way
     */
    std::ostream& operator<< (std::ostream& os, const GRelation& r);

    /**
     * \brief Iostream printing
     *
     * Basic properties the influence ground relation output.
     */
    class GRelationIO {
    private:
      /// String output when starting the printing
      std::string start_;
      /// String output at the end
      std::string end_;
      /// String used at the start of every value (tuple)
      std::string value_start_;
      /// String used at the end of every value (tuple)
      std::string value_end_;
      /// \name Current values for the output
      //@{
      /// Current string used at the begining of every value
      static std::string curr_start_;
      /// Current string when ending the printing
      static std::string curr_end_;
      /// Current string start value
      static std::string curr_value_start_;
      /// Current string end value
      static std::string curr_value_end_;
      //@}
      // Avoid default construction
      GRelationIO(void);
    public:
      /// Constructor
      GRelationIO(const char* relStart, const char* relEnd, const char* valStart,
		  const char* valEnd)
	: start_(relStart), end_(relEnd), value_start_(valStart), value_end_(valEnd) {}
      friend std::ostream& operator<< (std::ostream& os, const GRelation& r);
      friend std::ostream& operator<< (std::ostream& os, const GRelationIO& r);
    };

    /// Operator to change the ouput format of ground relations
    std::ostream& operator<< (std::ostream& os, const GRelationIO& f);

  }
#endif
