
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

#import "AngleViewController.h"
#import "MDSpreadViewClasses.h"
#import "PacketTreeController.h"
#import "ReginaHelper.h"
#import "angle/nanglestructure.h"
#import "angle/nanglestructurelist.h"
#import "triangulation/ntriangulation.h"

static UIColor* yesColour = [UIColor colorWithRed:0.0 green:0.5 blue:0.0 alpha:1.0];
static UIColor* noColour = [UIColor colorWithRed:0.5 green:0.0 blue:0.0 alpha:1.0];

// These fonts are hard-coded into the MDSpreadView classes.
// Replicate them here so we can compute cell sizes.
static UIFont* headerFont = [UIFont boldSystemFontOfSize:14];
static UIFont* cellFont = [UIFont systemFontOfSize:16];

// MDSpreadViewHeaderCell.m uses a horizontal padding of 28, assuming no sort indicator.
// Both the header and cell classes use a vertical padding of 3.
#define CELL_WIDTH_PADDING 32
#define CELL_HEIGHT_PADDING 6

#pragma mark - Angle structure view controller

@interface AngleViewController () <MDSpreadViewDataSource, MDSpreadViewDelegate, PacketDelegate> {
    PacketListenerIOS* _triListener;
    CGFloat widthHeader;
    CGFloat widthType;
    CGFloat widthAngle;
    CGFloat height;
}
@property (weak, nonatomic) IBOutlet UILabel *countAndType;
@property (weak, nonatomic) IBOutlet UILabel *span;
@property (weak, nonatomic) IBOutlet UIButton *triangulation;
@property (weak, nonatomic) IBOutlet MDSpreadView *angles;
@end

@implementation AngleViewController

- (void)viewDidLoad
{
    [super viewDidLoad];

    regina::NAngleStructureList* a = static_cast<regina::NAngleStructureList*>(self.packet);

    unsigned long count = a->getNumberOfStructures();
    if (a->isTautOnly()) {
        if (count == 0)
            self.countAndType.text = @"No taut angle structures";
        else if (count == 1)
            self.countAndType.text = @"1 taut angle structure";
        else
            self.countAndType.text = [NSString stringWithFormat:@"%ld taut angle structures", count];
    } else {
        if (count == 0)
            self.countAndType.text = @"No vertex angle structures";
        else if (count == 1)
            self.countAndType.text = @"1 vertex angle structure";
        else
            self.countAndType.text = [NSString stringWithFormat:@"%ld vertex angle structures", count];
    }

    NSAttributedString* strictText = (a->spansStrict() ?
                                      [[NSAttributedString alloc]
                                       initWithString:@"Strict"
                                       attributes:@{NSForegroundColorAttributeName: yesColour}] :
                                      [[NSAttributedString alloc]
                                       initWithString:@"No strict"
                                       attributes:@{NSForegroundColorAttributeName: noColour}]);
    NSAttributedString* tautText = (a->spansTaut() ?
                                    [[NSAttributedString alloc]
                                     initWithString:@"Taut"
                                     attributes:@{NSForegroundColorAttributeName: yesColour}] :
                                    [[NSAttributedString alloc]
                                     initWithString:@"No taut"
                                     attributes:@{NSForegroundColorAttributeName: noColour}]);

    NSMutableAttributedString* spanText = [[NSMutableAttributedString alloc] initWithString:@"Span includes: "];
    [spanText appendAttributedString:strictText];
    [spanText appendAttributedString:[[NSAttributedString alloc] initWithString:@", "]];
    [spanText appendAttributedString:tautText];
    self.span.attributedText = spanText;

    [self updateTriangulationButton];
    // Continue to update the button text if the triangulation is renamed.
    _triListener = [PacketListenerIOS listenerWithPacket:a->getTriangulation() delegate:self listenChildren:NO];

    [self initMetrics];
    self.angles.dataSource = self;
    self.angles.delegate = self;
    self.angles.allowsRowHeaderSelection = YES;
}

- (void)initMetrics
{
    regina::NAngleStructureList* a = static_cast<regina::NAngleStructureList*>(self.packet);
    NSString* text;

    text = [NSString stringWithFormat:@"%ld.", a->getNumberOfStructures() - 1];
    widthHeader = CELL_WIDTH_PADDING + [text sizeWithAttributes:@{NSFontAttributeName: headerFont}].width;

    text = @"Veering";
    CGSize size = [text sizeWithAttributes:@{NSFontAttributeName: cellFont}];
    widthType = CELL_WIDTH_PADDING + size.width;
    height = CELL_HEIGHT_PADDING + size.height;

    text = [NSString stringWithFormat:@"%ld: 01/23", a->getTriangulation()->getNumberOfTetrahedra() - 1];
    widthAngle = CELL_WIDTH_PADDING + [text sizeWithAttributes:@{NSFontAttributeName: headerFont}].width;
}

- (void)dealloc
{
    [_triListener permanentlyUnlisten];
}

- (IBAction)openTriangulation:(id)sender {
    regina::NPacket* show = static_cast<regina::NAngleStructureList*>(self.packet)->getTriangulation();
    [[ReginaHelper tree] navigateToPacket:show->getTreeParent()];

    // We can't select this normal surface list in the tree, since the pop action is animated and
    // will not have finished by this point.
    [ReginaHelper viewPacket:static_cast<regina::NAngleStructureList*>(self.packet)->getTriangulation()];
}

- (void)updateTriangulationButton
{
    regina::NPacket* tri = static_cast<regina::NAngleStructureList*>(self.packet)->getTriangulation();
    NSString* triName = [NSString stringWithUTF8String:tri->getPacketLabel().c_str()];
    if (triName.length == 0)
        triName = @"(Unnamed)";
    [self.triangulation setTitle:triName forState:UIControlStateNormal];
}

#pragma mark - Packet listener

- (void)packetWasRenamed:(regina::NPacket *)packet
{
    if (packet == static_cast<regina::NAngleStructureList*>(self.packet)->getTriangulation())
        [self updateTriangulationButton];
}

#pragma mark - MDSpreadView data source

- (NSInteger)spreadView:(MDSpreadView *)aSpreadView numberOfColumnsInSection:(NSInteger)section
{
    return 1 + 3 * static_cast<regina::NAngleStructureList*>(self.packet)->getTriangulation()->getNumberOfTetrahedra();
}

- (NSInteger)spreadView:(MDSpreadView *)aSpreadView numberOfRowsInSection:(NSInteger)section
{
    return static_cast<regina::NAngleStructureList*>(self.packet)->getNumberOfStructures();
}

- (id)spreadView:(MDSpreadView *)aSpreadView titleForHeaderInRowSection:(NSInteger)section forColumnAtIndexPath:(MDIndexPath *)columnPath
{
    if (columnPath.column == 0)
        return @"Type";
    else {
        NSInteger tet = (columnPath.column - 1) / 3;
        NSInteger type = (columnPath.column - 1) % 3;
        switch (type) {
            case 0: return [NSString stringWithFormat:@"%d: 01/23", tet];
            case 1: return [NSString stringWithFormat:@"%d: 02/13", tet];
            case 2: return [NSString stringWithFormat:@"%d: 03/12", tet];
            default: return nil;
        }
    }
}

- (id)spreadView:(MDSpreadView *)aSpreadView titleForHeaderInColumnSection:(NSInteger)section forRowAtIndexPath:(MDIndexPath *)rowPath
{
    return [NSString stringWithFormat:@"%d.", rowPath.row];
}

- (id)spreadView:(MDSpreadView *)aSpreadView objectValueForRowAtIndexPath:(MDIndexPath *)rowPath forColumnAtIndexPath:(MDIndexPath *)columnPath
{
    const regina::NAngleStructure* a = static_cast<regina::NAngleStructureList*>(self.packet)->getStructure(rowPath.row);

    if (columnPath.column == 0) {
        if (a->isStrict())
            return @"Strict";
        else if (a->isVeering())
            return @"Veering";
        else if (a->isTaut())
            return @"Taut";
        else
            return @"";
    }

    regina::NRational angle = a->getAngle((columnPath.column - 1) / 3, (columnPath.column - 1) % 3);
    if (angle == 0)
        return @"";
    if (angle == 1)
        return @"π";
    if (angle.getDenominator() == 1)
        return [NSString stringWithFormat:@"%s π",
                angle.getNumerator().stringValue().c_str()];
    if (angle.getNumerator() == 1)
        return [NSString stringWithFormat:@"π / %s",
                angle.getDenominator().stringValue().c_str()];
    return [NSString stringWithFormat:@"%s π / %s",
            angle.getNumerator().stringValue().c_str(),
            angle.getDenominator().stringValue().c_str()];
}

#pragma mark - MDSpreadView delegate

- (CGFloat)spreadView:(MDSpreadView *)aSpreadView widthForColumnHeaderInSection:(NSInteger)columnSection
{
    return widthHeader;
}

- (CGFloat)spreadView:(MDSpreadView *)aSpreadView widthForColumnAtIndexPath:(MDIndexPath *)indexPath
{
    switch (indexPath.column) {
        case 0: return widthType;
        default: return widthAngle;
    }
}

- (CGFloat)spreadView:(MDSpreadView *)aSpreadView heightForRowAtIndexPath:(MDIndexPath *)indexPath
{
    return height;
}

@end
