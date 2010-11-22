#ifndef __CDD_HH__
#define __CDD__HH__

#include <iostream>
#include <exception>
#include "cuddCDD.h" 

// 32 bits integers max 8ary
#define BBV 5
#define BA 3

//#define BBV 2
//#define BA 1


namespace Cdd {
  class failedMerge : public std::exception {
		virtual const char* what(void) const throw() {
			return "Failed operation on CDD";
		}
	};
  class malformedDomain : public std::exception {
		virtual const char* what(void) const throw() {
			return "Malformed bounds in definition";
		}    
  };
  /**
   * \brief BDD abstraction
   */
  class BDD {
	private:
		/// Function pointer
		DdNode *node;
  public:
    /// Constructor
    BDD(void);
		/// Constructor from an existing node
		BDD(DdNode *n);
		/// Copy constructor
		BDD(const BDD& from);
		/// Returns the variable at possition \a v with a value of \a b
		BDD(int v, bool b);
    /// Destructor
    ~BDD(void);
		/// \name Test operations
		//@{
		/// BDD equality
		bool operator == (const BDD& b) const;
		/// BDD diesquality
		bool operator != (const BDD& b) const;
		//@}
		/// \name Operations
		//@{
		/// BDD negation
		BDD operator ! (void) const;
		/// BDD negation
		BDD operator ~ (void) const;
		/// Assignment operation
		BDD operator = (const BDD& right);
		/// BDD intersection
		BDD operator &= (const BDD& other);
		/// BDD union
		BDD operator |= (const BDD& other);
		/// CDD merge
		BDD operator << (const BDD& other) const;
		/// CDD intersection
		BDD operator && (const BDD& other) const;
		/// CDD union
		BDD operator || (const BDD& other) const;
    /// Status operation
    BDD status(const BDD& r) const;
		//@}
		/// \name BDD information and statistics
		//@{
		/// Return the number of minterms of this BDD with a maximum number of variables of \a nvars
		double minterms(int nvars) const;
		/// Return the number of nodes in the BDD
		int numNodes(void) const;
		/// Print the tuples represented in the BDD for a relation with arity a
		void printTuples(int a) const;
		//@}
		/// \name BDD output
		//{@
		/// Output the BDD in dot format to \a fname
		void dot(const char* fname) const;
		//@}
  };
	
	class Cudd {
		friend class BDD;
	private:
		static DdManager *dd;
	public:
		/// Constructor
		Cudd(void);
		/// Terminal one
		static BDD one(void);
		/// Terminal unknown
		static BDD unk(void);
		/// Terminal zero
		static BDD zero(void);
#ifndef NDEBUG
		/// Return the number of referenced nodes in the manager
		static int references(void);
#endif
	};
	
	class Relation {
  private:
    /// Representation
		BDD r;
		/// Arity
    int arity;
	public:
		/// Returns a BDD with a path representing \a v
		BDD path(int p, int a) const;
	public:
		/// Constructor for an empty relation of arity \a a
		Relation(int a);
		/// Initialization of a constraint domain with \a lb and \a ub as the bounds
		void init(const BDD& lb, const BDD& ub);
		/// Destructor
		~Relation(void);
		/// Return a BDD representing the tuple \a t. \a t has to have \a arity elements
		BDD repr(int *t) const;
		/// Cardinality (number of tuples represented by the BDD \a b for a relation of this cardinality)
		double cardinality(const BDD& b) const;
    /// Return the lower bound of the relation (do not us this in propagation)
    const BDD glb(void) const;
    /// Return the upper bound of the relation (do not us this in propagation)
    const BDD oob(void) const;
    /// Return the unknow of the relation (do not us this in propagation)
    const BDD unk(void) const;
	};
}

#endif
