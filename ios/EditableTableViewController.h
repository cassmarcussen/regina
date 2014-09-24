
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  iOS User Interface                                                    *
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

#import <UIKit/UIKit.h>

/**
 * The view controller for a table that supports actions on cells, such as
 * renaming or deleting cells.
 *
 * This superclass:
 *
 * - offers a place to register where actions are taking place (the \a actionPath property);
 * - offers a place for deleting cells after confirmation (the \a delete... methods, which
 *   must be overridden);
 * - offers a facility for renaming cells using a long press gesture (the \a rename... methods,
 *   which must likewise be overridden);
 * - manages the positioning of the table as the keyboard appears and disappears.
 *
 * All other functionality is left for subclasses to implement.
 *
 * You must implement tableView:canEditRowAtIndexPath: in your subclass,
 * to indicate which cells may be deleted.
 *
 * The only table view data source and action sheet delegate methods that this superclass
 * implements (and which subclass implementations must therefore call in this superclass)
 * are those listed below.  This class also implements UITextFieldDelegate internally,
 * but subclasses should not need to override any UITextFieldDelegate functions.
 */
@interface EditableTableViewController : UITableViewController <UIActionSheetDelegate>

/**
 * If the user is currently editing a table cell (e.g., renaming
 * or deleting it), then this stores the corresponding index in the table.
 * Otherwise this property is \a nil.
 *
 * Subclasses should update this property as the user initiates, cancels
 * and/or completes actions, and should disable appropriate parts of the user
 * interface whenever it is non-nil.
 */
@property (strong, nonatomic) NSIndexPath *actionPath;

/**
 * Subclasses must override this to indicate whether the user is allowed
 * to rename the table cell at the given index.
 * The default implementation simply returns \c NO.
 *
 * This is called when a long press is detected.
 */
- (BOOL)renameAllowed:(NSIndexPath*)path;

/**
 * Subclasses must override this to indicate what initial name to offer
 * the user when they begin a renaming operation.
 *
 * The default implementation returns \c nil (which is not a valid value).
 *
 * This will only be called when renaming begins.  You may assume that
 * renameAllowed: returned \c YES.
 */
- (NSString*)renameInit:(NSIndexPath*)path;

/**
 * Subclasses must override this to handle the result of a renaming operation.
 * When the user finishes typing the new name, this routine will be called.
 * The subclass implementation is responsible for integrating this back into the
 * user interface and refreshing the table cell as appropriate.
 *
 * The default implementation does nothing.
 *
 * This will only be called after renaming is finished.  You may assume
 * that renameAllowed: returned \c YES.
 *
 * @param result The new name as entered by the user.
 */
- (void)renameDone:(NSIndexPath*)path result:(NSString*)result;

/**
 * Subclasses must override this to return the label on the destructive
 * delete confirmation button.
 *
 * @param path Indicates the cell to be deleted.  It may be assumed
 * that this allows the delete action according to
 * tableView:canEditRowAtIndexPath: (which your subclass must also implement).
 */
- (NSString*)deleteConfirmation:(NSIndexPath*)path;

/**
 * Subclasses must override this to perform the action of deleting
 * a table cell.  It may be assumed that the user has already confirmed
 * the action.
 *
 * The index of the cell to be deleted wll be stored in the actionPath property.
 * Your subclass implementation should not reset this property (this will be
 * done later).
 */
- (void)deleteAction;

/**
 * Internal implementation to handle the delete action.
 *
 * Normally there is no need to override this in your subclasses.
 * However, if your subclass needs to override this to support additional editing styles,
 * then you \e must call the superclass implementation if \a editingStyle is
 * UITableViewCellEditingStyleDelete.
 */
- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath;

/**
 * Internal implementation to handle the delete confirmation action sheet.
 *
 * Normally there is no need to override this in your subclasses.
 * However, if your subclass needs to override this to support additional action sheets, then you
 * \e must use action sheet tags, and you \e must call the superclass implementation if actionSheet.tag
 * is not one of your own.  All tags used internally by this class are greater than 500 (so if your
 * subclass tags are at most 500 then there will be no conflicts).
 */
- (void)actionSheet:(UIActionSheet *)actionSheet clickedButtonAtIndex:(NSInteger)buttonIndex;

@end