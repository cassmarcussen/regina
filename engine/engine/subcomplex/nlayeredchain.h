
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file nlayeredchain.h
 *  \brief Deals with layered chains in a triangulation.
 */

#ifndef __NLAYEREDCHAIN_H
#ifndef __DOXYGEN
#define __NLAYEREDCHAIN_H
#endif

#include "subcomplex/nstandardtri.h"
#include "triangulation/nperm.h"

namespace regina {

class NTetrahedron;

/**
 * \weakgroup subcomplex
 * @{
 */

/**
 * Represents a layered chain in a triangulation.
 *
 * A layered chain is a set of \a n tetrahedra glued to each other by
 * layerings.  For each tetrahedron, select two top faces, two bottom
 * faces and two hinge edges, so that the top faces are adjacent, the
 * bottom faces are adjacent, the hinge edges are opposite and each
 * hinge meets both a top and a bottom face.  The tetrahedron can thus
 * be thought of as a fattened square with the top and bottom faces
 * above and below the square respectively, and the hinges as the top
 * and bottom edges of the square.  The left and right edges of the
 * square are identified to form an annulus.
 *
 * For each \a i, the top faces of tetrahedron \a i are glued to the
 * bottom faces of tetrahedron <i>i</i>+1.  This is done by layering the
 * upper tetrahedron upon the annulus formed by the top faces of the
 * lower tetrahedron.  The layering should be done over the left or
 * right edge of the lower square (note that these two edges are
 * actually identified).  The top hinges of each tetrahedron should be
 * identified, as should the bottom hinges.
 *
 * The bottom faces of the first tetrahedron and the top faces of the
 * last tetrahedron form the boundary of the layered chain.  If there is
 * more than one tetrahedron, the layered chain forms a solid torus with
 * two vertices whose axis is parallel to each hinge edge.
 *
 * The \a index of the layered chain is the number of tetrahedra it
 * contains.  A layered chain must contain at least one tetrahedron.
 *
 * Note that for the purposes of getManifold() and getHomologyH1(), a
 * layered chain containing only one tetrahedron will be considered as a
 * standalone tetrahedron that forms a 3-ball (and not a solid torus).
 *
 * All optional NStandardTriangulation routines are implemented for this
 * class.
 */
class NLayeredChain : public NStandardTriangulation {
    private:
        NTetrahedron* bottom;
            /**< The bottom tetrahedron of this layered chain. */
        NTetrahedron* top;
            /**< The top tetrahedron of this layered chain. */
        unsigned long index;
            /**< The number of tetrahedra in this layered chain. */
        NPerm bottomVertexRoles;
            /**< The permutation described by getBottomVertexRoles(). */
        NPerm topVertexRoles;
            /**< The permutation described by getTopVertexRoles(). */

    public:
        /**
         * Creates a new layered chain containing only the given
         * tetrahedron.  This new layered chain will have index 1, but
         * may be extended using extendAbove(), extendBelow() or
         * extendMaximal().
         *
         * @param tet the tetrahedron that will make up this layered
         * chain.
         * @param vertexRoles a permutation describing the role each
         * tetrahedron vertex must play in the layered chain; this must be
         * in the same format as the permutation returned by
         * getBottomVertexRoles() and getTopVertexRoles().
         */
        NLayeredChain(NTetrahedron* tet, NPerm vertexRoles);

        /**
         * Creates a new layered chain that is a clone of the given
         * structure.
         *
         * @param cloneMe the layered chain to clone.
         */
        NLayeredChain(const NLayeredChain& cloneMe);

        /**
         * Destroys this layered chain.
         */
        virtual ~NLayeredChain();

        /**
         * Returns the bottom tetrahedron of this layered chain.
         *
         * @return the bottom tetrahedron.
         */
        NTetrahedron* getBottom() const;
        /**
         * Returns the top tetrahedron of this layered chain.
         *
         * @return the top tetrahedron.
         */
        NTetrahedron* getTop() const;
        /**
         * Returns the number of tetrahedra in this layered chain.
         *
         * @return the number of tetrahedra.
         */
        unsigned long getIndex() const;

        /**
         * Returns a permutation represeting the role that each vertex
         * of the bottom tetrahedron plays in the layered chain.
         * The permutation returned (call this <tt>p</tt>) maps 0, 1, 2 and
         * 3 to the four vertices of the bottom tetrahedron so that the
         * edge from <tt>p[0]</tt> to <tt>p[1]</tt> is the top hinge,
         * the edge from <tt>p[2]</tt> to <tt>p[3]</tt> is the bottom
         * hinge, faces <tt>p[1]</tt> and <tt>p[2]</tt> are the (boundary)
         * bottom faces and faces <tt>p[0]</tt> and <tt>p[3]</tt> are the top
         * faces.
         *
         * See the general class notes for further details.
         *
         * @return a permutation representing the roles of the vertices
         * of the bottom tetrahedron.
         */
        NPerm getBottomVertexRoles() const;

        /**
         * Returns a permutation represeting the role that each vertex
         * of the top tetrahedron plays in the layered chain.
         * The permutation returned (call this <tt>p</tt>) maps 0, 1, 2 and
         * 3 to the four vertices of the top tetrahedron so that the
         * edge from <tt>p[0]</tt> to <tt>p[1]</tt> is the top hinge,
         * the edge from <tt>p[2]</tt> to <tt>p[3]</tt> is the bottom
         * hinge, faces <tt>p[1]</tt> and <tt>p[2]</tt> are the bottom
         * faces and faces <tt>p[0]</tt> and <tt>p[3]</tt> are the
         * (boundary) top faces.
         *
         * See the general class notes for further details.
         *
         * @return a permutation representing the roles of the vertices
         * of the top tetrahedron.
         */
        NPerm getTopVertexRoles() const;

        /**
         * Checks to see whether this layered chain can be extended to
         * include the tetrahedron above the top tetrahedron (and still
         * remain a layered chain).  If so, this layered chain will be
         * modified accordingly (note that its index will be increased
         * by one and its top tetrahedron will change).
         *
         * @return \c true if and only if this layered chain was
         * extended.
         */
        bool extendAbove();

        /**
         * Checks to see whether this layered chain can be extended to
         * include the tetrahedron below the bottom tetrahedron (and still
         * remain a layered chain).  If so, this layered chain will be
         * modified accordingly (note that its index will be increased
         * by one and its bottom tetrahedron will change).
         *
         * @return \c true if and only if this layered chain was
         * extended.
         */
        bool extendBelow();

        /**
         * Extends this layered chain to a maximal length layered chain.
         * Both extendAbove() and extendBelow() will be used until this
         * layered chain can be extended no further.
         *
         * @return \c true if and only if this layered chain was
         * extended.
         */
        bool extendMaximal();

        /**
         * Reverses this layered chain so the top tetrahedron becomes
         * the bottom and vice versa.  The upper and lower hinges will
         * remain the upper and lower hinges respectively.
         *
         * Note that this operation will cause the hinge edges to point
         * in the opposite direction around the solid torus formed by
         * this layered chain.
         *
         * Note that only the representation of the chain is altered;
         * the underlying triangulation is not changed.
         */
        void reverse();

        /**
         * Inverts this layered chain so the upper hinge becomes the
         * lower and vice versa.  The top and bottom tetrahedra will
         * remain the top and bottom tetrahedra respectively.
         *
         * Note that this operation will cause the hinge edges to point
         * in the opposite direction around the solid torus formed by
         * this layered chain.
         *
         * Note that only the representation of the chain is altered;
         * the underlying triangulation is not changed.
         */
        void invert();

        NManifold* getManifold() const;
        NAbelianGroup* getHomologyH1() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;
};

/*@}*/

// Inline functions for NLayeredChain

inline NLayeredChain::NLayeredChain(NTetrahedron* tet, NPerm vertexRoles) :
        bottom(tet), top(tet), index(1), bottomVertexRoles(vertexRoles),
        topVertexRoles(vertexRoles) {
}
inline NLayeredChain::NLayeredChain(const NLayeredChain& cloneMe) :
        NStandardTriangulation(), bottom(cloneMe.bottom), top(cloneMe.top),
        index(cloneMe.index), bottomVertexRoles(cloneMe.bottomVertexRoles),
        topVertexRoles(cloneMe.topVertexRoles) {
}
inline NLayeredChain::~NLayeredChain() {
}

inline NTetrahedron* NLayeredChain::getBottom() const {
    return bottom;
}
inline NTetrahedron* NLayeredChain::getTop() const {
    return top;
}
inline unsigned long NLayeredChain::getIndex() const {
    return index;
}

inline NPerm NLayeredChain::getBottomVertexRoles() const {
    return bottomVertexRoles;
}
inline NPerm NLayeredChain::getTopVertexRoles() const {
    return topVertexRoles;
}

inline std::ostream& NLayeredChain::writeName(std::ostream& out) const {
    return out << "Chain(" << index << ')';
}
inline std::ostream& NLayeredChain::writeTeXName(std::ostream& out) const {
    return out << "$\\mathit{Chain}(" << index << ")$";
}
inline void NLayeredChain::writeTextLong(std::ostream& out) const {
    out << "Layered chain of index " << index;
}

} // namespace regina

#endif

