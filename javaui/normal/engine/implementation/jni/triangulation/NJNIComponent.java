
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Java User Interface                                                   *
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

package normal.engine.implementation.jni.triangulation;

import normal.engine.implementation.jni.*;
import normal.engine.triangulation.*;

public class NJNIComponent extends JNIShareableObject implements NComponent {
    public NJNIComponent(Sentry s) {
        super(s);
    }
    
    public native boolean isIdeal();
    public native boolean isOrientable();
    public native boolean isClosed();

    public native long getNumberOfTetrahedra();
    public native long getNumberOfFaces();
    public native long getNumberOfEdges();
    public native long getNumberOfVertices();
    public native long getNumberOfBoundaryComponents();

    public native NTetrahedron getTetrahedron(long index);
    public native NFace getFace(long index);
    public native NEdge getEdge(long index);
    public native NVertex getVertex(long index);
    public native NBoundaryComponent getBoundaryComponent(long index);
}
