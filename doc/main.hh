
/**
 * \page install Installation
 *
 * In order to build and install the package you will need the following
 * packages:
 *
 * - The <a href="http://www.cmake.org">CMake</a> build system. At least version
 *   2.8
 * - <a href="http://www.boost.org">Boost C++ libraries</a>. Version 1.36 or
 *   greater.
 * - A working C/C++ compiler. At this time only \c gcc has been tested but other
 *   compilers should work. In the case of \c gcc please make sure that is at
 *   least version 4.2
 *
 * For the following instructions it is assumed that the directory /path/to/cprel
 * contains the sorce distribution of this project.
 *
 * Building instructions:
 * - Open a terminal and create a directory to for the object files:
 * \code
 *   mkdir /path/to/build-cprel
 *   cd /path/to/build-cprel
 * \endcode
 * - Configure and build:
 * \code
 *   cmake /path/to/cprel
 *   make && make install
 * \endcode
 *
 * If any dependency is missing the configuration process will detect and inform
 * you about that. The building process can be influenced by the definition of
 * several variables when calling \a cmake. As an example consider the cmmand:
 * \code
 *   cmake -DCMAKE_PREFIX_PATH=/usr/local /path/to/cprel
 * \endcode
 * That defines the variable \c CMAKE_PREFIX_PATH with the value <tt>/usr/local
 * </tt>. this particular variable tell \c cmake to look for headers and libraries
 * inside the <tt>/usr/local</tt> directory. This variable is specially useful if
 * for instance, boost libraries and headers are located in <tt>/usr/local/include
 * </tt> and <tt>/usr/local/lib</tt>.
 *
 * The configuration script will search a \a gecode installation. For this project
 * it is required that \a gecode is built with support for relation variables and
 * this is why a normal installation will not work. If after running \a cmake you
 * get a message that looks like: <tt>"No compatible version of gecode was found.
 * "</tt>, proceed as follows:
 *
 * - Tell \a cmake to build gecode for you:
 * \code
 *   make BUILD_GECODE
 *   cmake /path/to/cprel
 *   make && make install
 * \endcode
 * The first statement will download the right version of gecode (\a svn is
 * required) and will configure and install it. Please note that resulting gecode
 * offers the same functionality as the one provided from the gecode web page. The
 * only difference is that it is built with support for the new type of variables.
 *
 * At this point you should have a working version of the constraint system. If
 * something went wrong please <b>DO NOT</b> report the problems to the gecode
 * mailing list but instead contact the authors of CPRel.
 */


/**
 * \page license License information
 *
 * This package is distributed under the <a href="http://www.opensource.org/licenses/mit-license.php">MIT
 * </a> license.A copy of this licenseis included as part of the sources in the
 * file \c LICENSE.txt.
 *
 * Part of the functionality of this package relies on the <a href="http://vlsi.colorado.edu/~fabio/CUDD/">CUDD
 * </a> library. The sources of this library are included, however CUDD is distributed
 * under another license (see CUDD-LICENSE.txt). And its copyrighted:
 * \code Copyright (c) 1995-2004, Regents of the University of Colorado \endcode
 *
 */

/**
 * \page practical Practical constraint programming on relations
 *
 * \section declground Ground relation specification
 *
 * There are several ways to specify a ground relation. The most common one is
 * by specifying the set of tuples that conform it. See the documentation of the
 * CPRel::Tuple class for more information about how to create tuples.
 *
 */

/** \mainpage Constraint programming on relations (CPRel)
 *
 * \authors Gustavo Gutierrez: <tt>gutierrez.gustavo</tt> <b>at</b> uclouvain.be
 * \authors Yves Jaradin: <tt>yves.jaradin</tt> <b>at</b> uclouvain.be
 *
 * \section intro Introduction
 * This package provides the implementation of a constraint system on relations.
 * It is developed as a module for the <a href="http://www.gecode.org"> Gecode</a>
 * constraint library. This documentation is divided in:
 *
 * - Using the library.
 *   - \subpage install
 *   - Examples
 * - \subpage license
 * - Constraint programming on relations
 * - Acknowledgements
 *
 */
