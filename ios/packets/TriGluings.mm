
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

#import "TriangulationViewController.h"
#import "TriGluings.h"
#import "triangulation/ntriangulation.h"

// TODO: Don't forget the keyboard scrolling nonsense.
// TODO: A few deletes and then we get stuck.
// TODO: Extend height of tap region to the entire cell.
// TODO: Use a padlock to indicate uneditability?

#pragma mark - Table cell

@interface TriGluingCell : UITableViewCell
@property (weak, nonatomic) IBOutlet UILabel *index;
@property (weak, nonatomic) IBOutlet UILabel *face3;
@property (weak, nonatomic) IBOutlet UILabel *face2;
@property (weak, nonatomic) IBOutlet UILabel *face1;
@property (weak, nonatomic) IBOutlet UILabel *face0;
@end

@implementation TriGluingCell
@end

#pragma mark - Triangulation gluings

@interface TriGluings () <UITableViewDelegate, UITableViewDataSource, UITextFieldDelegate> {
    CGFloat headerHeight;
    UILabel* editLabel;
    UITextField* editField;
    int editSimplex;
    int editFacet; // -1 for editing the description
    BOOL myEdit;
}
@property (weak, nonatomic) IBOutlet UILabel *header;
@property (weak, nonatomic) IBOutlet UITableView *tetrahedra;

@property (strong, nonatomic) TriangulationViewController* viewer;
@property (assign, nonatomic) regina::NTriangulation* packet;
@end

@implementation TriGluings

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.viewer = static_cast<TriangulationViewController*>(self.parentViewController);
    
    UITapGestureRecognizer *r = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(touched:)];
    [self.tetrahedra addGestureRecognizer:r];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    self.packet = self.viewer.packet;

    self.tetrahedra.delegate = self;
    self.tetrahedra.dataSource = self;
    
    [self reloadPacket];
}

- (void)reloadPacket
{
    if (myEdit)
        return;
    
    [self.viewer updateHeader:self.header];
    [self.tetrahedra reloadData];
}

+ (NSString*)destStringFromFacet:(int)srcFacet dest:(regina::NTetrahedron*)destTet gluing:(const regina::NPerm4&)gluing
{
    if (! destTet)
        return @" "; // Use a space to ensure the label has enough height to pick up touches.
    else
        return [NSString stringWithFormat:@"%ld (%s)",
                destTet->markedIndex(),
                (gluing * regina::NTriangle::ordering[srcFacet]).trunc3().c_str()];
}

- (IBAction)newSimplex:(id)sender {
    if (! self.packet->isPacketEditable())
        return;
    
    myEdit = YES;
    self.packet->newSimplex();
    myEdit = NO;
    
    // Update the necessary elements of the UI.
    NSIndexPath* last = [NSIndexPath indexPathForRow:self.packet->getNumberOfSimplices()
                                           inSection:0];
    NSIndexPath* add = [NSIndexPath indexPathForRow:self.packet->getNumberOfSimplices()+1
                                          inSection:0];
    [self.tetrahedra insertRowsAtIndexPaths:@[last]
                          withRowAnimation:UITableViewRowAnimationAutomatic];
    [self.tetrahedra scrollToRowAtIndexPath:add
                          atScrollPosition:UITableViewScrollPositionBottom animated:YES];
    
    [self.viewer updateHeader:self.header];
}

- (void)endEditing
{
    // As a consequence, this calls textViewDidEndEditing:,
    // which is where the real work is done.
    [editField resignFirstResponder];
}

- (void)editGluingForSimplex:(int)simplex cell:(TriGluingCell*)cell label:(UILabel*)label
{
    editLabel = label;
    editSimplex = simplex;
    editFacet = label.tag;
    
    editField = [[UITextField alloc] initWithFrame:label.frame];
    editField.backgroundColor = cell.backgroundColor;
    editField.borderStyle = UITextBorderStyleNone;
    editField.placeholder = (editFacet >= 0 ? @"Gluing" : @"Name");
    editField.clearButtonMode = UITextFieldViewModeAlways;
    editField.returnKeyType = UIReturnKeyDone;
    editField.autocorrectionType = UITextAutocorrectionTypeNo;
    if (editFacet >= 0) {
        regina::NTetrahedron* t = self.packet->getSimplex(editSimplex);
        editField.text = [TriGluings destStringFromFacet:editFacet
                                                    dest:t->adjacentSimplex(editFacet)
                                                  gluing:t->adjacentGluing(editFacet)];
        editField.autocapitalizationType = UITextAutocapitalizationTypeNone;
        editField.keyboardType = UIKeyboardTypeNumbersAndPunctuation;
        editField.textAlignment = NSTextAlignmentRight;
    } else {
        editField.text = @(self.packet->getSimplex(simplex)->getDescription().c_str());
        editField.autocapitalizationType = UITextAutocapitalizationTypeSentences;
        editField.keyboardType = UIKeyboardTypeDefault;
        editField.textAlignment = NSTextAlignmentLeft;
    }
    editField.delegate = self;
    
    [cell addSubview:editField];
    [editField becomeFirstResponder];
    [editField selectAll:nil];
}

- (IBAction)touched:(id)sender {
    // Finish and process any other edit that is currently in progress.
    if (editField) {
        [editField resignFirstResponder];
        editField = nil;
    }
    
    if (! self.packet->isPacketEditable())
        return;

    UITapGestureRecognizer *tap = static_cast<UITapGestureRecognizer*>(sender);
    if (tap.state != UIGestureRecognizerStateRecognized)
        return;
    
    CGPoint location = [tap locationInView:self.tetrahedra];
    NSIndexPath *indexPath = [self.tetrahedra indexPathForRowAtPoint:location];
    if (indexPath.row == 0 || indexPath.row > self.packet->getNumberOfSimplices())
        return;
    
    TriGluingCell* cell = static_cast<TriGluingCell*>([self.tetrahedra cellForRowAtIndexPath:indexPath]);
    CGPoint inner = [self.tetrahedra convertPoint:location toView:cell];
    if (CGRectContainsPoint(cell.index.frame, inner))
        [self editGluingForSimplex:indexPath.row-1 cell:cell label:cell.index];
    else if (CGRectContainsPoint(cell.face0.frame, inner))
        [self editGluingForSimplex:indexPath.row-1 cell:cell label:cell.face0];
    else if (CGRectContainsPoint(cell.face1.frame, inner))
        [self editGluingForSimplex:indexPath.row-1 cell:cell label:cell.face1];
    else if (CGRectContainsPoint(cell.face2.frame, inner))
        [self editGluingForSimplex:indexPath.row-1 cell:cell label:cell.face2];
    else if (CGRectContainsPoint(cell.face3.frame, inner))
        [self editGluingForSimplex:indexPath.row-1 cell:cell label:cell.face3];
}

#pragma mark - Text field

- (void)textFieldDidEndEditing:(UITextField *)textField
{
    if (editField != textField) {
        NSLog(@"Error: Mismatched text field when editing gluings.");
        return;
    }
    
    if (! self.packet->isPacketEditable()) {
        [editField removeFromSuperview];
        editField = nil;
        editLabel = nil;
        return;
    }
    
    regina::NTetrahedron* t = self.packet->getSimplex(editSimplex);
    
    NSMutableArray* toReload = [[NSMutableArray alloc] init];
    if (editFacet >= 0) {
        NSString* dest = [editField.text stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
        if (dest.length == 0) {
            // We are making this a boundary facet.
            if (t->adjacentSimplex(editFacet)) {
                myEdit = YES;
                [toReload addObject:[NSIndexPath indexPathForRow:t->adjacentSimplex(editFacet)->markedIndex()+1 inSection:0]];
                t->unjoin(editFacet);
                editLabel.text = @" ";
                [self.viewer updateHeader:self.header];
                myEdit = NO;
            }
        } else {
            NSRegularExpression* regex = [[NSRegularExpression alloc] initWithPattern:@"\\A(\\d+)[ ,\\(]+([0-3][0-3][0-3])[ \\)]*\\Z" options:0 error:nil];
            NSTextCheckingResult* result = [regex firstMatchInString:dest options:0 range:NSMakeRange(0, dest.length)];
            if (! result) {
                UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Invalid Gluing"
                                                                message:@"Please enter the gluing in the form tetrahedron (face).  For example, you could enter \"6 (130)\", or just \"6 130\"."
                                                               delegate:nil
                                                      cancelButtonTitle:@"Close"
                                                      otherButtonTitles:nil];
                [alert show];
                goto cleanUpGluing;
            }
            
            int destSimplex = [[dest substringWithRange:[result rangeAtIndex:1]] intValue];
            if (destSimplex < 0 || destSimplex >= self.packet->getNumberOfSimplices()) {
                UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Invalid Tetrahedron"
                                                                message:@"Please enter the gluing in the form tetrahedron (face).  For example, you could enter \"6 (130)\", or just \"6 130\"."
                                                               delegate:nil
                                                      cancelButtonTitle:@"Close"
                                                      otherButtonTitles:nil];
                [alert show];
                goto cleanUpGluing;
            }
            
            // We know at this point that we pass the regex, which means the adjacent facet
            // is in the form [0-3][0-3][0-3].
            int adjGluingAsInt = [dest substringWithRange:[result rangeAtIndex:2]].intValue;
            int adj0 = (adjGluingAsInt / 100);
            int adj1 = (adjGluingAsInt % 100) / 10;
            int adj2 = (adjGluingAsInt % 10);
            if (adj0 == adj1 || adj0 == adj2 || adj1 == adj2) {
                UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Invalid Face"
                                                                message:@"Please enter the gluing in the form tetrahedron (face).  For example, you could enter \"6 (130)\", or just \"6 130\"."
                                                               delegate:nil
                                                      cancelButtonTitle:@"Close"
                                                      otherButtonTitles:nil];
                [alert show];
                goto cleanUpGluing;
            }
            regina::NPerm4 destGluing = regina::NPerm4(adj0, adj1, adj2, (6 - adj0 - adj1 - adj2)) *
                regina::NTriangle::ordering[editFacet].inverse();
            
            // Are we gluing the facet to itself?
            if (destSimplex == editSimplex && destGluing[editFacet] == editFacet) {
                UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Cannot Glue a Face to Itself"
                                                                message:nil
                                                               delegate:nil
                                                      cancelButtonTitle:@"Close"
                                                      otherButtonTitles:nil];
                [alert show];
                goto cleanUpGluing;
            }
            
            if (t->adjacentSimplex(editFacet) != self.packet->getSimplex(destSimplex) || destGluing != t->adjacentGluing(editFacet)) {
                // We are making a change, and it's a valid one.
                // Do it.
                myEdit = YES;
                {
                    regina::NPacket::ChangeEventSpan span(self.packet);
                    
                    // First unglue from the old partner if it exists.
                    if (t->adjacentSimplex(editFacet)) {
                        [toReload addObject:[NSIndexPath indexPathForRow:t->adjacentSimplex(editFacet)->markedIndex()+1 inSection:0]];
                        t->unjoin(editFacet);
                    }
                    
                    // We are gluing the facet to a new partner.
                    int destFacet = destGluing[editFacet];
                    
                    // Does this new partner already have its own partner?
                    // If so, better unglue it.
                    regina::NTetrahedron* adj = self.packet->getSimplex(destSimplex);
                    if (adj->adjacentSimplex(destFacet)) {
                        NSIndexPath* path = [NSIndexPath indexPathForRow:adj->adjacentSimplex(destFacet)->markedIndex()+1 inSection:0];
                        if ([toReload indexOfObject:path] == NSNotFound)
                            [toReload addObject:path];
                        adj->unjoin(destFacet);
                    }
                    
                    // Glue the two facets together.
                    t->joinTo(editFacet, adj, destGluing);
                    NSIndexPath* path = [NSIndexPath indexPathForRow:destSimplex+1 inSection:0];
                    if ([toReload indexOfObject:path] == NSNotFound)
                        [toReload addObject:path];
                    
                    editLabel.text = [NSString stringWithFormat:@"%d (%d%d%d)", destSimplex, adj0, adj1, adj2];
                }
                [self.viewer updateHeader:self.header];
                myEdit = NO;
            }
        }
    } else {
        myEdit = YES;
        NSString* desc = [editField.text stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
        t->setDescription(desc.UTF8String);
        editLabel.text = [NSString stringWithFormat:@"%d. %@", editSimplex, desc];
        myEdit = NO;
    }
    
cleanUpGluing:
    [editField removeFromSuperview];
    editField = nil;
    editLabel = nil;
    
    if (toReload.count)
        [self.tetrahedra reloadRowsAtIndexPaths:toReload withRowAnimation:NO];
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    [textField resignFirstResponder];
    return NO;
}

#pragma mark - Table view

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return (self.packet->isPacketEditable() ? 2 : 1) + self.packet->getNumberOfSimplices();
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.row == 0)
        return [tableView dequeueReusableCellWithIdentifier:@"Header"];
    else if (indexPath.row == self.packet->getNumberOfSimplices() + 1)
        return [tableView dequeueReusableCellWithIdentifier:@"Add"];
    
    TriGluingCell *cell = [tableView dequeueReusableCellWithIdentifier:@"Tetrahedron" forIndexPath:indexPath];
    regina::NTetrahedron* t = self.packet->getSimplex(indexPath.row - 1);
    cell.index.text = [NSString stringWithFormat:@"%d. %s", indexPath.row - 1, t->getDescription().c_str()];
    cell.face0.text = [TriGluings destStringFromFacet:0 dest:t->adjacentSimplex(0) gluing:t->adjacentGluing(0)];
    cell.face1.text = [TriGluings destStringFromFacet:1 dest:t->adjacentSimplex(1) gluing:t->adjacentGluing(1)];
    cell.face2.text = [TriGluings destStringFromFacet:2 dest:t->adjacentSimplex(2) gluing:t->adjacentGluing(2)];
    cell.face3.text = [TriGluings destStringFromFacet:3 dest:t->adjacentSimplex(3) gluing:t->adjacentGluing(3)];
    return cell;
}

- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (self.packet->isPacketEditable())
        return (indexPath.row > 0 && indexPath.row <= self.packet->getNumberOfSimplices());
    else
        return NO;
}

- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.row == 0 || indexPath.row > self.packet->getNumberOfSimplices())
        return;
    
    // Many rows could change - not only do we blank out gluings for adjacent tetrahedra,
    // but we also need to reindex every tetrahedron after the one that was removed.
    // Just reload everything.
    // This is easy: we don't set myEdit=YES, and instead rely on the automatic packet reload.
    self.packet->removeSimplexAt(indexPath.row - 1);
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.row > 0)
        return self.tetrahedra.rowHeight;
    
    // The header row is smaller.  Calculate it based on the cell contents, which include
    // auto-layout constraints that pin the labels to the upper and lower boundaries.
    if (headerHeight == 0) {
        UITableViewCell* cell = [self.tetrahedra dequeueReusableCellWithIdentifier:@"Header"];
        [cell layoutIfNeeded];
        CGSize size = [cell.contentView systemLayoutSizeFittingSize:UILayoutFittingCompressedSize];
        headerHeight = size.height;
    }
    return headerHeight;
}

@end
