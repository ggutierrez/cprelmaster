#ifndef __CPREL_BDDDOMAIN_ENCODING_HH__
#define __CPREL_BDDDOMAIN_ENCODING_HH__

#include <vector>
#include <obj/cuddObj.hh>
#include <bdddomain/perm-descriptor.hh>

/*
  Note: This file contain the operations that we need in order to use
  a BDD as a domain representation. All of the functions defined here
  assume a minimum set of operations provided by the bdd package
  itself but also tries to keep this assumptions at the minumum to
  ease the integration of other bdd libraries.

  All of the functions implemented here are aware of the notion of
  "column". A column is represented by a set of BDD variables and is
  pretty much the equivalent of a domain in the terminology of some
  BDD libraries.
 */

namespace MPG { namespace VarImpl {
    /**
     * \defgroup BDDEnc Ground relation encoding
     *
     *
     */
    /**
     * \brief Returns the variables (indices) used in the bdd to encode column \a c
     * \ingroup BDDEnc
     */
    std::vector<int> domainIndices(int c);
    /**
     * \brief Creates a cube with all the bdd variables that represent
     * column \a c 
     *
     * \ingroup BDDEnc
     */
    BDD makeCube(int c);
    /**
     * \brief Creates a cube with all the bdd variables that represent
     * columns in \a c
     *
     *\ingroup BDDEnc
     */
    BDD makeCube(const std::vector<int>& c);
    /**
     * \brief Returns a bdd containing the permutation of columns
     * according with the permutation specification \a pairing.
     *
     * \a pairing contains a pair of column indices per element. The
     * first element of the pair is the is the index of the column
     * that will be replaced by the column on the second element. This
     * operation will replace the contents of the column.
     */
    BDD replace(const std::vector<std::pair<int,int>>& pairing, const BDD& r);
    /**
     * \brief Returns a bdd with the swapping of the columns in \a r
     * represented by \a pairing
     */
    BDD swap(const std::vector<std::pair<int,int>>& pairing, const BDD& r);
    /**
     * \brief Unique quantification of \a r with respect to \a c
     *
     * \a c is assumed to be a cube
     */
    BDD unique(BDD r, BDD c);
    /**
     * \brief Unique quantification of column \a c on relation \a r.
     * \ingroup BDDEnc
     */
    BDD unique(int c, BDD r);
    /**
     * \brief Unique quantification on the columns in \a c on relation \a r.
     * \ingroup BDDEnc
     */
    BDD unique(const std::vector<int>& c, BDD r);
    /**
     * \brief Existensial quantification of column \a c on relation \a r.
     * \ingroup BDDEnc
     */
    BDD exists(int c, BDD r);
    /**
     * \brief Existential queantification of all the columns with
     * indices in \a c of relation \a r.
     */
    BDD exists(const std::vector<int>& c, BDD r);
    /**
     * \brief Universal quantification of column \a c on relation \a r.
     * \ingroup BDDEnc
     */
    BDD forall(int c, BDD r);
    /**
     * \brief Returns one minterm of \a r for that represents a tuple with \a c columns.
     * \ingroup BDDEnc
     */
    BDD oneTuple(int c, BDD r);
  }
}
#endif
