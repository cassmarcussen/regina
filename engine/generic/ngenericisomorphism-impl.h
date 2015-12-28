
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/*! \file generic/ngenericisomorphism-impl.h
 *  \brief Contains implementation details for the NGenericIsomorphism
 *  class template.
 *
 *  This file is \e not included automatically by ngenericisomorphism.h.
 *  However, typical end users should never need to include it, since
 *  Regina's calculation engine provides full explicit instantiations
 *  of NGenericIsomorphism for \ref stddim "standard dimensions".
 */

#ifndef __NGENERICISOMORPHISM_IMPL_H
#ifndef __DOXYGEN
#define __NGENERICISOMORPHISM_IMPL_H
#endif

#include <algorithm>
#include <cstdlib>
#include "packet/npacket.h"
#include "generic/ngenericisomorphism.h"

namespace regina {

template <int dim>
void NGenericIsomorphism<dim>::writeTextShort(std::ostream& out) const {
    out << "Isomorphism between " << dim << "-manifold triangulations";
}

template <int dim>
void NGenericIsomorphism<dim>::writeTextLong(std::ostream& out) const {
    for (unsigned i = 0; i < nSimplices_; ++i)
        out << i << " -> " << simpImage_[i] << " (" << facetPerm_[i] << ")\n";
}

template <int dim>
bool NGenericIsomorphism<dim>::isIdentity() const {
    for (unsigned p = 0; p < nSimplices_; ++p) {
        if (simpImage_[p] != static_cast<int>(p))
            return false;
        if (! facetPerm_[p].isIdentity())
            return false;
    }
    return true;
}

template <int dim>
NGenericIsomorphism<dim>::NGenericIsomorphism(
        const NGenericIsomorphism<dim>& cloneMe) :
        nSimplices_(cloneMe.nSimplices_),
        simpImage_(cloneMe.nSimplices_ > 0 ?
            new int[cloneMe.nSimplices_] : 0),
        facetPerm_(cloneMe.nSimplices_ > 0 ?
            new Perm[cloneMe.nSimplices_] : 0) {
    std::copy(cloneMe.simpImage_, cloneMe.simpImage_ + nSimplices_,
        simpImage_);
    std::copy(cloneMe.facetPerm_, cloneMe.facetPerm_ + nSimplices_,
        facetPerm_);
}

template <int dim>
typename NGenericIsomorphism<dim>::Isomorphism* NGenericIsomorphism<dim>::
        inverse() const {
    Isomorphism* ans = new Isomorphism(nSimplices_);

    unsigned i;
    for (int i = 0; i < nSimplices_; i++) {
        ans->simpImage_[simpImage_[i]] = i;
        ans->facetPerm_[simpImage_[i]] = facetPerm_[i].inverse();
    }
    return ans;

}

template <int dim>
typename NGenericIsomorphism<dim>::Isomorphism* NGenericIsomorphism<dim>::
        random(unsigned nSimplices) {
    Isomorphism* ans = new Isomorphism(nSimplices);

    // Randomly choose the destination simplices.
    unsigned i;
    for (i = 0; i < nSimplices; i++)
        ans->simpImage_[i] = i;
    std::random_shuffle(ans->simpImage_, ans->simpImage_ + nSimplices);

    // Randomly choose the individual permutations.
    for (i = 0; i < nSimplices; i++)
        ans->facetPerm_[i] = DimTraits<dim>::Perm::Sn[rand() %
            DimTraits<dim>::Perm::nPerms];

    return ans;
}

template <int dim>
typename DimTraits<dim>::Triangulation* NGenericIsomorphism<dim>::apply(
        const typename DimTraits<dim>::Triangulation* original) const {
    if (original->getNumberOfSimplices() != nSimplices_)
        return 0;

    if (nSimplices_ == 0)
        return new Triangulation();

    Triangulation* ans = new Triangulation();
    Simplex** tet = new Simplex*[nSimplices_];
    unsigned long t;
    int f;

    NPacket::ChangeEventSpan span(ans);
    for (t = 0; t < nSimplices_; t++)
        tet[t] = ans->newSimplex();

    for (t = 0; t < nSimplices_; t++)
        tet[simpImage_[t]]->setDescription(
            original->getSimplex(t)->getDescription());

    const Simplex *myTet, *adjTet;
    unsigned long adjTetIndex;
    Perm gluingPerm;
    for (t = 0; t < nSimplices_; t++) {
        myTet = original->getSimplex(t);
        for (f = 0; f <= dim; f++)
            if ((adjTet = myTet->adjacentSimplex(f))) {
                // We have an adjacent simplex.
                adjTetIndex = original->simplexIndex(adjTet);
                gluingPerm = myTet->adjacentGluing(f);

                // Make the gluing from one side only.
                if (adjTetIndex > t || (adjTetIndex == t &&
                        gluingPerm[f] > f))
                    tet[simpImage_[t]]->joinTo(facetPerm_[t][f],
                        tet[simpImage_[adjTetIndex]],
                        facetPerm_[adjTetIndex] * gluingPerm *
                            facetPerm_[t].inverse());
            }
    }

    delete[] tet;
    return ans;
}

template <int dim>
void NGenericIsomorphism<dim>::applyInPlace(
        typename DimTraits<dim>::Triangulation* tri) const {
    if (tri->getNumberOfSimplices() != nSimplices_)
        return;

    if (nSimplices_ == 0)
        return;

    Triangulation staging;
    Simplex** tet = new Simplex*[nSimplices_];
    unsigned long t;
    int f;

    NPacket::ChangeEventSpan span1(&staging);
    for (t = 0; t < nSimplices_; t++)
        tet[t] = staging.newSimplex();

    for (t = 0; t < nSimplices_; t++)
        tet[simpImage_[t]]->setDescription(
            tri->getSimplex(t)->getDescription());

    const Simplex *myTet, *adjTet;
    unsigned long adjTetIndex;
    Perm gluingPerm;
    for (t = 0; t < nSimplices_; t++) {
        myTet = tri->getSimplex(t);
        for (f = 0; f <= dim; f++)
            if ((adjTet = myTet->adjacentSimplex(f))) {
                // We have an adjacent simplex.
                adjTetIndex = tri->simplexIndex(adjTet);
                gluingPerm = myTet->adjacentGluing(f);

                // Make the gluing from one side only.
                if (adjTetIndex > t || (adjTetIndex == t &&
                        gluingPerm[f] > f))
                    tet[simpImage_[t]]->joinTo(facetPerm_[t][f],
                        tet[simpImage_[adjTetIndex]],
                        facetPerm_[adjTetIndex] * gluingPerm *
                            facetPerm_[t].inverse());
            }
    }

    delete[] tet;

    NPacket::ChangeEventSpan span2(tri);
    tri->removeAllSimplices();
    tri->swapContents(staging);
}

} // namespace regina

#endif
