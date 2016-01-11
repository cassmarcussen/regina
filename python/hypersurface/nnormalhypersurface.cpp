
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
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

#include <boost/python.hpp>
#include "dim4/dim4triangulation.h"
#include "hypersurface/nnormalhypersurface.h"
#include "triangulation/ntriangulation.h"
#include "../helpers.h"

using namespace boost::python;
using regina::NNormalHypersurface;
using regina::Dim4Triangulation;

namespace {
    void writeTextShort_stdio(const NNormalHypersurface& s) {
        s.writeTextShort(std::cout);
    }
    void writeRawVector_stdio(const NNormalHypersurface& s) {
        s.writeRawVector(std::cout);
    }
}

void addNNormalHypersurface() {
    class_<NNormalHypersurface, std::auto_ptr<NNormalHypersurface>,
            boost::noncopyable>("NNormalHypersurface", no_init)
        .def("clone", &NNormalHypersurface::clone,
            return_value_policy<manage_new_object>())
        .def("getTetrahedronCoord", &NNormalHypersurface::getTetrahedronCoord)
        .def("getPrismCoord", &NNormalHypersurface::getPrismCoord)
        .def("getEdgeWeight", &NNormalHypersurface::getEdgeWeight)
        .def("countCoords", &NNormalHypersurface::countCoords)
        .def("getNumberOfCoords", &NNormalHypersurface::getNumberOfCoords)
        .def("getTriangulation", &NNormalHypersurface::getTriangulation,
            return_value_policy<reference_existing_object>())
        .def("getName", &NNormalHypersurface::getName,
            return_value_policy<return_by_value>())
        .def("setName", &NNormalHypersurface::setName)
        .def("writeTextShort", writeTextShort_stdio)
        .def("writeRawVector", writeRawVector_stdio)
        .def("isEmpty", &NNormalHypersurface::isEmpty)
        .def("isCompact", &NNormalHypersurface::isCompact)
        .def("hasRealBoundary", &NNormalHypersurface::hasRealBoundary)
        .def("isVertexLinking", &NNormalHypersurface::isVertexLinking)
        .def("isVertexLink", &NNormalHypersurface::isVertexLink,
            return_value_policy<reference_existing_object>())
        .def("isThinEdgeLink", &NNormalHypersurface::isThinEdgeLink,
            return_value_policy<reference_existing_object>())
        .def("triangulate", &NNormalHypersurface::triangulate,
            return_value_policy<manage_new_object>())
        .def("sameSurface", &NNormalHypersurface::sameSurface)
        .def("embedded", &NNormalHypersurface::embedded)
        .def("locallyCompatible", &NNormalHypersurface::locallyCompatible)
        .def("str", &NNormalHypersurface::str)
        .def("toString", &NNormalHypersurface::toString)
        .def("detail", &NNormalHypersurface::detail)
        .def("toStringLong", &NNormalHypersurface::toStringLong)
        .def("__str__", &NNormalHypersurface::str)
        .def(regina::python::add_eq_operators())
    ;
}

