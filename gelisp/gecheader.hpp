#ifndef __GELISPIV_HPP__
#define __GELISPIV_HPP__

/************************************************int.hh*************************************************************/


class IntVar {
public:
  /// \name Value access
  //@{
  /// Return minimum of domain
  int min(void) const;
  /// Return maximum of domain
  int max(void) const;
  /// Return median of domain (greatest element not greater than the median)
  int med(void) const;
  /**
   * \brief Return assigned value
   *
   * Throws an exception of type Int::ValOfUnassignedVar if variable
   * is not yet assigned.
   *
   */
  int val(void) const;
	
  /// Return size (cardinality) of domain
  unsigned int size(void) const;
  /// Return width of domain (distance between maximum and minimum)
  unsigned int width(void) const;
  /// Return regret of domain minimum (distance to next larger value)
  unsigned int regret_min(void) const;
  /// Return regret of domain maximum (distance to next smaller value)
  unsigned int regret_max(void) const;
  //@}
	
  /// \name Domain tests
  //@{
  /// Test whether domain is a range
  bool range(void) const;
  /// Test whether view is assigned
  bool assigned(void) const;
	
  /// Test whether \a n is contained in domain
  bool in(int n) const;
  //@}
};

 /**
   * \brief Boolean integer variables
   *
   * \ingroup TaskModelIntVars
   */
  class BoolVar {
  /*private:
    using VarImpVar<Int::BoolVarImp>::x;*/
    /**
     * \brief Initialize Boolean variable with range domain
     *
     * The variable is created with a domain ranging from \a min
     * to \a max. No exceptions are thrown.
     */
    //void _init(Space& home, int min, int max);
  public:
    /// \name Constructors and initialization
    //@{
    /// Default constructor
    //BoolVar(void);
    /// Initialize from Boolean variable \a y
    //BoolVar(const BoolVar& y);
    /// Initialize from Boolean view \a y
    //BoolVar(const Int::BoolView& y);
    /**
     * \brief Initialize Boolean variable with range domain
     *
     * The variable is created with a domain ranging from \a min
     * to \a max. The following exceptions might be thrown:
     *  - If \a min is greater than \a max, an exception of type
     *    Gecode::Int::VariableEmptyDomain is thrown.
     *  - If \a min is less than 0 or \a max is greater than 1,
     *    an exception of type
     *    Gecode::Int::NotZeroOne is thrown.
     */
    // BoolVar(Space& home, int min, int max);
    //@}

    /// \name Value access
    //@{
    /// Return minimum of domain
    int min(void) const;
    /// Return maximum of domain
    int max(void) const;
    /// Return median of domain (greatest element not greater than the median)
    int med(void) const;
    /**
     * \brief Return assigned value
     *
     * Throws an exception of type Int::ValOfUnassignedVar if variable
     * is not yet assigned.
     *
     */
    int val(void) const;

    /// Return size (cardinality) of domain
    unsigned int size(void) const;
    /// Return width of domain (distance between maximum and minimum)
    unsigned int width(void) const;
    /// Return regret of domain minimum (distance to next larger value)
    unsigned int regret_min(void) const;
    /// Return regret of domain maximum (distance to next smaller value)
    unsigned int regret_max(void) const;
    //@}

    /// \name Domain tests
    //@{
    /// Test whether domain is a range
    bool range(void) const;
    /// Test whether \a n is contained in domain
    bool in(int n) const;
    //@}

    /// \name Boolean domain tests
    //@{
    /// Test whether domain is zero
    bool zero(void) const;
    /// Test whether domain is one
    bool one(void) const;
    /// Test whether domain is neither zero nor one
    bool none(void) const;
    //@}
  };


class IntSet {
  /*private:
    /// %Range (intervals) of integers
    class Range {
    public:
      int min, max;
    };
    class IntSetObject : public SharedHandle::Object {
    public:
      /// Size of set
      unsigned int size;
      /// Number of ranges
      int n;
      /// Array of ranges
      Range* r;
      /// Allocate object with \a m elements
       static IntSetObject* allocate(int m);
      /// Return copy of object
       SharedHandle::Object* copy(void) const;
      /// Check whether \a n is included in the set
       bool in(int n) const;
      /// Delete object
       virtual ~IntSetObject(void);
    };
    /// Sort ranges according to increasing minimum
    class MinInc;
    /// Normalize the first \a n elements of \a r
    void normalize(Range* r, int n);
    /// Initialize as range with minimum \a n and maximum \a m
    void init(int n, int m);
    /// Initialize with \a n integers from array \a r
    void init(const int r[], int n);
    /// Initialize with \a n ranges from array \a r
    void init(const int r[][2], int n);*/
  public:
    /// \name Constructors and initialization
    //@{
    /// Initialize as empty set
    IntSet(void);
    /** \brief Initialize as range with minimum \a n and maximum \a m
     *
     * Note that the set is empty if \a n is larger than \a m
     */
    IntSet(int n, int m);
    /// Initialize with \a n integers from array \a r
    IntSet(const int r[],   int n);
    /** \brief Initialize with \a n ranges from array \a r
     *
     * For position \a i in the array \a r, the minimum is \a r[\a i][0]
     * and the maximum is \a r[\a i][1].
     */
    IntSet(const int r[][2], int n);
    /// Initialize with range iterator \a i
    template<class I>
    explicit IntSet(I& i);
#ifdef __INTEL_COMPILER
    /// Initialize with integer set \a s
    IntSet(const IntSet& s);
    /// Initialize with integer set \a s
    IntSet(IntSet& s);
    /// Initialize with integers \a i
    IntSet(const PrimArgArray<int>& i);
    /// Initialize with integers \a i
    IntSet(PrimArgArray<int>& i);
#endif
    //@}

    /// \name Range access
    //@{
    /// Return number of ranges of the specification
    int ranges(void) const;
    /// Return minimum of range at position \a i
    int min(int i) const;
    /// Return maximum of range at position \a i
    int max(int i) const;
    /// Return width of range at position \a i
    unsigned int width(int i) const;
    //@}

    /// \name Entire set access
    //@{
    /// Return whether \a n is included in the set
    bool in(int n) const;
    /// Return size (cardinality) of set
    unsigned int size(void) const;
    /// Return width of set (distance between maximum and minimum)
    unsigned int width(void) const;
    /// Return minimum of entire set
    int min(void) const;
    /// Return maximum of entire set
    int max(void) const;
    //@}

    /// \name Predefined value
    //@{
    /// Empty set
    static const IntSet empty;
    //@}
  };
  

/**
   * \brief Relation types for integers
   * \ingroup TaskModelInt
   */
  enum IntRelType {
    IRT_EQ, ///< Equality (\f$=\f$)
    IRT_NQ, ///< Disequality (\f$\neq\f$)
    IRT_LQ, ///< Less or equal (\f$\leq\f$)
    IRT_LE, ///< Less (\f$<\f$)
    IRT_GQ, ///< Greater or equal (\f$\geq\f$)
    IRT_GR  ///< Greater (\f$>\f$)
  };

  /**
   * \brief Operation types for Booleans
   * \ingroup TaskModelInt
   */
  enum BoolOpType {
    BOT_AND, ///< Conjunction
    BOT_OR,  ///< Disjunction
    BOT_IMP, ///< Implication
    BOT_EQV, ///< Equivalence
    BOT_XOR  ///< Exclusive or
  };

  /**
   * \brief Consistency levels for integer propagators
   *
   * The descriptions are meant to be suggestions. It is not
   * required that a propagator achieves full domain consistency or
   * full bounds consistency. It is more like: which level
   * of consistency comes closest.
   *
   * If in the description of a constraint below no consistency level
   * is mentioned, the propagator for the constraint implements
   * domain consistency.
   * \ingroup TaskModelInt
   */
  enum IntConLevel {
    ICL_VAL, ///< Value propagation or consistency (naive)
    ICL_BND, ///< Bounds propagation or consistency
    ICL_DOM, ///< Domain propagation or consistency
    ICL_DEF  ///< The default consistency for a constraint
  };

/// Propagates \f$ x=n\f$
void dom(Space& home, IntVar x, int n, IntConLevel icl=ICL_DEF);

void dom(Space& home, const IntVarArgs& x, int n, IntConLevel icl=ICL_DEF);

/// Propagates \f$ l\leq x\leq m\f$
void dom(Space& home, IntVar x, int l, int m, IntConLevel icl=ICL_DEF);

/// Propagates \f$ l\leq x_i\leq m\f$ for all \f$0\leq i<|x|\f$
void dom(Space& home, const IntVarArgs& x, int l, int m, IntConLevel icl=ICL_DEF);

/// Propagates \f$ x\in s \f$
void dom(Space& home, IntVar x, const IntSet& s, IntConLevel icl=ICL_DEF);

/// Propagates \f$ x_i\in s\f$ for all \f$0\leq i<|x|\f$
void dom(Space& home, const IntVarArgs& x, const IntSet& s, IntConLevel icl=ICL_DEF);

/// Post propagator for \f$ (x=n) \Leftrightarrow b\f$
void dom(Space& home, IntVar x, int n, BoolVar b, IntConLevel icl=ICL_DEF);

/// Post propagator for \f$ (l\leq x \leq m) \Leftrightarrow b\f$
void dom(Space& home, IntVar x, int l, int m, BoolVar b, IntConLevel icl=ICL_DEF);
/// Post propagator for \f$ (x \in s) \Leftrightarrow b\f$
void dom(Space& home, IntVar x, const IntSet& s, BoolVar b, IntConLevel icl=ICL_DEF);


/**
   * \defgroup TaskModelIntRelInt Simple relation constraints over integer variables
   * \ingroup TaskModelInt
   */
  /** \brief Post propagator for \f$ x_0 \sim_r x_1\f$
   *
   * Supports both bounds (\a icl = ICL_BND) and
   * domain consistency (\a icl = ICL_DOM, default).
   * \ingroup TaskModelIntRelInt
   */
  void rel(Space& home, IntVar x0, IntRelType r, IntVar x1, IntConLevel icl=ICL_DEF);
  
  /** \brief Post propagators for \f$ x_i \sim_r y \f$ for all \f$0\leq i<|x|\f$
   *
   * Supports both bounds (\a icl = ICL_BND) and
   * domain consistency (\a icl = ICL_DOM, default).
   * \ingroup TaskModelIntRelInt
   */
  void rel(Space& home, const IntVarArgs& x, IntRelType r, IntVar y, IntConLevel icl=ICL_DEF);
  
  /** \brief Propagates \f$ x \sim_r c\f$
   * \ingroup TaskModelIntRelInt
   */
   void rel(Space&, IntVar x, IntRelType r, int c, IntConLevel icl=ICL_DEF);
  /** \brief Propagates \f$ x_i \sim_r c \f$ for all \f$0\leq i<|x|\f$
   * \ingroup TaskModelIntRelInt
   */
	void rel(Space& home, const IntVarArgs& x, IntRelType r, int c, IntConLevel icl=ICL_DEF);
	
	/** \brief Post propagator for \f$ (x_0 \sim_r x_1)\Leftrightarrow b\f$
   *
   * Supports both bounds (\a icl = ICL_BND) and
   * domain consistency (\a icl = ICL_DOM, default).
   * \ingroup TaskModelIntRelInt
   */
	void rel(Space& home, IntVar x0, IntRelType r, IntVar x1, BoolVar b, IntConLevel icl=ICL_DEF);
  /** \brief Post propagator for \f$(x \sim_r c)\Leftrightarrow b\f$
   *
   * Supports both bounds (\a icl = ICL_BND) and
   * domain consistency (\a icl = ICL_DOM, default).
   * \ingroup TaskModelIntRelInt
   */
   void rel(Space& home, IntVar x, IntRelType r, int c, BoolVar b, IntConLevel icl=ICL_DEF);
  
  
  
  
  
  
	void rel(Space& home, const IntVarArgs& x, IntRelType r, IntConLevel icl=ICL_DEF);




  /** \brief Post propagator for relation between \a x and \a y.
   *
   * Note that for the inequality relations this corresponds to
   * the lexical order between \a x and \a y.
   *
   * Supports both bounds (\a icl = ICL_BND) and
   * domain consistency (\a icl = ICL_DOM, default).
   *
   * Throws an exception of type Int::ArgumentSizeMismatch, if
   * \a x and \a y are of different size.
   * \ingroup TaskModelIntRelInt
   */
   
void rel(Space& home, const IntVarArgs& x, IntRelType r, const IntVarArgs& y, IntConLevel icl=ICL_DEF);


/**
   * \defgroup TaskModelIntRelBool Simple relation constraints over Boolean variables
   * \ingroup TaskModelInt
   */
  /** \brief Post propagator for \f$ x_0 \sim_r x_1\f$
   * \ingroup TaskModelIntRelBool
   */
  void rel(Space& home, BoolVar x0, IntRelType r, BoolVar x1, IntConLevel icl=ICL_DEF);
  /** \brief Post propagator for \f$(x_0 \sim_r x_1)\Leftrightarrow b\f$
   * \ingroup TaskModelIntRelBool
   */
   void rel(Space& home, BoolVar x0, IntRelType r, BoolVar x1, BoolVar b, IntConLevel icl=ICL_DEF);
  /** \brief Post propagator for \f$ x_i \sim_r y \f$ for all \f$0\leq i<|x|\f$
   * \ingroup TaskModelIntRelBool
   */
   void rel(Space& home, const BoolVarArgs& x, IntRelType r, BoolVar y, IntConLevel icl=ICL_DEF);
  /**
   * \brief Propagates \f$ x \sim_r n\f$
   *
   * Throws an exception of type Int::NotZeroOne, if \a n is neither
   * 0 or 1.
   * \ingroup TaskModelIntRelBool
   */
   void rel(Space& home, BoolVar x, IntRelType r, int n, IntConLevel icl=ICL_DEF);
  /**
   * \brief Propagates \f$(x \sim_r n)\Leftrightarrow b\f$
   *
   * Throws an exception of type Int::NotZeroOne, if \a n is neither
   * 0 or 1.
   * \ingroup TaskModelIntRelBool
   */
   void rel(Space& home, BoolVar x, IntRelType r, int n, BoolVar b, IntConLevel icl=ICL_DEF);
  /**
   * \brief Propagates \f$ x_i \sim_r n \f$ for all \f$0\leq i<|x|\f$
   *
   * Throws an exception of type Int::NotZeroOne, if \a n is neither
   * 0 or 1.
   * \ingroup TaskModelIntRelBool
   */
   void rel(Space& home, const BoolVarArgs& x, IntRelType r, int n, IntConLevel icl=ICL_DEF);
   
  /** \brief Post propagator for relation between \a x and \a y.
   *
   * Note that for the inequality relations this corresponds to
   * the lexical order between \a x and \a y.
   *
   * Throws an exception of type Int::ArgumentSizeMismatch, if
   * \a x and \a y are of different size.
   * \ingroup TaskModelIntRelBool
   */
   void rel(Space& home, const BoolVarArgs& x, IntRelType r, const BoolVarArgs& y, IntConLevel icl=ICL_DEF);
  /** \brief Post propagator for pairwise relation on \a x.
   *
   * States that the elements of \a x are in the following relation:
   *  - if \a r = IRT_EQ, then all elements of \a x must be equal.
   *  - if \a r = IRT_LE, \a r = IRT_LQ, \a r = IRT_GR, or \a r = IRT_GQ,
   *    then the elements of \a x are ordered with respt to \a r.
   *  - if \a r = IRT_NQ, then all elements of \a x must be pairwise
   *    distinct (corresponds to the distinct constraint).
   *
   * \ingroup TaskModelIntRelBool
   */
   void rel(Space& home, const BoolVarArgs& x, IntRelType r, IntConLevel icl=ICL_DEF);
  /** \brief Post propagator for Boolean operation on \a x0 and \a x1
   *
   * Posts propagator for \f$ x_0 \diamond_{\mathit{o}} x_1 = x_2\f$
   * \ingroup TaskModelIntRelBool
   */
   void rel(Space& home, BoolVar x0, BoolOpType o, BoolVar x1, BoolVar x2, IntConLevel icl=ICL_DEF);
  /** \brief Post propagator for Boolean operation on \a x0 and \a x1
   *
   * Posts propagator for \f$ x_0 \diamond_{\mathit{o}} x_1 = n\f$
   *
   * Throws an exception of type Int::NotZeroOne, if \a n is neither
   * 0 or 1.
   * \ingroup TaskModelIntRelBool
   */
   void rel(Space& home, BoolVar x0, BoolOpType o, BoolVar x1, int n, IntConLevel icl=ICL_DEF);
  /** \brief Post propagator for Boolean operation on \a x
   *
   * Posts propagator for \f$ x_0 \diamond_{\mathit{o}} \cdots
   * \diamond_{\mathit{o}} x_{|x|-1}= y\f$
   *
   * Throws an exception of type Int::TooFewArguments, if \f$|x|<2\f$
   * and \a o is BOT_IMP, BOT_EQV, or BOT_XOR.
   * \ingroup TaskModelIntRelBool
   */
   void rel(Space& home, BoolOpType o, const BoolVarArgs& x, BoolVar y, IntConLevel icl=ICL_DEF);
  /** \brief Post propagator for Boolean operation on \a x
   *
   * Posts propagator for \f$ x_0 \diamond_{\mathit{o}} \cdots
   * \diamond_{\mathit{o}} x_{|x|-1}= n\f$
   *
   * Throws an exception of type Int::NotZeroOne, if \a n is neither
   * 0 or 1.
   *
   * Throws an exception of type Int::TooFewArguments, if \f$|x|<2\f$
   * and \a o is BOT_IMP, BOT_EQV, or BOT_XOR.
   * \ingroup TaskModelIntRelBool
   */
   void rel(Space& home, BoolOpType o, const BoolVarArgs& x, int n, IntConLevel icl=ICL_DEF);
  /** \brief Post propagator for Boolean clause with positive variables \a x and negative variables \a y
   *
   * Posts propagator for \f$ x_0 \diamond_{\mathit{o}} \cdots
   * \diamond_{\mathit{o}} x_{|x|-1} \diamond_{\mathit{o}} \neg y_0
   * \diamond_{\mathit{o}} \cdots \diamond_{\mathit{o}} \neg y_{|y|-1}= z\f$
   *
   * Throws an exception of type Int::IllegalOperation, if \a o is different
   * from BOT_AND or BOT_OR.
   * \ingroup TaskModelIntRelBool
   */
   void clause(Space& home, BoolOpType o, const BoolVarArgs& x, const BoolVarArgs& y, BoolVar z, IntConLevel icl=ICL_DEF);
  /** \brief Post propagator for Boolean clause with positive variables \a x and negative variables \a y
   *
   * Posts propagator for \f$ x_0 \diamond_{\mathit{o}} \cdots
   * \diamond_{\mathit{o}} x_{|x|-1} \diamond_{\mathit{o}} \neg y_0
   * \diamond_{\mathit{o}} \cdots \diamond_{\mathit{o}} \neg y_{|y|-1}= n\f$
   *
   * Throws an exception of type Int::NotZeroOne, if \a n is neither
   * 0 or 1.
   *
   * Throws an exception of type Int::IllegalOperation, if \a o is different
   * from BOT_AND or BOT_OR.
   * \ingroup TaskModelIntRelBool
   */
   void clause(Space& home, BoolOpType o, const BoolVarArgs& x, const BoolVarArgs& y, int n, IntConLevel icl=ICL_DEF);


/**
   * \defgroup TaskModelIntElement Element constraints
   * \ingroup TaskModelInt
   */

  //@{
  /// Arrays of integers that can be shared among several element constraints
  //typedef SharedArray<int> IntSharedArray;
  /** \brief Post propagator for \f$ n_{x_0}=x_1\f$
   *
   *  Throws an exception of type Int::OutOfLimits, if
   *  the integers in \a n exceed the limits in Int::Limits.
   */
   //void element(Space& home, IntSharedArray n, IntVar x0, IntVar x1, IntConLevel icl=ICL_DEF);
  /** \brief Post propagator for \f$ n_{x_0}=x_1\f$
   *
   *  Throws an exception of type Int::OutOfLimits, if
   *  the integers in \a n exceed the limits in Int::Limits.
   */
  /* void
  element(Space& home, IntSharedArray n, IntVar x0, BoolVar x1,
          IntConLevel icl=ICL_DEF);*/
  /** \brief Post propagator for \f$ n_{x_0}=x_1\f$
   *
   *  Throws an exception of type Int::OutOfLimits, if
   *  the integers in \a n exceed the limits in Int::Limits.
   */
   //void element(Space& home, IntSharedArray n, IntVar x0, int x1,IntConLevel icl=ICL_DEF);
   
  /** \brief Post propagator for \f$ x_{y_0}=y_1\f$
   *
   * Supports both bounds (\a icl = ICL_BND) and
   * domain consistency (\a icl = ICL_DOM, default).
   */
   void element(Space& home, const IntVarArgs& x, IntVar y0, IntVar y1, IntConLevel icl=ICL_DEF);
  /** \brief Post propagator for \f$ x_{y_0}=y_1\f$
   *
   * Supports both bounds (\a icl = ICL_BND) and
   * domain consistency (\a icl = ICL_DOM, default).
   */
   void element(Space& home, const IntVarArgs& x, IntVar y0, int y1, IntConLevel icl=ICL_DEF);
  /// Post propagator for \f$ x_{y_0}=y_1\f$
  void element(Space& home, const BoolVarArgs& x, IntVar y0, BoolVar y1, IntConLevel icl=ICL_DEF);
  /// Post propagator for \f$ x_{y_0}=y_1\f$
  void element(Space& home, const BoolVarArgs& x, IntVar y0, int y1, IntConLevel icl=ICL_DEF);

  /** \brief Post propagator for \f$ a_{x+w\cdot y}=z\f$
   *
   * If \a a is regarded as a two-dimensional array in row-major
   * order of width \a w and height \a h, then \a z is constrained
   * to be the element in column \a x and row \a y.
   *
   * Throws an exception of type Int::OutOfLimits, if
   * the integers in \a n exceed the limits in Int::Limits.
   *
   * Throws an exception of type Int::ArgumentSizeMismatch, if
   * \f$ w\cdot h\neq|a|\f$.
   */
   /*void
  element(Space& home, IntSharedArray a, 
          IntVar x, int w, IntVar y, int h, IntVar z,
          IntConLevel icl=ICL_DEF);*/
  /** \brief Post propagator for \f$ a_{x+w\cdot y}=z\f$
   *
   * If \a a is regarded as a two-dimensional array in row-major
   * order of width \a w and height \a h, then \a z is constrained
   * to be the element in column \a x and row \a y.
   *
   * Throws an exception of type Int::OutOfLimits, if
   * the integers in \a n exceed the limits in Int::Limits.
   *
   * Throws an exception of type Int::ArgumentSizeMismatch, if
   * \f$ w\cdot h\neq|a|\f$.
   */
  /* void
  element(Space& home, IntSharedArray a, 
          IntVar x, int w, IntVar y, int h, BoolVar z,
          IntConLevel icl=ICL_DEF);*/
  /** \brief Post propagator for \f$ a_{x+w\cdot y}=z\f$
   *
   * If \a a is regarded as a two-dimensional array in row-major
   * order of width \a w and height \a h, then \a z is constrained
   * to be the element in column \a x and row \a y.
   *
   * Supports both bounds (\a icl = ICL_BND) and
   * domain consistency (\a icl = ICL_DOM, default).
   *
   * Throws an exception of type Int::OutOfLimits, if
   * the integers in \a n exceed the limits in Int::Limits.
   *
   * Throws an exception of type Int::ArgumentSizeMismatch, if
   * \f$ w\cdot h\neq|a|\f$.
   */
   void element(Space& home, const IntVarArgs& a, IntVar x, int w, IntVar y, int h, IntVar z, IntConLevel icl=ICL_DEF);
  /** \brief Post propagator for \f$ a_{x+w\cdot y}=z\f$
   *
   * If \a a is regarded as a two-dimensional array in row-major
   * order of width \a w and height \a h, then \a z is constrained
   * to be the element in column \a x and row \a y.
   *
   * Throws an exception of type Int::OutOfLimits, if
   * the integers in \a n exceed the limits in Int::Limits.
   *
   * Throws an exception of type Int::ArgumentSizeMismatch, if
   * \f$ w\cdot h\neq|a|\f$.
   */
  void element(Space& home, const BoolVarArgs& a, IntVar x, int w, IntVar y, int h, BoolVar z, IntConLevel icl=ICL_DEF);
  //@}




void distinct(Space& home, const IntVarArgs& x, IntConLevel icl=ICL_DEF);

void distinct(Space& home, const IntArgs& n, const IntVarArgs& x, IntConLevel icl=ICL_DEF);

/**
   * \defgroup TaskModelIntChannel Channel constraints
   * \ingroup TaskModelInt
   */

  //@{
  /** \brief Post propagator for \f$ x_i = j\leftrightarrow y_j=i\f$ for all \f$0\leq i<|x|\f$
   *
   * \li Supports domain consistency (\a icl = ICL_DOM) and value 
   *     propagation (all other values for \a icl).
   * \li Throws an exception of type Int::ArgumentSizeMismatch, if
   *     \a x and \a y are of different size.
   * \li Throws an exception of type Int::ArgumentSame, if \a x or
   *     \a y contain the same unassigned variable multiply. Note that a
   *     variable can occur in both \a x and \a y, but not more than
   *     once in either \a x or \a y.
   */
  void channel(Space& home, const IntVarArgs& x, const IntVarArgs& y, IntConLevel icl=ICL_DEF);

  /** \brief Post propagator for \f$ x_i - \mathit{xoff} = j\leftrightarrow y_j - \mathit{yoff} = i\f$ for all \f$0\leq i<|x|\f$
   *
   * \li Supports domain consistency (\a icl = ICL_DOM) and value 
   *     propagation (all other values for \a icl).
   * \li Throws an exception of type Int::ArgumentSizeMismatch, if
   *     \a x and \a y are of different size.
   * \li Throws an exception of type Int::ArgumentSame, if \a x or
   *     \a y contain the same unassigned variable multiply. Note that a
   *     variable can occur in both \a x and \a y, but not more than
   *     once in either \a x or \a y.
   * \li Throws an exception of type Int::OutOfLimits, if \a xoff or
   *     \a yoff are negative.
   */
   void channel(Space& home, const IntVarArgs& x, int xoff, const IntVarArgs& y, int yoff, IntConLevel icl=ICL_DEF);

  /// Post propagator for channeling a Boolean and an integer variable \f$ x_0 = x_1\f$
	void channel(Space& home, BoolVar x0, IntVar x1, IntConLevel icl=ICL_DEF);
  /// Post propagator for channeling an integer and a Boolean variable \f$ x_0 = x_1\f$
  /*forceinline*/ void channel(Space& home, IntVar x0, BoolVar x1, IntConLevel icl=ICL_DEF) 
  {
    channel(home,x1,x0,icl);
  }
  /** \brief Post propagator for channeling Boolean and integer variables \f$ x_i = 1\leftrightarrow y=i+o\f$
   *
   * Throws an exception of type Int::ArgumentSame, if \a x
   * contains the same unassigned variable multiply.
   */
  void channel(Space& home, const BoolVarArgs& x, IntVar y, int o=0, IntConLevel icl=ICL_DEF);
  //@}

/**
   * \defgroup TaskModelIntSorted Sorted constraints
   *
   * All sorted constraints support bounds consistency.
   *
   * \ingroup TaskModelInt
   */
  //@{
  /**
   * \brief Post propagator that \a y is \a x sorted in increasing order
   *
   * Might throw the following exceptions:
   *  - Int::ArgumentSizeMismatch, if \a x and \a y differ in size.
   *  - Int::ArgumentSame, if \a x or \a y contain
   *             shared unassigned variables.
   */
   void sorted(Space& home, const IntVarArgs& x, const IntVarArgs& y, IntConLevel icl=ICL_DEF);

  /**
   * \brief Post propagator that \a y is \a x sorted in increasing order
   *
   * The values in \a z describe the sorting permutation, that is
   * \f$\forall i\in\{0,\dots,|x|-1\}: x_i=y_{z_i} \f$.
   *
   * Might throw the following exceptions:
   *  - Int::ArgumentSizeMismatch, if \a x and \a y differ in size.
   *  - Int::ArgumentSame, if \a x or \a y contain
   *             shared unassigned variables.
   */
   void sorted(Space& home, const IntVarArgs& x, const IntVarArgs& y, const IntVarArgs& z, IntConLevel icl=ICL_DEF);
  //@}

	/**
   * \defgroup TaskModelIntCount Counting constraints
   * \ingroup TaskModelInt
   *
   *  \note
   *    Domain consistency on the extended cardinality variables of
   *    the Global Cardinality Propagator is only obtained if they are bounds
   *    consistent, otherwise the problem of enforcing domain consistency
   *    on the cardinality variables is NP-complete as proved by
   *    Qumiper et. al. in
   *    ''Improved Algorithms for the Global Cardinality Constraint''.
   */

  //@{
  /** \brief Post propagator for \f$\#\{i\in\{0,\ldots,|x|-1\}\;|\;x_i=n\}\sim_r m\f$
   *
   * Supports domain consistent propagation only.
   */
   void count(Space& home, const IntVarArgs& x, int n, IntRelType r, int m, IntConLevel icl=ICL_DEF);
  /** \brief Post propagator for \f$\#\{i\in\{0,\ldots,|x|-1\}\;|\;x_i=y\}\sim_r m\f$
   *
   * Supports domain consistent propagation only.
   */
   void count(Space& home, const IntVarArgs& x, IntVar y, IntRelType r, int m, IntConLevel icl=ICL_DEF);
  /** \brief Post propagator for \f$\#\{i\in\{0,\ldots,|x|-1\}\;|\;x_i=y_i\}\sim_r m\f$
   *
   * Supports domain consistent propagation only.
   *
   * Throws an exception of type Int::ArgumentSizeMismatch, if
   *  \a x and \a y are of different size.
   */
   void count(Space& home, const IntVarArgs& x, const IntArgs& y, IntRelType r, int m, IntConLevel icl=ICL_DEF);
  /** \brief Post propagator for \f$\#\{i\in\{0,\ldots,|x|-1\}\;|\;x_i=n\}\sim_r z\f$
   *
   * Supports domain consistent propagation only.
   */
   void count(Space& home, const IntVarArgs& x, int n, IntRelType r, IntVar z, IntConLevel icl=ICL_DEF);
  /** \brief Post propagator for \f$\#\{i\in\{0,\ldots,|x|-1\}\;|\;x_i=y\}\sim_r z\f$
   *
   * Supports domain consistent propagation only.
   */
  void count(Space& home, const IntVarArgs& x, IntVar y, IntRelType r, IntVar z, IntConLevel icl=ICL_DEF);
  /** \brief Post propagator for \f$\#\{i\in\{0,\ldots,|x|-1\}\;|\;x_i=y_i\}\sim_r z\f$
   *
   * Supports domain consistent propagation only.
   *
   * Throws an exception of type Int::ArgumentSizeMismatch, if
   *  \a x and \a y are of different size.
   */
   void count(Space& home, const IntVarArgs& x, const IntArgs& y, IntRelType r, IntVar z, IntConLevel icl=ICL_DEF);

  /** \brief Posts a global count (cardinality) constraint
    *
    * Posts the constraint that
    * \f$\#\{i\in\{0,\ldots,|x|-1\}\;|\;x_i=j\}=c_j\f$ and
    * \f$ \bigcup_i \{x_i\} \subseteq \{0,\ldots,|c|-1\}\f$
    * (no other value occurs).
    *
    * Supports value (\a icl = ICL_VAL, default), bounds (\a icl = ICL_BND),
    * and domain consistency (\a icl = ICL_DOM).
    *
    * Throws an exception of type Int::ArgumentSame, if \a x contains
    * the same unassigned variable multiply.
    */
   void count(Space& home, const IntVarArgs& x, const IntVarArgs& c, IntConLevel icl=ICL_DEF);

  /** \brief Posts a global count (cardinality) constraint
    *
    * Posts the constraint that
    * \f$\#\{i\in\{0,\ldots,|x|-1\}\;|\;x_i=j\}\in c_j\f$ and
    * \f$ \bigcup_i \{x_i\} \subseteq \{0,\ldots,|c|-1\}\f$
    * (no other value occurs).
    *
    * Supports value (\a icl = ICL_VAL, default), bounds (\a icl = ICL_BND),
    * and domain consistency (\a icl = ICL_DOM).
    *
    * Throws an exception of type Int::ArgumentSame, if \a x contains
    * the same unassigned variable multiply.
    */
   void count(Space& home, const IntVarArgs& x, const IntSetArgs& c, IntConLevel icl=ICL_DEF);

  /** \brief Posts a global count (cardinality) constraint
    *
    * Posts the constraint that
    * \f$\#\{i\in\{0,\ldots,|x|-1\}\;|\;x_i=v_j\}=c_j\f$ and
    * \f$ \bigcup_i \{x_i\} \subseteq \bigcup_j \{v_j\}\f$
    * (no other value occurs).
    *
    * Supports value (\a icl = ICL_VAL, default), bounds (\a icl = ICL_BND),
    * and domain consistency (\a icl = ICL_DOM).
    *
    * Throws an exception of type Int::ArgumentSame, if \a x contains
    * the same unassigned variable multiply.
    *
    * Throws an exception of type Int::ArgumentSizeMismatch, if
    *  \a c and \a v are of different size.
    */
   void count(Space& home, const IntVarArgs& x, const IntVarArgs& c, const IntArgs& v, IntConLevel icl=ICL_DEF);

  /** \brief Posts a global count (cardinality) constraint
    *
    * Posts the constraint that
    * \f$\#\{i\in\{0,\ldots,|x|-1\}\;|\;x_i=v_j\}\in c_j\f$ and
    * \f$ \bigcup_i \{x_i\} \subseteq \bigcup_j \{v_j\}\f$
    * (no other value occurs).
    *
    * Supports value (\a icl = ICL_VAL, default), bounds (\a icl = ICL_BND),
    * and domain consistency (\a icl = ICL_DOM).
    *
    * Throws an exception of type Int::ArgumentSame, if \a x contains
    * the same unassigned variable multiply.
    *
    * Throws an exception of type Int::ArgumentSizeMismatch, if
    *  \a c and \a v are of different size.
    */
   void count(Space& home, const IntVarArgs& x, const IntSetArgs& c, const IntArgs& v, IntConLevel icl=ICL_DEF);

  /** \brief Posts a global count (cardinality) constraint
    *
    * Posts the constraint that
    * \f$\#\{i\in\{0,\ldots,|x|-1\}\;|\;x_i=v_j\}\in c\f$ and
    * \f$ \bigcup_i \{x_i\} \subseteq \bigcup_j \{v_j\}\f$
    * (no other value occurs).
    *
    * Supports value (\a icl = ICL_VAL, default), bounds (\a icl = ICL_BND),
    * and domain consistency (\a icl = ICL_DOM).
    *
    * Throws an exception of type Int::ArgumentSame, if \a x contains
    * the same unassigned variable multiply.
    *
    * Throws an exception of type Int::ArgumentSizeMismatch, if
    *  \a c and \a v are of different size.
    */
   void count(Space& home, const IntVarArgs& x, const IntSet& c, const IntArgs& v, IntConLevel icl=ICL_DEF);

  //@}

/**
   * \defgroup TaskModelIntSequence Sequence constraints
   * \ingroup TaskModelInt
   */

  //@{
  /** \brief Post propagator for \f$\operatorname{sequence}(x,s,q,l,u)\f$
   *
   * Posts a domain consistent propagator for the constraint
   * \f$\bigwedge_{i=0}^{|x|-q} 
   *      \operatorname{among}(\langle x_i,\ldots,x_{i+q-1}\rangle,s,l,u)\f$
   * where the among constraint is defined as
   * \f$l\leq\#\{j\in\{i,\ldots,i+q-1\}\;|\;x_j\in s\} \leq u\f$.
   *
   * Throws the following exceptions:
   *  - Of type Int::TooFewArguments, if \f$|x|=0\f$.
   *  - Of type Int::ArgumentSame, if \a x contains
   *    the same unassigned variable multiply.
   *  - Of type Exception, if \f$q < 1 \vee q > |x|\f$.
   */
   void sequence(Space& home, const IntVarArgs& x, const IntSet& s, int q, int l, int u, IntConLevel icl=ICL_DEF);
  
  /** \brief Post propagator for \f$\operatorname{sequence}(x,s,q,l,u)\f$
   *
   * Posts a domain consistent propagator for the constraint
   * \f$\bigwedge_{i=0}^{|x|-q} 
   *      \operatorname{among}(\langle x_i,\ldots,x_{i+q-1}\rangle,s,l,u)\f$
   * where the among constraint is defined as
   * \f$l\leq\#\{j\in\{i,\ldots,i+q-1\}\;|\;x_j\in s\} \leq u\f$.
   *
   * Throws the following exceptions:
   *  - Of type Int::TooFewArguments, if \f$|x|=0\f$.
   *  - Of type Int::ArgumentSame, if \a x contains
   *    the same unassigned variable multiply.
   *  - Of type Exception, if \f$q < 1 \vee q > |x|\f$.
   */
	void sequence(Space& home, const BoolVarArgs& x, const IntSet& s, int q, int l, int u, IntConLevel icl=ICL_DEF);

  //@}
  
  
  //@{
  /** \brief Post propagator for \f$ \min\{x_0,x_1\}=x_2\f$
   *
   * Supports both bounds consistency (\a icl = ICL_BND, default)
   * and domain consistency (\a icl = ICL_DOM).
   */
   void min(Space& home, IntVar x0, IntVar x1, IntVar x2, IntConLevel icl=ICL_DEF);
   
  /** \brief Post propagator for \f$ \min x=y\f$
   *
   * Supports both bounds consistency (\a icl = ICL_BND, default)
   * and domain consistency (\a icl = ICL_DOM).
   *
   * If \a x is empty, an exception of type Int::TooFewArguments is thrown.
   */
   void min(Space& home, const IntVarArgs& x, IntVar y, IntConLevel icl=ICL_DEF);
  /** \brief Post propagator for \f$ \max\{x_0,x_1\}=x_2\f$
   *
   * Supports both bounds consistency (\a icl = ICL_BND, default)
   * and domain consistency (\a icl = ICL_DOM).
   */
   void max(Space& home, IntVar x0, IntVar x1, IntVar x2, IntConLevel icl=ICL_DEF);
  /** \brief Post propagator for \f$ \max x=y\f$
   *
   * Supports both bounds consistency (\a icl = ICL_BND, default)
   * and domain consistency (\a icl = ICL_DOM).
   *
   * If \a x is empty, an exception of type Int::TooFewArguments is thrown.
   */
   void max(Space& home, const IntVarArgs& x, IntVar y, IntConLevel icl=ICL_DEF);

  /** \brief Post propagator for \f$ |x_0|=x_1\f$
   *
   * Supports both bounds consistency (\a icl = ICL_BND, default)
   * and domain consistency (\a icl = ICL_DOM).
   */
   void abs(Space& home, IntVar x0, IntVar x1, IntConLevel icl=ICL_DEF);

  /** \brief Post propagator for \f$x_0\cdot x_1=x_2\f$
   *
   * Supports both bounds consistency (\a icl = ICL_BND, default)
   * and domain consistency (\a icl = ICL_DOM).
   */
   void mult(Space& home, IntVar x0, IntVar x1, IntVar x2, IntConLevel icl=ICL_DEF);

  /** \brief Post propagator for \f$x_0\cdot x_0=x_1\f$
   *
   * Supports both bounds consistency (\a icl = ICL_BND, default)
   * and domain consistency (\a icl = ICL_DOM).
   */
   void sqr(Space& home, IntVar x0, IntVar x1, IntConLevel icl=ICL_DEF);

  /** \brief Post propagatorSpace& for \f$\lfloor\sqrt{x_0}\rfloor=x_1\f$
   *
   * Supports both bounds consistency (\a icl = ICL_BND, default)
   * and domain consistency (\a icl = ICL_DOM).
   */
   void sqrt(Space& home, IntVar x0, IntVar x1, IntConLevel icl=ICL_DEF);

  /** \brief Post propagator for \f$x_0\ \mathrm{div}\ x_1=x_2 \land x_0\ \mathrm{mod}\ x_1 = x_3\f$
   *
   * Supports bounds consistency (\a icl = ICL_BND, default).
   */
   void divmod(Space& home, IntVar x0, IntVar x1, IntVar x2, IntVar x3, IntConLevel icl=ICL_DEF);

  /** \brief Post propagator for \f$x_0\ \mathrm{div}\ x_1=x_2\f$
   *
   * Supports bounds consistency (\a icl = ICL_BND, default).
   */
  void div(Space& home, IntVar x0, IntVar x1, IntVar x2, IntConLevel icl=ICL_DEF);

  /** \brief Post propagator for \f$x_0\ \mathrm{mod}\ x_1=x_2\f$
   *
   * Supports bounds consistency (\a icl = ICL_BND, default).
   */
   void mod(Space& home, IntVar x0, IntVar x1, IntVar x2, IntConLevel icl=ICL_DEF);
  //@}

/**
   * \defgroup TaskModelIntLI Linear constraints over integer variables
   * \ingroup TaskModelInt
   *
   * All variants for linear constraints over integer variables share
   * the following properties:
   *  - Bounds consistency (over the real numbers) is supported for
   *    all constraints (actually, for disequlities always domain consistency
   *    is used as it is cheaper). Domain consistency is supported for all
   *    non-reified constraint. As bounds consistency for inequalities
   *    coincides with domain consistency, the only
   *    real variation is for linear equations. Domain consistent
   *    linear equations have exponential complexity, so use with care!
   *  - Variables occurring multiply in the argument arrays are replaced
   *    by a single occurrence: for example, \f$ax+bx\f$ becomes
   *    \f$(a+b)x\f$.
   *  - If in the above simplification the value for \f$(a+b)\f$ (or for
   *    \f$a\f$ and \f$b\f$) exceeds the limits for integers as
   *    defined in Int::Limits, an exception of type
   *    Int::OutOfLimits is thrown.
   *  - Assume the constraint
   *    \f$\sum_{i=0}^{|x|-1}a_i\cdot x_i\sim_r c\f$.
   *    If  \f$|c|+\sum_{i=0}^{|x|-1}a_i\cdot x_i\f$ exceeds the maximal
   *    available precision (at least \f$2^{48}\f$), an exception of
   *    type Int::OutOfLimits is thrown.
   *  - In all other cases, the created propagators are accurate (that
   *    is, they will not silently overflow during propagation).
   */
  /** \brief Post propagator for \f$\sum_{i=0}^{|x|-1}x_i\sim_r c\f$
   * \ingroup TaskModelIntLI
   */
   void linear(Space& home, const IntVarArgs& x, IntRelType r, int c, IntConLevel icl=ICL_DEF);
  /** \brief Post propagator for \f$\sum_{i=0}^{|x|-1}x_i\sim_r y\f$
   * \ingroup TaskModelIntLI
   */
   void linear(Space& home, const IntVarArgs& x, IntRelType r, IntVar y, IntConLevel icl=ICL_DEF);
  /** \brief Post propagator for \f$\left(\sum_{i=0}^{|x|-1}x_i\sim_r c\right)\Leftrightarrow b\f$
   * \ingroup TaskModelIntLI
   */
   void linear(Space& home, const IntVarArgs& x, IntRelType r, int c, BoolVar b, IntConLevel icl=ICL_DEF);
  /** \brief Post propagator for \f$\left(\sum_{i=0}^{|x|-1}x_i\sim_r y\right)\Leftrightarrow b\f$
   * \ingroup TaskModelIntLI
   */
   void linear(Space& home, const IntVarArgs& x, IntRelType r, IntVar y, BoolVar b, IntConLevel icl=ICL_DEF);
  /** \brief Post propagator for \f$\sum_{i=0}^{|x|-1}a_i\cdot x_i\sim_r c\f$
   *
   *  Throws an exception of type Int::ArgumentSizeMismatch, if
   *  \a a and \a x are of different size.
   * \ingroup TaskModelIntLI
   */
  void linear(Space& home, const IntArgs& a, const IntVarArgs& x, IntRelType r, int c, IntConLevel icl=ICL_DEF);
  /** \brief Post propagator for \f$\sum_{i=0}^{|x|-1}a_i\cdot x_i\sim_r y\f$
   *
   *  Throws an exception of type Int::ArgumentSizeMismatch, if
   *  \a a and \a x are of different size.
   * \ingroup TaskModelIntLI
   */
   void linear(Space& home, const IntArgs& a, const IntVarArgs& x, IntRelType r, IntVar y, IntConLevel icl=ICL_DEF);
  /** \brief Post propagator for \f$\left(\sum_{i=0}^{|x|-1}a_i\cdot x_i\sim_r c\right)\Leftrightarrow b\f$
   *
   *  Throws an exception of type Int::ArgumentSizeMismatch, if
   *  \a a and \a x are of different size.
   * \ingroup TaskModelIntLI
   */
   void linear(Space& home, const IntArgs& a, const IntVarArgs& x, IntRelType r, int c, BoolVar b, IntConLevel icl=ICL_DEF);
  /** \brief Post propagator for \f$\left(\sum_{i=0}^{|x|-1}a_i\cdot x_i\sim_r y\right)\Leftrightarrow b\f$
   *
   *  Throws an exception of type Int::ArgumentSizeMismatch, if
   *  \a a and \a x are of different size.
   * \ingroup TaskModelIntLI
   */
   void linear(Space& home, const IntArgs& a, const IntVarArgs& x, IntRelType r, IntVar y, BoolVar b, IntConLevel icl=ICL_DEF);


  /**
   * \defgroup TaskModelIntLB Linear constraints over Boolean variables
   * \ingroup TaskModelInt
   *
   * All variants for linear constraints over Boolean variables share
   * the following properties:
   *  - Bounds consistency (over the real numbers) is supported for
   *    all constraints (actually, for disequlities always domain consistency
   *    is used as it is cheaper).
   *  - Variables occurring multiply in the argument arrays are replaced
   *    by a single occurrence: for example, \f$ax+bx\f$ becomes
   *    \f$(a+b)x\f$.
   *  - If in the above simplification the value for \f$(a+b)\f$ (or for
   *    \f$a\f$ and \f$b\f$) exceeds the limits for integers as
   *    defined in Int::Limits, an exception of type
   *    Int::OutOfLimits is thrown.
   *  - Assume the constraint
   *    \f$\sum_{i=0}^{|x|-1}a_i\cdot x_i\sim_r c\f$.
   *    If  \f$|c|+\sum_{i=0}^{|x|-1}a_i\cdot x_i\f$ exceeds the limits
   *    for integers as defined in Int::Limits, an exception of
   *    type Int::OutOfLimits is thrown.
   *  - In all other cases, the created propagators are accurate (that
   *    is, they will not silently overflow during propagation).
   */
  /** \brief Post propagator for \f$\sum_{i=0}^{|x|-1}x_i\sim_r c\f$
   * \ingroup TaskModelIntLB
   */
   void linear(Space& home, const BoolVarArgs& x, IntRelType r, int c, IntConLevel icl=ICL_DEF);
  /** \brief Post propagator for \f$\left(\sum_{i=0}^{|x|-1}x_i\sim_r c\right)\Leftrightarrow b\f$
   * \ingroup TaskModelIntLB
   */
   void linear(Space& home, const BoolVarArgs& x, IntRelType r, int c, BoolVar b, IntConLevel icl=ICL_DEF);
  /** \brief Post propagator for \f$\sum_{i=0}^{|x|-1}x_i\sim_r y\f$
   * \ingroup TaskModelIntLB
   */
  void linear(Space& home, const BoolVarArgs& x, IntRelType r, IntVar y, IntConLevel icl=ICL_DEF);
  /** \brief Post propagator for \f$\left(\sum_{i=0}^{|x|-1}x_i\sim_r y\right)\Leftrightarrow b\f$
   * \ingroup TaskModelIntLB
   */
   void linear(Space& home, const BoolVarArgs& x, IntRelType r, IntVar y, BoolVar b, IntConLevel icl=ICL_DEF);
  /** \brief Post propagator for \f$\sum_{i=0}^{|x|-1}a_i\cdot x_i\sim_r c\f$
   *
   *  Throws an exception of type Int::ArgumentSizeMismatch, if
   *  \a a and \a x are of different size.
   * \ingroup TaskModelIntLB
   */
  void linear(Space& home, const IntArgs& a, const BoolVarArgs& x, IntRelType r, int c, IntConLevel icl=ICL_DEF);
  /** \brief Post propagator for \f$\left(\sum_{i=0}^{|x|-1}a_i\cdot x_i\sim_r c\right)\Leftrightarrow b\f$
   *
   *  Throws an exception of type Int::ArgumentSizeMismatch, if
   *  \a a and \a x are of different size.
   * \ingroup TaskModelIntLB
   */
   void linear(Space& home, const IntArgs& a, const BoolVarArgs& x, IntRelType r, int c, BoolVar b, IntConLevel icl=ICL_DEF);
  /** \brief Post propagator for \f$\sum_{i=0}^{|x|-1}a_i\cdot x_i\sim_r y\f$
   *
   *  Throws an exception of type Int::ArgumentSizeMismatch, if
   *  \a a and \a x are of different size.
   * \ingroup TaskModelIntLB
   */
   void linear(Space& home, const IntArgs& a, const BoolVarArgs& x, IntRelType r, IntVar y, IntConLevel icl=ICL_DEF);
  /** \brief Post propagator for \f$\left(\sum_{i=0}^{|x|-1}a_i\cdot x_i\sim_r y\right)\Leftrightarrow b\f$
   *
   *  Throws an exception of type Int::ArgumentSizeMismatch, if
   *  \a a and \a x are of different size.
   * \ingroup TaskModelIntLB
   */
  void linear(Space& home, const IntArgs& a, const BoolVarArgs& x, IntRelType r, IntVar y, BoolVar b, IntConLevel icl=ICL_DEF);

/**
   * \defgroup TaskModelIntBranch Branching
   * \ingroup TaskModelInt
   */
  //@{
  /// Which variable to select for branching
  enum IntVarBranch {
    INT_VAR_NONE = 0,        ///< First unassigned
    INT_VAR_RND,             ///< Random (uniform, for tie breaking)
    INT_VAR_DEGREE_MIN,      ///< With smallest degree
    INT_VAR_DEGREE_MAX,      ///< With largest degree
    INT_VAR_AFC_MIN,         ///< With smallest accumulated failure count
    INT_VAR_AFC_MAX,         ///< With largest accumulated failure count
    INT_VAR_MIN_MIN,         ///< With smallest min
    INT_VAR_MIN_MAX,         ///< With largest min
    INT_VAR_MAX_MIN,         ///< With smallest max
    INT_VAR_MAX_MAX,         ///< With largest max
    INT_VAR_SIZE_MIN,        ///< With smallest domain size
    INT_VAR_SIZE_MAX,        ///< With largest domain size
    INT_VAR_SIZE_DEGREE_MIN, ///< With smallest domain size divided by degree
    INT_VAR_SIZE_DEGREE_MAX, ///< With largest domain size divided by degree
    INT_VAR_SIZE_AFC_MIN,    ///< With smallest domain size divided by accumulated failure count
    INT_VAR_SIZE_AFC_MAX,    ///< With largest domain size divided by accumulated failure count
    /** \brief With smallest min-regret
     *
     * The min-regret of a variable is the difference between the
     * smallest and second-smallest value still in the domain.
     */
    INT_VAR_REGRET_MIN_MIN,
    /** \brief With largest min-regret
     *
     * The min-regret of a variable is the difference between the
     * smallest and second-smallest value still in the domain.
     */
    INT_VAR_REGRET_MIN_MAX,
    /** \brief With smallest max-regret
     *
     * The max-regret of a variable is the difference between the
     * largest and second-largest value still in the domain.
     */
    INT_VAR_REGRET_MAX_MIN,
    /** \brief With largest max-regret
     *
     * The max-regret of a variable is the difference between the
     * largest and second-largest value still in the domain.
     */
    INT_VAR_REGRET_MAX_MAX
  };

  /// Which values to select first for branching
  enum IntValBranch {
    INT_VAL_MIN,       ///< Select smallest value
    INT_VAL_MED,       ///< Select greatest value not greater than the median
    INT_VAL_MAX,       ///< Select largest value
    INT_VAL_RND,       ///< Select random value
    INT_VAL_SPLIT_MIN, ///< Select values not greater than mean of smallest and largest value
    INT_VAL_SPLIT_MAX, ///< Select values greater than mean of smallest and largest value
    INT_VAL_RANGE_MIN, ///< Select the smallest range of the variable domain if it has sevral ranges, otherwise select values not greater than mean of smallest and largest value
    INT_VAL_RANGE_MAX, ///< Select the largest range of the variable domain if it has sevral ranges, otherwise select values greater than mean of smallest and largest value
    INT_VALUES_MIN,    ///< Try all values starting from smallest
    INT_VALUES_MAX     ///< Try all values starting from largest
  };

 /// Branch over \a x with variable selection \a vars and value selection \a vals
  void
  branch(Space& home, const IntVarArgs& x,
         IntVarBranch vars, IntValBranch vals,
         const VarBranchOptions& o_vars = VarBranchOptions::def,
         const ValBranchOptions& o_vals = ValBranchOptions::def);
         
  void
  branch(Space& home, IntVar x, IntValBranch vals,
         const ValBranchOptions& o_vals = ValBranchOptions::def);       

	/// Branch over \a x with variable selection \a vars and value selection \a vals
  void
  branch(Space& home, const BoolVarArgs& x,
         IntVarBranch vars, IntValBranch vals,
         const VarBranchOptions& o_vars = VarBranchOptions::def,
         const ValBranchOptions& o_vals = ValBranchOptions::def);
         
    /// Branch over \a x with value selection \a vals
	void
  branch(Space& home, BoolVar x, IntValBranch vals,
         const ValBranchOptions& o_vals = ValBranchOptions::def);     


	 /**
   * \defgroup TaskModelIntAssign Assigning
   * \ingroup TaskModelInt
   */
  //@{
  /// Which value to select for assignment
  enum IntAssign {
    INT_ASSIGN_MIN, ///< Select smallest value
    INT_ASSIGN_MED, ///< Select greatest element not greater than the median
    INT_ASSIGN_MAX, ///< Select maximum value
    INT_ASSIGN_RND  ///< Select random value
  };

  /// Assign all \a x with value selection \a vals
  void assign(Space& home, const IntVarArgs& x, IntAssign vals, const ValBranchOptions& o_vals = ValBranchOptions::def);
  /// Assign \a x with value selection \a vals
   void assign(Space& home, IntVar x, IntAssign vals, const ValBranchOptions& o_vals = ValBranchOptions::def);
  /// Assign all \a x with value selection \a vals
   void assign(Space& home, const BoolVarArgs& x, IntAssign vals, const ValBranchOptions& o_vals = ValBranchOptions::def);
  /// Assign \a x with value selection \a vals
   void assign(Space& home, BoolVar x, IntAssign vals, const ValBranchOptions& o_vals = ValBranchOptions::def);

  //@}



/************************************************set.hh*************************************************************/


/**
   * \brief %Set variables
   *
   * \ingroup TaskModelSetVars
   */
  class SetVar  {
  public:

    /// \name Value access
    //@{
    /// Return number of elements in the greatest lower bound
    unsigned int glbSize(void) const;
    /// Return number of elements in the least upper bound
    unsigned int lubSize(void) const;
    /// Return number of unknown elements (elements in lub but not in glb)
    unsigned int unknownSize(void) const;
    /// Return cardinality minimum
    unsigned int cardMin(void) const;
    /// Return cardinality maximum
    unsigned int cardMax(void) const;
    /// Return minimum element of least upper bound
    int lubMin(void) const;
    /// Return maximum element of least upper bound
    int lubMax(void) const;
    /// Return minimum element of greatest lower bound
    int glbMin(void) const;
    /// Return maximum of greatest lower bound
    int glbMax(void) const;
    //@}

    /// \name Domain tests
    //@{
    /// Test whether \a i is in greatest lower bound
    bool contains(int i) const;
    /// Test whether \a i is not in the least upper bound
    bool notContains(int i) const;
    //@}
  };





/**
   * \brief Common relation types for sets
   * \ingroup TaskModelSet
   */
  enum SetRelType {
    SRT_EQ,   ///< Equality (\f$=\f$)
    SRT_NQ,   ///< Disequality (\f$\neq\f$)
    SRT_SUB,  ///< Subset (\f$\subseteq\f$)
    SRT_SUP,  ///< Superset (\f$\supseteq\f$)
    SRT_DISJ, ///< Disjoint (\f$\parallel\f$)
    SRT_CMPL  ///< Complement
  };

  /**
   * \brief Common operations for sets
   * \ingroup TaskModelSet
   */
  enum SetOpType {
    SOT_UNION,  ///< Union
    SOT_DUNION, ///< Disjoint union
    SOT_INTER,  ///< %Intersection
    SOT_MINUS   ///< Difference
  };


//@{

  /// Propagates \f$ x \sim_r \{i\}\f$
   void
  dom(Space& home, SetVar x, SetRelType r, int i);

  /// Propagates \f$ x \sim_r \{i,\dots,j\}\f$
   void
  dom(Space& home, SetVar x, SetRelType r, int i, int j);

  /// Propagates \f$ x \sim_r s\f$
   void
  dom(Space& home, SetVar x, SetRelType r, const IntSet& s);

  /// Post propagator for \f$ (x \sim_r \{i\}) \Leftrightarrow b \f$
   void
  dom(Space& home, SetVar x, SetRelType r, int i, BoolVar b);

  /// Post propagator for \f$ (x \sim_r \{i,\dots,j\}) \Leftrightarrow b \f$
   void
  dom(Space& home, SetVar x, SetRelType r, int i, int j, BoolVar b);

  /// Post propagator for \f$ (x \sim_r s) \Leftrightarrow b \f$
   void
  dom(Space& home, SetVar x, SetRelType r, const IntSet& s, BoolVar b);

  /// Propagates \f$ i \leq |s| \leq j \f$
   void
  cardinality(Space& home, SetVar x, unsigned int i, unsigned int j);

  //@}


//@{

  /// Post propagator for \f$ x \sim_r y\f$
	void
  rel(Space& home, SetVar x, SetRelType r, SetVar y);

  /// Post propagator for \f$ (x \sim_r y) \Leftrightarrow b \f$
   void
  rel(Space& home, SetVar x, SetRelType r, SetVar y, BoolVar b);

  /// Post propagator for \f$ s \sim_r \{x\}\f$
   void
  rel(Space& home, SetVar s, SetRelType r, IntVar x);

  /// Post propagator for \f$ \{x\} \sim_r s\f$
   void
  rel(Space& home, IntVar x, SetRelType r, SetVar s);

  /// Post propagator for \f$ (s \sim_r \{x\}) \Leftrightarrow b \f$
   void
  rel(Space& home, SetVar s, SetRelType r, IntVar x, BoolVar b);

  /// Post propagator for \f$ (\{x\} \sim_r s) \Leftrightarrow b \f$
   void
  rel(Space& home, IntVar x, SetRelType r, SetVar s, BoolVar b);

  /// Post propagator for \f$|s|\geq 1 \land \forall i\in s:\ i \sim_r x\f$
   void
  rel(Space& home, SetVar s, IntRelType r, IntVar x);

  /// Post propagator for \f$|s|\geq 1 \land \forall i\in s:\ x \sim_r i\f$
   void
  rel(Space& home, IntVar x, IntRelType r, SetVar s);

  //@}
  
  
  //@{

  /// Post propagator for \f$ (x \diamond_{\mathit{op}} y) \sim_r z \f$
   void
  rel(Space& home, SetVar x, SetOpType op, SetVar y, SetRelType r, SetVar z);

  /// Post propagator for \f$ y = \diamond_{\mathit{op}} x\f$
   void
  rel(Space& home, SetOpType op, const SetVarArgs& x, SetVar y);

  /// Post propagator for \f$ y = \diamond_{\mathit{op}} x \diamond_{\mathit{op}} z\f$
   void
  rel(Space& home, SetOpType op, const SetVarArgs& x, const IntSet& z, SetVar y);

  /// Post propagator for \f$ y = \diamond_{\mathit{op}} x \diamond_{\mathit{op}} z\f$
   void
  rel(Space& home, SetOpType op, const IntVarArgs& x, const IntSet& z, SetVar y);

  /// Post propagator for \f$ y = \diamond_{\mathit{op}} x\f$
   void
  rel(Space& home, SetOpType op, const IntVarArgs& x, SetVar y);

  /// Post propagator for \f$ (x \diamond_{\mathit{op}} y) \sim_r z \f$
   void
  rel(Space& home, const IntSet& x, SetOpType op, SetVar y,
      SetRelType r, SetVar z);

  /// Post propagator for \f$ (x \diamond_{\mathit{op}} y) \sim_r z \f$
   void
  rel(Space& home, SetVar x, SetOpType op, const IntSet& y,
      SetRelType r, SetVar z);

  /// Post propagator for \f$ (x \diamond_{\mathit{op}} y) \sim_r z \f$
   void
  rel(Space& home, SetVar x, SetOpType op, SetVar y,
      SetRelType r, const IntSet& z);

  /// Post propagator for \f$ (x \diamond_{\mathit{op}} y) \sim_r z \f$
   void
  rel(Space& home, const IntSet& x, SetOpType op, SetVar y, SetRelType r,
      const IntSet& z);

  /// Post propagator for \f$ (x \diamond_{\mathit{op}} y) \sim_r z \f$
   void
  rel(Space& home, SetVar x, SetOpType op, const IntSet& y, SetRelType r,
      const IntSet& z);

  //@}

  void
  convex(Space& home, SetVar x);
  
  void
  convex(Space& home, SetVar x, SetVar y);

  /**
   * \defgroup TaskModelSetSequence Sequence constraints
   * \ingroup TaskModelSet
   *
   */
  //@{

  /// Post propagator for \f$\forall 0\leq i< |x|-1 : \max(x_i)<\min(x_{i+1})\f$
  void
  sequence(Space& home, const SetVarArgs& x);

  /// Post propagator for \f$\forall 0\leq i< |x|-1 : \max(x_i)<\min(x_{i+1})\f$ and \f$ x = \bigcup_{i\in\{0,\dots,n-1\}} y_i \f$
  void
  sequence(Space& home, const SetVarArgs& y, SetVar x);

  //@}

  //@{


  /// Post propagator for \f$\forall 0\leq i\leq |x| : |x_i|=c\f$ and \f$\forall 0\leq i<j\leq |x| : |x_i\cap x_j|\leq 1\f$
  void
  atmostOne(Space& home, const SetVarArgs& x, unsigned int c);

  //@}
  
  
   //@{

  /** \brief Post propagator that propagates that \a x is the
   *  minimal element of \a s, and that \a s is not empty */
  void
  min(Space& home, SetVar s, IntVar x);

  /** \brief Post propagator that propagates that \a x is not the
   *  minimal element of \a s */
   void
  notMin(Space& home, SetVar s, IntVar x);

  /** \brief Post reified propagator for \a b iff \a x is the
   *  minimal element of \a s */
   void
  min(Space& home, SetVar s, IntVar x, BoolVar b);

  /** \brief Post propagator that propagates that \a x is the
   *  maximal element of \a s, and that \a s is not empty */
   void
  max(Space& home, SetVar s, IntVar x);

  /** \brief Post propagator that propagates that \a x is not the
   *  maximal element of \a s */
   void
  notMax(Space& home, SetVar s, IntVar x);

  /** \brief Post reified propagator for \a b iff \a x is the
   *  maximal element of \a s */
   void
  max(Space& home, SetVar s, IntVar x, BoolVar b);

  /// Post propagator for \f$\{x_0,\dots,x_{n-1}\}=y\f$ and \f$x_i<x_{i+1}\f$
   void
  channel(Space& home, const IntVarArgs& x, SetVar y);

  /// Post propagator for \f$x_i=j \Leftrightarrow i\in y_j\f$
   void
  channel(Space& home, const IntVarArgs& x,const SetVarArgs& y);

  /// Post propagator for \f$x_i=1 \Leftrightarrow i\in y\f$
   void
  channel(Space& home, const BoolVarArgs& x, SetVar y);

  /// Post propagator for \f$ |s|=x \f$
   void
  cardinality(Space& home, SetVar s, IntVar x);
  
  
   /**
   * \brief Post propagator for \f$y = \mathrm{weight}(x)\f$
   *
   * The weights are given as pairs of elements and their weight:
   * \f$\mathrm{weight}(\mathrm{elements}_i) = \mathrm{weights}_i\f$
   *
   * The upper bound of \a x is constrained to contain only elements from
   * \a elements. The weight of a set is the sum of the weights of its
   * elements.
   */
  void
  weights(Space& home, const IntArgs& elements, const IntArgs& weights,
          SetVar x, IntVar y);

  //@}


  //@{

  /**
   * \brief Post propagator for \f$ z=\diamond_{\mathit{op}}\langle x_0,\dots,x_{n-1}\rangle[y] \f$
   *
   * If \a y is the empty set, the usual conventions for set operations apply:
   * an empty union is empty, while an empty intersection is the universe,
   * which can be given as the optional parameter \a u.
   *
   * The indices for \a y start at 0.
   */
   void
  element(Space& home, SetOpType op, const SetVarArgs& x, SetVar y, SetVar z,
    const IntSet& u = IntSet(Set::Limits::min,Set::Limits::max));

  /**
   * \brief Post propagator for \f$ z=\diamond_{\mathit{op}}\langle x_0,\dots,x_{n-1}\rangle[y] \f$
   *
   * If \a y is the empty set, the usual conventions for set operations apply:
   * an empty union is empty, while an empty intersection is the universe,
   * which can be given as the optional parameter \a u.
   *
   * The indices for \a y start at 0.
   */
   void
  element(Space& home, SetOpType op, const IntSetArgs& x, SetVar y, SetVar z,
    const IntSet& u = IntSet(Set::Limits::min,Set::Limits::max));

  /**
   * \brief Post propagator for \f$ z=\langle x_0,\dots,x_{n-1}\rangle[y] \f$
   *
   * The indices for \a y start at 0.
   */
   void
  element(Space& home, const SetVarArgs& x, IntVar y, SetVar z);

  /**
   * \brief Post propagator for \f$ z=\langle s_0,\dots,s_{n-1}\rangle[y] \f$
   *
   * The indices for \a y start at 0.
   */
   void
  element(Space& home, const IntSetArgs& s, IntVar y, SetVar z);

  /** \brief Post propagator for \f$ a_{x+w\cdot y}=z\f$
   *
   * Throws an exception of type Set::ArgumentSizeMismatch, if
   * \f$ w\cdot h\neq|a|\f$.
   */
   void
  element(Space& home, const IntSetArgs& a, 
          IntVar x, int w, IntVar y, int h, SetVar z);
  /** \brief Post propagator for \f$ a_{x+w\cdot y}=z\f$
   *
   * Throws an exception of type Set::ArgumentSizeMismatch, if
   * \f$ w\cdot h\neq|a|\f$.
   */
   void
  element(Space& home, const SetVarArgs& a, 
          IntVar x, int w, IntVar y, int h, SetVar z);
  //@}

  //@{
  /// Which variable to select for branching
  enum SetVarBranch {
    SET_VAR_NONE = 0,   ///< First unassigned
    SET_VAR_RND,        ///< Random (uniform, for tie breaking)
    SET_VAR_DEGREE_MIN, ///< With smallest degree
    SET_VAR_DEGREE_MAX, ///< With largest degree
    SET_VAR_AFC_MIN,    ///< With smallest accumulated failure count
    SET_VAR_AFC_MAX,    ///< With largest accumulated failure count
    SET_VAR_MIN_MIN,    ///< With smallest minimum unknown element
    SET_VAR_MIN_MAX,    ///< With largest minimum unknown element
    SET_VAR_MAX_MIN,    ///< With smallest maximum unknown element
    SET_VAR_MAX_MAX,    ///< With largest maximum unknown element
    SET_VAR_SIZE_MIN,   ///< With smallest unknown set
    SET_VAR_SIZE_MAX,   ///< With largest unknown set
    SET_VAR_SIZE_DEGREE_MIN, ///< With smallest domain size divided by degree
    SET_VAR_SIZE_DEGREE_MAX, ///< With largest domain size divided by degree
    SET_VAR_SIZE_AFC_MIN, ///< With smallest domain size divided by accumulated failure count
    SET_VAR_SIZE_AFC_MAX  ///< With largest domain size divided by accumulated failure count
  };

  /// Which values to select first for branching
  enum SetValBranch {
    SET_VAL_MIN_INC, ///< Include smallest element
    SET_VAL_MIN_EXC, ///< Exclude smallest element
    SET_VAL_MED_INC, ///< Include median element (rounding downwards)
    SET_VAL_MED_EXC, ///< Exclude median element (rounding downwards)
    SET_VAL_MAX_INC, ///< Include largest element
    SET_VAL_MAX_EXC, ///< Exclude largest element
    SET_VAL_RND_INC, ///< Include random element
    SET_VAL_RND_EXC  ///< Exclude random element
  };

  /// Branch over \a x with variable selection \a vars and value selection \a vals
   void
  branch(Space& home, const SetVarArgs& x,
         SetVarBranch vars, SetValBranch vals,
         const VarBranchOptions& o_vars = VarBranchOptions::def,
         const ValBranchOptions& o_vals = ValBranchOptions::def);
 
   /// Branch over \a x with value selection \a vals
   void
  branch(Space& home, SetVar x, SetValBranch vals,
         const ValBranchOptions& o_vals = ValBranchOptions::def);
  //@}

  /**
   * \defgroup TaskModelSetAssign Assigning
   * \ingroup TaskModelSet
   */
  //@{
  /// Which value to select for assignment
  enum SetAssign {
    SET_ASSIGN_MIN_INC, ///< Include smallest element
    SET_ASSIGN_MIN_EXC, ///< Exclude smallest element
    SET_ASSIGN_MED_INC, ///< Include median element (rounding downwards)
    SET_ASSIGN_MED_EXC, ///< Exclude median element (rounding downwards)
    SET_ASSIGN_MAX_INC, ///< Include largest element
    SET_ASSIGN_MAX_EXC, ///< Exclude largest element
    SET_ASSIGN_RND_INC, ///< Include random element
    SET_ASSIGN_RND_EXC  ///< Exclude random element
  };

  /// Assign all \a x with value selection \a vals
   void
  assign(Space& home, const SetVarArgs& x, SetAssign vals,
         const ValBranchOptions& o_vals = ValBranchOptions::def);
  /// Assign \a x with value selection \a vals
   void
  assign(Space& home, SetVar x, SetAssign vals,
         const ValBranchOptions& o_vals = ValBranchOptions::def);

  //@}



/****************************************************************************************************************/
/*																												*/
/*																												*/
/*											BINARY RELATIONS PART											  	*/
/*																												*/
/*																												*/
/****************************************************************************************************************/

/******************************************** graph.hh ********************************************************/

 // class GraphVar : public VarImpVar<Graph::GraphVarImp> {
 //    //friend std::ostream& operator <<(std::ostream& os, const GraphVar& x);
 //  private:
 //    using VarImpVar<Graph::GraphVarImp>::x;
 //  public:
 //    /// \name Constructors and initialization
 //    //@{
 //    /// Default constructor
 //    //GraphVar(void);
 //    /// Initialize from graph variable \a x
 //    //GraphVar(const GraphVar& y);
 //    /// Initialize from graph variable \a x
 //    //GraphVar(const Graph::GraphView& y);
 //    /**
 //     * \brief Initialize with an empty lower bound and a full upper
 //     * bound of order \a n.
 //     *
 //     * The resulting upper bound will be \f$A\times A \f$, with \f$ A
 //     * = \{0,\ldots,n\}\f$.
 //     */
 //    /*GECODE_GRAPH_EXPORT
 //    GraphVar(Space& home, int n);*/
 //    /**
 //     * \brief Initialize with the edges in \a glb as lower bound and
 //     * the edges in \a lub as upper bound.
 //     *
 //     * The order of the resulting variable is set to the maximum node
 //     * present in both arguments.
 //     *
 //     * \todo Should throw an exception if lb is not contained in ub.
 //     */
 //    /*GECODE_GRAPH_EXPORT 
 //    GraphVar(Space& home, const EdgesDesc& glb, const EdgesDesc& lub);*/
 //    /**
 //     * \brief Initialize with the edges in \a glb as lower bound and the edges in \a lub as upper 
 //     * bound.
 //     *
 //     * \warn Throws an exception if both sets do not have the same max node.
 //     * \warn Throws an exception if \a glb is not a subset of \a lub.
 //     */
 //    /*GECODE_GRAPH_EXPORT 
 //    GraphVar(Space& home, const Graph::EdgeSet& glb, const Graph::EdgeSet& lub);*/
 //    //@}
 //    /// \name Value access
 //    //@{
 //    /// Return number of edges in the lower bound
 //    unsigned int glbSize(void) const;
 //    /// Return number of edges in the upper bound
 //    unsigned int lubSize(void) const;
 //    /// Return nodes in the lower bound
 //    IntSet glbSet(void) const;
 //    /// Return nodes in the upper bound
 //    IntSet lubSet(void) const;
 //    //@}
 //    /// \name Cloning
 //    //@{
 //    /// Update this variable to be a clone of variable \a x
 //    //void update(Space& home, bool, GraphVar& x);
 //    //@}
 //  };

/******************************************** edges.hh ********************************************************/

// class Edge {
//     //friend std::ostream& operator<<(std::ostream& os, const Edge& s);
//   public:
//     /// Start node
//     int start;
//     /// End node
//     int end;
//     /// Default constructor
//     //Edge(void);
//     /// Creates the edge (\a s, \a e)
//     Edge(int s, int e);
//     /// Creates an edge from a pair of integers \a e
//     //Edge(std::pair<int,int> e);
//     /// Copy constructor
//     //Edge(const Edge& e);
//     /// Tests whether this is strictly less than \a e
//     bool operator<(const Edge& e) const;
//     /// Tests whether this is equal to \a e
//     bool operator==(const Edge& e) const;
//     /// Tests whether this is strictly greater than \a e
//     bool operator>(const Edge& e) const;
//     /// Tests whether this is different from \a e
//     bool operator!=(const Edge& e) const;
//     /// Tests whether this is less or equal than \a e
//     bool operator<=(const Edge& e) const;
//     /// Tests whether this is greater or equal than \a e
//     bool operator>=(const Edge& e) const;
//   };

/******************************************** edges-desc.hpp ********************************************************/

 // class EdgesDesc : public SharedHandle {
 //    //friend class EdgesDescIter;
 //  /*private:
 //    class EdgesDescObject : public SharedHandle::Object {
 //    public:
 //      /// Number of elements in the description
 //      int n;
 //      /// maximum node
 //      int mx;
 //      /// Array of edges in the descriptino
 //      Edge* d;
 //      /// Allocate object with \a m elements
 //      static EdgesDescObject* allocate(int m);
 //      /// Return a copy of the object
 //      SharedHandle::Object* copy(void) const;
 //      /// Delete the object
 //      virtual ~EdgesDescObject(void);
 //    };
 //    /// Initialize with \a n edges from array r.
 //    void init(const Edge r[], int n);*/
 //  public:
 //    /// \name Constructors and initialization
 //    //@{
 //    /// Initialize as an empty description
 //    EdgesDesc(void);
 //    /// Initialize with edge description d
 //    //EdgesDesc(const EdgesDesc& d);
 //    /// Initialize with \a n edges from array \a r
 // 	EdgesDesc(const Edge r[], int n);
 //    /// Initialize with an EdgeIterator
 //    //template <class I>
 //    //explicit EdgesDesc(I& i);
 //    //@}
 //    /// \name Edges description access
 //    //@{
 //    /// Return the cardinality of the description set
 //    unsigned int size(void) const;
 //    /**
 //     * \brief Return the maximum node of the represented set
 //     *
 //     * \warn When this operation is performed on the empty description or on an
 //     * uninitialized object it returns -1.
 //     */
 //    int maxNode(void) const;
 //    //@}
 //    /// \name Predefined value
 //    //@{
 //    /// Empty set
 //    static const EdgesDesc empty;
 //    /// \todo Provide a way to specify a full bound by only giving the max node.
 //    //@}    
 //  };
  
/******************************************** edgeset.hh ********************************************************/  
  
 /* TODO */

/**************************************************** and.hpp ****************************/

  /**
   * \brief Propagates \f$ land(X,Y,Z) \equiv \forall e : (e \in X
   * \land e \in Y) \iff e \in Z\f$
   *
   * Requires \code #include <graph/and.hh> \endcode   
   * \ingroup CpRelConsLog
   */    
  /*forceinline*/ //void
  // land(Space& home, GraphVar f, GraphVar g, GraphVar h) {
  //   using namespace Graph;
  //   GraphView fv(f);
  //   GraphView gv(g);
  //   GraphView hv(h);
  //   GECODE_ES_FAIL((And<GraphView,GraphView,GraphView>::post(home,fv,gv,hv)));
  // }
  
  /**
   * \brief Propagates \f$ lor(X,Y,Z) \equiv \forall e : (e \in X \lor
   * e \in Y) \iff e \in Z\f$
   *
   * \warn \f$ lor(X,Y,Z) \equiv
   * land(\mbox{\emph{Compl}}(X),\mbox{\emph{Compl}}(Y),\mbox{\emph{Compl}}(Z))
   * \f$
   *   
   * Requires \code #include <graph/and.hh> \endcode   
   * \ingroup CpRelConsLog
   */      
		  /*forceinline*/ //void
  // lor(Space& home, GraphVar f, GraphVar g, GraphVar h) {
  //   using namespace Graph;
  //   typedef ComplementView<GraphView> CV;
  //   GraphView fv(f);
  //   GraphView gv(g);
  //   GraphView hv(h);
  //   CV cfv(fv);
  //   CV cgv(gv);
  //   CV chv(hv);
  //   GECODE_ES_FAIL((And<CV,CV,CV>::post(home,cfv,cgv,chv)));
  // }
  /*int testblabla(void)
  {
  	return 27;
  }*/

/**************************************************** follow.hpp ****************************/


/**
   * \brief Propagates \f$ transClosure(X) \equiv \forall x,y,z : ((x,y) \in X \land (y,z) \in X) \iff (x,z) \in X\f$
   *
   * Requires \code #include <graph/follow.hh> \endcode   
   * \ingroup CpRelConsGraph
   */      
/*forceinline*/ //void
  // transClousure(Space& home, GraphVar f) {
  //   using namespace Graph;
  //   GraphView fv(f);
  //   GraphView fvv(f);
  //   GraphView fvvv(f);
  //   GECODE_ES_FAIL((Follow<GraphView,GraphView,GraphView>::post(home,fv,fvv,fvvv)));
  // }
  
 
/**************************************************** equiv.hpp ****************************/ 
 
  
 /**
   * \brief Propagates \f$ equiv(X,Y,Z) \equiv \forall x,y : ((x,y) \in X \iff (x,y) \in Y) \iff (x,y) \in Z \f$ 
   *
   * Requires \code #include <graph/eq.hh> \endcode
   * \ingroup CpRelConsLog
   */
  /*forceinline void
  equiv(Space& home, GraphVar f, GraphVar g, GraphVar h) {
    using namespace Graph;
    GraphView fv(f);
    GraphView gv(g);
    GraphView hv(h);
    GECODE_ES_FAIL((Equiv<GraphView,GraphView,GraphView>::post(home,fv,gv,hv,"equiv")));
  } */  
  
#endif
