
/**************************************************************************
 *                                                                        *
 *  Regina - A normal surface theory calculator                           *
 *  Computational engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2001, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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

/*! \file docs.h
 *  \brief Contains miscellaneous documentation.
 */

/*! \page GeneralNotes General Notes
 *
 *  <center><b>
 *  Regina<br>
 *  The Application Formerly Known As Normal<br>
 *  Copyright &copy; 1999-2001, Ben Burton
 *  </b>
 *  <p>
 *  Written by Ben Burton
 *  <p>
 *  Normal 1.0 was written by David Letscher, who also provided invaluable
 *  technical advice for this project.
 *  </center>
 *
 *  <h3>Regina 2.1 - A normal surface theory calculator</h3>
 *
 *  Additional code contributed by:
 *  <ul>
 *     <li>David Letscher</li>
 *  </ul>
 *  Thanks to:
 *  <ul>
 *      <li>William Jaco</li>
 *      <li>Hyam Rubinstein</li>
 *      <li>Jeff Weeks</li>
 *      <li>The American Institute of Mathematics</li>
 *      <li>Oklahoma State University</li>
 *      <li>The University of Melbourne, Australia</li>
 *  </ul>
 *
 *  This documentation describes the functions, classes et al in the
 *  C++ computational engine, as well as the mirroring of these in the
 *  various external interfaces (such as the Java interface).
 *
 *  Contact Ben Burton (<a href="mailto:benb@acm.org">benb@acm.org</a>)
 *  for more details.
 *
 *  Regina has been released under the GNU General Public License, as
 *  described below.
 *
 *  It is hereby explicitly allowed that this program may be used
 *  with any Java implementation without the GPL applying to that
 *  Java implementation.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307, USA.
 */

/*! \page InterfaceNotes External Interfaces
 *
 *  Most classes and functions in the engine are mirrored in the
 *  various external interfaces to
 *  allow for a simple user interface through which both high and low
 *  level tasks can be performed.
 *  See the notes page for each external interface for details
 *  specific to that particular interface.
 *
 *  Unless otherwise specified, the entities mirrored are:
 *  \arg global functions;
 *  \arg global variables;
 *  \arg classes and all their public data members and functions.
 *
 *  For classes in the engine derived from ShareableObject, the
 *  corresponding interface classes are nothing more than vacuous wrappers
 *  that refuse to do any real work.  The underlying C++ engine stores
 *  all the real data and does all the real work.
 *
 *  Within any interface, an object of class Engine will represent the link
 *  between the interface and the underlying engine.
 *  To create a new object in the underlying engine of class \a Foo
 *  with a constructor that takes parameters \a x and \a y, the routine
 *  <tt>Engine::newFoo(x,y)</tt> should be called.  This
 *  will return a wrapper for a newly created
 *  \a Foo object in the underlying C++ engine.
 *
 *  All global functions and variables will be member functions of
 *  Engine and will only be accessible through an Engine object,
 *  unless otherwise specified.  The engine documentation will
 *  contain details regarding what becomes of static C++ class members.
 *
 *  Note that there might be many wrapper objects floating about for a single 
 *  underlying object in the C++ engine.  For this reason, the function
 *  ShareableObject::sameObject() is provided to test if two wrappers in
 *  fact refer to the same underlying object in the engine.
 * 
 *  Furthermore, because an engine object can have multiple wrappers,
 *  when a wrapper is destroyed it cannot simply destroy the
 *  corresponding engine object.  It is thus the user's responsibility
 *  to do this themselves; the default destructor for the wrapper object
 *  will do nothing.  The function ShareableObject::destroy() is
 *  provided to destroy the underlying engine object that a wrapper
 *  refers to.  Calling this function when you are finished with an
 *  object is critical to avoiding memory leaks.
 *
 *  The situation may arise in which the class of a wrapper
 *  corresponds to a superclass of the class of the
 *  underlying engine object.  In this case, ShareableObject::castAs()
 *  may be used to create a new wrapper of the correct wrapper class.
 *
 *  Note the routine Engine::testEngine() which is provided to test whether
 *  communications between the interface and the C++ engine are
 *  working properly.
 *
 *  Objects of C++ class NBoolSet will always be passed as characters,
 *  where the character is the corresponding byte code (see
 *  NBoolSet::getByteCode()).
 *  Objects of C++ class NPerm will always be passed as characters, where the
 *  character is the corresponding permutation code (see NPerm::getPermCode()).
 */

/*! \page CorbaNotes CORBA Interface
 *  
 *  CORBA may be used to communicate between a user interface and
 *  the calculation engine (possibly running on a different machine).
 *  The CORBA interface details are identical to those explained on the
 *  \ref InterfaceNotes page unless otherwise specified.  The exception
 *  is that global variables and public data members of classes are not
 *  mirrored in CORBA.
 *
 *  The CORBA objects on the CORBA server are wrappers for the
 *  corresponding \a real engine objects.  Thus the CORBA server
 *  itself may contain multiple CORBA wrappers for the same underlying
 *  engine object, and ShareableObject::sameObject() should be used to
 *  determine if two different CORBA objects on the server wrap the same
 *  underlying engine object.
 *
 *  A single object of type Engine will accessible through the naming
 *  service as <tt>regina/Engine</tt>,
 *  with kind attributes <tt>context/object</tt>.
 *  A CORBA client should connect to this Engine object on the CORBA
 *  server, from which new objects in the engine can then be created.
 *
 *  If two routines have the same name but take different types of
 *  arguments, the corresponding CORBA routines will have the argument
 *  types appended to the routine names to ensure that routine names are
 *  unique.  For instance, if a class contains routines
 *  <tt>saute()</tt>, <tt>saute(long)</tt> and <tt>saute(NEdible&)</tt>,
 *  the corresponding CORBA routines will be named <tt>saute_</tt>,
 *  <tt>saute_long</tt> and <tt>saute_NEdible</tt> respectively.
 *
 *  Objects of C++ class NLargeInteger will be passed as strings
 *  containing the corresponding decimal representations or <tt>inf</tt>
 *  for infinity.  Objects of C++ type <tt>int</tt> will be passed as
 *  type <tt>long</tt>.
 *
 *  Objects of C++ class NVertexEmbedding, NEdgeEmbedding or
 *  NFaceEmbedding will be replaced with two parameters of types
 *  NTetrahedron and <tt>long</tt>, representing the two parameters
 *  passed to the corresponding <tt>NxxxEmbeding</tt> constructor.  If
 *  such an object is to returned from a function, the function will
 *  become void and the two new parameters will appear as <tt>out</tt>
 *  parameters at the beginning of the function parameter list.
 *
 *  See the \ref InterfaceNotes page for other situations in which
 *  names or parameters of routines are routinely altered.
 *
 *  \todo \bugurgent Non-standard host/port for the CORBA engine is
 *  completely broken as of the omniORB3 port!
 *  \todo \bug Audit CORBA classes for use of \c _ptr instead of \c _var.
 */

/*! \page JavaNotes Java User Interface
 *
 *  Java wrapper classes are available in the package
 *  <tt>normal.engine</tt>.  Each wrapper class is in a subpackage of
 *  <tt>normal.engine</tt> corresponding to the subdirectory of
 *  <tt>engine</tt> in the C++ source that the corresponding C++ headers
 *  lie in.
 *
 *  Each Java wrapper class is in fact an interface, allowing for
 *  multiple forms of Java-to-C++ connection (such as JNI or CORBA).
 *  The package <tt>normal.engine.implementation</tt> stores
 *  implementations of the different connection types; these
 *  implementation classes should never be referenced directly.
 *
 *  Instead, a single object implementing the interface
 *  <tt>normal.engine.Engine</tt> should be
 *  created to reflect the specific connection being used; this is the
 *  only time the package <tt>normal.engine.implementation</tt> should
 *  ever be referred to.  The creation of this object will initialise
 *  the Java-to-C++ connection, and all further direct interactions with
 *  the engine should be through this object.
 *
 *  Since Java does not allow for unsigned integral types, the qualifier
 *  \c unsigned should be interpreted as a precondition that the
 *  corresponding parameter must be non-negative.
 *
 *  The C++ NLargeInteger class corresponds to the Java
 *  <tt>java.math.BigInteger</tt> class.
 *
 *  If a C++ routine takes an NDoubleList argument, the corresponding
 *  Java routine should take a <tt>java.util.Enumeration</tt> of objects
 *  of the appropriate class.
 *
 *  See the \ref InterfaceNotes page for other situations in which
 *  names or parameters of routines are routinely altered.
 *
 *  Note that when working with scripts and the JPython console, Java
 *  objects, classes and methods can be directly accessed as if they
 *  were JPython objects, classes and methods.
 *
 *  \todo \featurelong Have \a checkjni utility that checks consistency
 *  between <tt>.h</tt> and <tt>.cpp</tt> java wrapper files.
 */
