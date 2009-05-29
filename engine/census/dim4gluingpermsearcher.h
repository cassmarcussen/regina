
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file census/dim4gluingpermsearcher.h
 *  \brief Supports searching through all possible sets of pentachoron
 *  gluing permutations for a given pentachoron facet pairing.
 */

#ifndef __DIM4GLUINGPERMSEARCHER_H
#ifndef __DOXYGEN
#define __DIM4GLUINGPERMSEARCHER_H
#endif

#include "census/dim4gluingperms.h"

namespace regina {

/**
 * \weakgroup census
 * @{
 */

class Dim4GluingPermSearcher;

/**
 * A routine used to do arbitrary processing upon a particular set of
 * pentachoron gluing permutations.  Such routines are used to process
 * permutation sets found when running Dim4GluingPermSearcher::findAllPerms().
 *
 * The first parameter passed will be a set of gluing permutations
 * (in fact it will be of the subclass Dim4GluingPermSearcher in order to
 * support partial searches as well as full searches).  This set of
 * gluing permutations must not be deallocated by this routine, since it
 * may be used again later by the caller.  The second parameter may contain
 * arbitrary data as passed to either Dim4GluingPerms::findAllPerms() or
 * the Dim4GluingPermSearcher class constructor.
 *
 * Note that the first parameter passed might be \c null to signal that
 * gluing permutation generation has finished.
 */
typedef void (*UseDim4GluingPerms)(const Dim4GluingPermSearcher*, void*);

/**
 * A utility class for searching through all possible gluing permutation
 * sets that correspond to a given pentachoron face pairing.  Subclasses of
 * Dim4GluingPermSearcher correspond to specialised (and heavily optimised)
 * search algorithms that may be used in sufficiently constrained scenarios.
 * The main class Dim4GluingPermSearcher offers a default (but slower) search
 * algorithm that may be used in more general contexts.
 *
 * The simplest way of performing a search through all possible gluing
 * permutations is by calling the static method findAllPerms().  This will
 * examine the search parameters and ensure that the best possible algorithm
 * is used.  For finer control over the program flow, the static method
 * bestSearcher() can be used to create a search manager of the most
 * suitable class and then runSearch() can be called on this object directly.
 * For absolute control, a specific algorithm can be forced by explicitly
 * constructing an object of the corresponding class (and again
 * calling runSearch() on that object directly).
 *
 * Note that this class derives from Dim4GluingPerms.  The search will
 * involve building and repeatedly modifying the inherited Dim4GluingPerms
 * data in-place.
 *
 * \ifacespython Not present.
 */
class Dim4GluingPermSearcher : public Dim4GluingPerms {
    public:
        static const char dataTag_;
            /**< A character used to identify this class when reading
                 and writing tagged data in text format. */

    protected:
        const Dim4FacetPairingIsoList* autos_;
            /**< The set of isomorphisms that define equivalence of
                 gluing permutation sets.  Generally this is the set of all
                 automorphisms of the underlying facet pairing. */
        bool autosNew_;
            /**< Did we create the isomorphism list autos_ ourselves (in
                 which case we must destroy it also)? */
        bool orientableOnly_;
            /**< Are we only searching for gluing permutations that
                 correspond to orientable triangulations? */
        bool finiteOnly_;
            /**< Are we only searching for gluing permutations that
                 correspond to finite (non-ideal) triangulations? */
        UseDim4GluingPerms use_;
            /**< A routine to call each time a gluing permutation set is
                 found during the search. */
        void* useArgs_;
            /**< Additional user-supplied data to be passed as the second
                 argument to the \a use_ routine. */

        bool started_;
            /**< Has the search started yet?  This helps distinguish
                 between a new search and the resumption of a partially
                 completed search. */
        int* orientation_;
            /**< Keeps track of the orientation of each tetrahedron in the
                 underlying triangulation.  Orientation is positive/negative,
                 or 0 if unknown.
                 Note that in some algorithms the orientation is simply
                 +/-1, and in some algorithms the orientation counts
                 forwards or backwards from 0 according to how many
                 times the orientation has been set or verified. */

    protected:
        Dim4PentFacet* order_;
            /**< Describes the order in which gluing permutations are
                 assigned to pentachoron facets.  Specifically, this order is
                 order_[0], order_[1], ..., order_[orderSize_-1].

                 Note that each element of this array corresponds to a
                 single edge of the underlying facet pairing graph, which in
                 turn represents a pentachoron facet and its image under
                 the given facet pairing.

                 The specific pentachoron facet stored in this array for each
                 edge of the underlying facet pairing graph will be the smaller
                 of the two identified pentachoron facets. */
        int orderSize_;
            /**< The total number of edges in the facet pairing graph, i.e.,
                 the number of elements of interest in the order_[] array. */
        int orderElt_;
            /**< Marks which element of order_[] we are currently examining
                 at this stage of the search. */

    public:
        /**
         * Initialises a new search for gluing permutation sets.  The
         * search is started by calling runSearch().  Note that the
         * static method findAllPerms() handles both construction and
         * searching, and is the preferred entry point for end users.
         *
         * The arguments to this constructor describe the search
         * parameters in detail, as well as what should be done with
         * each gluing permutation set that is found.
         *
         * The appropriate use of parameters \a orientableOnly and
         * \a finiteOnly can significantly speed up the permutation
         * set generation.  For some combinations of these parameters
         * entirely different algorithms are used.
         *
         * Note that even if \a finiteOnly is set to \c true, some
         * ideal triangulations might still slip through the net
         * (since the full vertex links are not always constructed).
         * However, setting \a finiteOnly to \c true will
         * allow the census algorithm to take shortcuts and therefore
         * run faster.  The resulting triangulations may be tested for
         * finiteness (and other properties) by calling triangulate().
         *
         * \pre The given facet pairing is connected, i.e., it is possible
         * to reach any pentachoron from any other pentachoron via a
         * series of matched facet pairs.
         * \pre The given facet pairing is in canonical form as described
         * by Dim4FacetPairing::isCanonical().  Note that all facet pairings
         * constructed by Dim4FacetPairing::findAllPairings() are of this form.
         *
         * @param pairing the specific pairing of pentachoron facets
         * that the generated permutation sets will complement.
         * @param autos the collection of isomorphisms that define equivalence
         * of permutation sets.  These are used by runSearch(), which produces
         * each permutation set precisely once up to equivalence.  These
         * isomorphisms must all be automorphisms of the given facet pairing,
         * and will generally be the set of all such automorphisms.  This
         * parameter may be 0, in which case the set of all automorphisms
         * of the given facet pairing will be generated and used.
         * @param orientableOnly \c true if only gluing permutations
         * corresponding to orientable triangulations should be
         * generated, or \c false if no such restriction should be imposed.
         * @param finiteOnly \c true if only gluing permutations
         * corresponding to finite (non-ideal) triangulations are required, or
         * \c false if there is no such requirement.  Note that
         * regardless of this value, some ideal triangulations
         * might still be produced; see the notes above for details.
         * @param use the function to call upon each permutation set that
         * is found.  The first parameter passed to this function will be
         * a gluing permutation set.  The second parameter will be
         * parameter \a useArgs as was passed to this routine.
         * @param useArgs the pointer to pass as the final parameter for
         * the function \a use which will be called upon each permutation
         * set found.
         */
        Dim4GluingPermSearcher(const Dim4FacetPairing* pairing,
                const Dim4FacetPairingIsoList* autos,
                bool orientableOnly, bool finiteOnly,
                UseDim4GluingPerms use, void* useArgs = 0);

        /**
         * Initialises a new search manager based on data read from the
         * given input stream.  This may be a new search or a partially
         * completed search.
         *
         * This routine reads data in the format written by dumpData().
         * If you wish to read data whose precise class is unknown,
         * consider using dumpTaggedData() and readTaggedData() instead.
         *
         * If the data found in the input stream is invalid or incorrectly
         * formatted, the routine inputError() will return \c true but
         * the contents of this object will be otherwise undefined.
         *
         * \warning The data format is liable to change between Regina
         * releases.  Data in this format should be used on a short-term
         * temporary basis only.
         *
         * @param in the input stream from which to read.
         * @param use as for the main Dim4GluingPermSearcher constructor.
         * @param useArgs as for the main Dim4GluingPermSearcher constructor.
         */
        Dim4GluingPermSearcher(std::istream& in,
            UseDim4GluingPerms use, void* useArgs = 0);

        /**
         * Destroys this search manager and all supporting data
         * structures.
         */
        virtual ~Dim4GluingPermSearcher();

        /**
         * Generates all possible gluing permutation sets that satisfy
         * the current search criteria.  The search criteria are
         * specified in the class constructor, or through the static
         * method findAllPerms().
         *
         * Each set of gluing permutations will be produced precisely
         * once up to equivalence, where equivalence is defined by the
         * given set of automorphisms of the given facet pairing.
         *
         * For each permutation set that is generated, routine \a use_
         * (as passed to the class constructor) will be called with that
         * permutation set as an argument.
         *
         * Once the generation of permutation sets has finished, routine
         * \a use_ will be called once more, this time with \c null as its
         * first (permutation set) argument.
         *
         * Subclasses corresponding to more specialised search criteria
         * should override this routine to use a better optimised algorithm
         * where possible.
         *
         * It is possible to run only a partial search, branching to a
         * given depth but no further.  In this case, rather than
         * producing complete gluing permutation sets, the search will
         * produce a series of partially-complete Dim4GluingPermSearcher
         * objects.  These partial searches may then be restarted by
         * calling runSearch() once more (usually after being frozen or
         * passed on to a different processor).  If necessary, the \a use_
         * routine may call completePermSet() to distinguish between
         * a complete set of gluing permutations and a partial search state.
         *
         * Note that a restarted search will never drop below its
         * initial depth.  That is, calling runSearch() with a fixed
         * depth can be used to subdivide the overall search space into
         * many branches, and then calling runSearch() on each resulting
         * partial search will complete each of these branches without overlap.
         *
         * \todo \feature Allow cancellation of permutation set generation.
         *
         * @param maxDepth the depth of the partial search to run, or a
         * negative number if a full search should be run (the default).
         */
        virtual void runSearch(long maxDepth = -1);

        /**
         * Determines whether this search manager holds a complete
         * gluing permutation set or just a partially completed search
         * state.
         *
         * This may assist the \a use_ routine when running partial
         * depth-based searches.  See runSearch() for further details.
         *
         * @return \c true if a complete gluing permutation set is held,
         * or \c false otherwise.
         */
        bool completePermSet() const;

        /**
         * Dumps all internal data in a plain text format, along with a
         * marker to signify which precise class the data belongs to.
         * This routine can be used with readTaggedData() to transport
         * objects from place to place whose precise class is unknown.
         *
         * \warning The data format is liable to change between Regina
         * releases.  Data in this format should be used on a short-term
         * temporary basis only.
         *
         * @param out the output stream to which the data should be
         * written.
         */
        void dumpTaggedData(std::ostream& out) const;

        // Overridden methods:
        virtual void dumpData(std::ostream& out) const;

        /**
         * The main entry routine for running a search for all gluing
         * permutation sets that complement a given pentachoron facet pairing.
         *
         * This routine examines the search parameters, chooses the best
         * possible search algorithm, constructs an object of the
         * corresponding subclass of Dim4GluingPermSearcher and then calls
         * runSearch().
         *
         * See the Dim4GluingPermSearcher constructor for documentation on
         * the arguments to this routine.  See the runSearch() method
         * for documentation on how the search runs and returns its
         * results.
         *
         * \pre The given facet pairing is connected, i.e., it is possible
         * to reach any pentachoron from any other pentachoron via a
         * series of matched facet pairs.
         * \pre The given facet pairing is in canonical form as described
         * by Dim4FacetPairing::isCanonical().  Note that all facet pairings
         * constructed by Dim4FacetPairing::findAllPairings() are of this form.
         */
        static void findAllPerms(const Dim4FacetPairing* pairing,
                const Dim4FacetPairingIsoList* autos,
                bool orientableOnly, bool finiteOnly,
                UseDim4GluingPerms use, void* useArgs = 0);

        /**
         * Constructs a search manager of the best possible class for the
         * given search parameters.  Different subclasses of
         * Dim4GluingPermSearcher provide optimised search algorithms for
         * different types of search.
         *
         * Calling this routine and then calling runSearch() on the
         * result has the same effect as the all-in-one routine
         * findAllPerms().  Unless you have specialised requirements
         * (such as partial searching), you are probably better calling
         * findAllPerms() instead.
         *
         * The resulting object is newly created, and must be destroyed
         * by the caller of this routine.
         *
         * See the Dim4GluingPermSearcher constructor for documentation on
         * the arguments to this routine.
         *
         * \pre The given facet pairing is connected, i.e., it is possible
         * to reach any pentachoron from any other pentachoron via a
         * series of matched facet pairs.
         * \pre The given facet pairing is in canonical form as described
         * by Dim4FacetPairing::isCanonical().  Note that all facet pairings
         * constructed by Dim4FacetPairing::findAllPairings() are of this form.
         *
         * @return the newly created search manager.
         */
        static Dim4GluingPermSearcher* bestSearcher(
                const Dim4FacetPairing* pairing,
                const Dim4FacetPairingIsoList* autos,
                bool orientableOnly, bool finiteOnly,
                UseDim4GluingPerms use, void* useArgs = 0);

        /**
         * Creates a new search manager based on tagged data read from
         * the given input stream.  This may be a new search or a
         * partially completed search.
         *
         * The tagged data should be in the format written by
         * dumpTaggedData().  The precise class of the search manager
         * will be determined from the tagged data, and does not need to
         * be known in advance.  This is in contrast to dumpData() and
         * the input stream constructors, where the class of the data being
         * read must be known at compile time.
         *
         * If the data found in the input stream is invalid or
         * incorrectly formatted, a null pointer will be returned.
         * Otherwise a newly constructed search manager will be returned,
         * and it is the responsibility of the caller of this routine to
         * destroy it after use.
         *
         * The arguments \a use and \a useArgs are the smae as for the
         * Dim4GluingPermSearcher constructor.
         *
         * \warning The data format is liable to change between Regina
         * releases.  Data in this format should be used on a short-term
         * temporary basis only.
         *
         * @param in the input stream from which to read.
         */
        static Dim4GluingPermSearcher* readTaggedData(std::istream& in,
                UseDim4GluingPerms use, void* useArgs = 0);

    protected:
        /**
         * Compares the current set of gluing permutations with its
         * preimage under each automorphism of the underlying facet pairing,
         * in order to see whether the current set is in canonical form
         * (i.e., is lexicographically smallest).
         *
         * @return \c true if the current set is in canonical form,
         * or \c false otherwise.
         */
        bool isCanonical() const;

        /**
         * Determines whether the permutations already constructed model
         * a 4-manifold triangulation with a (2-dimensional) face identified
         * with itself using a non-trivial rotation or reflection.
         *
         * Tests that do not refer to the gluing permutation for the
         * given pentachoron facet will not be run.
         *
         * This routine is not fussy about the order in which gluing
         * permutations are selected, as long as permutations not yet
         * selected have the corresponding element of permIndices[] set
         * to -1.
         *
         * @param facet the specific pentachoron facet upon which tests
         * will be based.
         * @return \c true if the permutations under construction will
         * lead to a face identified with itself using a non-trivial
         * rotation or reflection, or \c false if no such face is found.
         */
        bool badFaceLink(const Dim4PentFacet& facet) const;

        /**
         * Returns the character used to identify this class when
         * storing tagged data in text format.
         *
         * @return the class tag.
         */
        virtual char dataTag() const;
};

/*@}*/

// Inline functions for Dim4GluingPermSearcher

inline bool Dim4GluingPermSearcher::completePermSet() const {
    return (orderElt_ == orderSize_);
}

inline char Dim4GluingPermSearcher::dataTag() const {
    return Dim4GluingPermSearcher::dataTag_;
}

} // namespace regina

#endif

