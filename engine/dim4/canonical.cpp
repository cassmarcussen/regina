
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
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

#include "dim4/dim4isomorphism.h"
#include "dim4/dim4triangulation.h"

namespace regina {

namespace {
    /**
     * For internal use by makeCanonical().  This routines assumes that
     * the preimage of pentachronn 0 has been fixed (along with the
     * corresponding facet permutation), and tries to extend
     * this to a "possibly canonical" isomorphism.
     *
     * If it becomes clear that the isomorphism cannot be made canonical
     * and/or cannot be made better (i.e., lexicographically smaller) than
     * the best isomorphism found so far, this routine returns \c false
     * (possibly before the full isomorphism has been constructed).
     * Otherwise it returns \c true (and it is guaranteed that the
     * isomorphism is both fully constructed and moreover a strict
     * improvement upon the best found so far).
     *
     * This routine currently only works for connected triangulations.
     */
    bool extendIsomorphism(const Dim4Triangulation* tri,
            Dim4Isomorphism& current, Dim4Isomorphism& currentInv,
            const Dim4Isomorphism& best, const Dim4Isomorphism& bestInv) {
        bool better = false;

        unsigned nPents = tri->getNumberOfPentachora();
        unsigned pent;

        for (pent = 0; pent < nPents; ++pent)
            if (pent != currentInv.simpImage(0))
                current.simpImage(pent) = -1;

        int facet;

        unsigned origPent, origPentBest;
        int origFacet, origFacetBest;

        Dim4Pentachoron *adjPent, *adjPentBest;
        unsigned adjPentIndex, adjPentIndexBest;
        unsigned finalImage, finalImageBest;

        NPerm5 gluingPerm, gluingPermBest;
        NPerm5 finalGluing, finalGluingBest;
        int comp;

        bool justAssigned;
        unsigned lastAssigned = 0;
        for (pent = 0; pent < nPents; ++pent) {
            // INV: We have already selected the preimage of pent and
            // the corresponding facet permutation by the time we reach
            // this point.
            origPent = currentInv.simpImage(pent);
            origPentBest = bestInv.simpImage(pent);

            for (facet = 0; facet < 5; ++facet) {
                origFacet = current.facetPerm(origPent).preImageOf(facet);
                origFacetBest = best.facetPerm(origPentBest).preImageOf(facet);

                // Check out the adjacency along pent/facet.
                adjPent = tri->getPentachoron(origPent)->
                    adjacentPentachoron(origFacet);
                adjPentIndex = (adjPent ?
                    tri->pentachoronIndex(adjPent) : nPents);
                adjPentBest = tri->getPentachoron(origPentBest)->
                    adjacentPentachoron(origFacetBest);
                adjPentIndexBest = (adjPentBest ?
                    tri->pentachoronIndex(adjPentBest) : nPents);

                justAssigned = false;
                if (adjPent && current.simpImage(adjPentIndex) < 0) {
                    // We have a new pentachoron that needs assignment.
                    ++lastAssigned;
                    current.simpImage(adjPentIndex) = lastAssigned;
                    currentInv.simpImage(lastAssigned) = adjPentIndex;
                    justAssigned = true;
                }

                finalImage = (adjPent ?
                    current.simpImage(adjPentIndex) : nPents);
                finalImageBest = (adjPentBest ?
                    best.simpImage(adjPentIndexBest) : nPents);

                // We now have a gluing (but possibly not a gluing
                // permutation).  Compare adjacent pentachoron indices.
                if ((! better) && finalImage > finalImageBest)
                    return false; // Worse than best-so-far.
                if (finalImage < finalImageBest)
                    better = true;

                // Time now to look at the gluing permutation.
                if (! adjPent)
                    continue;

                gluingPerm = tri->getPentachoron(origPent)->
                    adjacentGluing(origFacet);
                gluingPermBest = tri->getPentachoron(origPentBest)->
                    adjacentGluing(origFacetBest);

                if (justAssigned) {
                    // We can choose the permutation ourselves.
                    // Make it so that the final gluing (computed later
                    // below) becomes the identity.
                    current.facetPerm(adjPentIndex) =
                        current.facetPerm(origPent) * gluingPerm.inverse();
                    currentInv.facetPerm(lastAssigned) =
                        current.facetPerm(adjPentIndex).inverse();
                }

                // Although adjPent is guaranteed to exist, adjPentBest is
                // not.  However, if adjPentBest does not exist then our
                // isomorphism-under-construction must already be an
                // improvement over best.
                if (better)
                    continue;

                // Now we are guaranteed that adjPentBest exists.
                finalGluing = current.facetPerm(adjPentIndex) *
                    gluingPerm * current.facetPerm(origPent).inverse();
                finalGluingBest = best.facetPerm(adjPentIndexBest) *
                    gluingPermBest * best.facetPerm(origPentBest).inverse();

                comp = finalGluing.compareWith(finalGluingBest);
                if ((! better) && comp > 0)
                    return false; // Worse than best-so-far.
                if (comp < 0)
                    better = true;
            }
        }

        return better;
    }
}

bool Dim4Triangulation::makeCanonical() {
    unsigned nPents = getNumberOfPentachora();

    // Get the empty triangulation out of the way.
    if (nPents == 0)
        return false;

    // Prepare to search for isomorphisms.
    Dim4Isomorphism current(nPents), currentInv(nPents);
    Dim4Isomorphism best(nPents), bestInv(nPents);

    // The thing to best is the identity isomorphism.
    unsigned pent, inner;
    for (pent = 0; pent < nPents; ++pent) {
        best.simpImage(pent) = bestInv.simpImage(pent) = pent;
        best.facetPerm(pent) = bestInv.facetPerm(pent) = NPerm5();
    }

    // Run through potential preimages of pentachoron 0.
    int perm;
    for (pent = 0; pent < nPents; ++pent) {
        for (perm = 0; perm < 120; ++perm) {
            // Build a "perhaps canonical" isomorphism based on this
            // preimage of pentachoron 0.
            current.simpImage(pent) = 0;
            currentInv.simpImage(0) = pent;

            current.facetPerm(pent) = NPerm5::S5[NPerm5::invS5[perm]];
            currentInv.facetPerm(0) = NPerm5::S5[perm];

            if (extendIsomorphism(this, current, currentInv, best, bestInv)) {
                // This is better than anything we've seen before.
                for (inner = 0; inner < nPents; ++inner) {
                    best.simpImage(inner) = current.simpImage(inner);
                    best.facetPerm(inner) = current.facetPerm(inner);
                    bestInv.simpImage(inner) = currentInv.simpImage(inner);
                    bestInv.facetPerm(inner) = currentInv.facetPerm(inner);
                }
            }
        }
    }

    // Is there anything to do?
    if (best.isIdentity())
        return false;

    // Do it.
    best.applyInPlace(this);
    return true;
}

} // namespace regina
