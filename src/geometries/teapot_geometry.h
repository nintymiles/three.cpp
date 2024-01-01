//
// Created by SeanR on 2024/1/1.
//

#ifndef THREE_CPP_TEAPOT_GEOMETRY_H
#define THREE_CPP_TEAPOT_GEOMETRY_H

#include "buffer_geometry.h"

/**
* Tessellates the famous Utah teapot database by Martin Newell into triangles.
*
* Parameters: size = 50, segments = 10, bottom = true, lid = true, body = true,
*   fitLid = false, blinn = true
*
* size is a relative scale: I've scaled the teapot to fit vertically between -1 and 1.
* Think of it as a "radius".
* segments - number of line segments to subdivide each patch edge;
*   1 is possible but gives degenerates, so two is the real minimum.
* bottom - boolean, if true (default) then the bottom patches are added. Some consider
*   adding the bottom heresy, so set this to "false" to adhere to the One True Way.
* lid - to remove the lid and look inside, set to true.
* body - to remove the body and leave the lid, set this and "bottom" to false.
* fitLid - the lid is a tad small in the original. This stretches it a bit so you can't
*   see the teapot's insides through the gap.
* blinn - Jim Blinn scaled the original data vertically by dividing by about 1.3 to look
*   nicer. If you want to see the original teapot, similar to the real-world model, set
*   this to false. True by default.
*   See http://en.wikipedia.org/wiki/File:Original_Utah_Teapot.jpg for the original
*   real-world teapot (from http://en.wikipedia.org/wiki/Utah_teapot).
*
* Note that the bottom (the last four patches) is not flat - blame Frank Crow, not me.
*
* The teapot should normally be rendered as a double sided object, since for some
* patches both sides can be seen, e.g., the gap around the lid and inside the spout.
*
* Segments 'n' determines the number of triangles output.
*   Total triangles = 32*2*n*n - 8*n    [degenerates at the top and bottom cusps are deleted]
*
*   size_factor   # triangles
*       1          56
*       2         240
*       3         552
*       4         992
*
*      10        6320
*      20       25440
*      30       57360
*
* Code converted from my ancient SPD software, http://tog.acm.org/resources/SPD/
* Created for the Udacity course "Interactive Rendering", http://bit.ly/ericity
* YouTube video on teapot history: https://www.youtube.com/watch?v=DxMfblPzFNc
*
* See https://en.wikipedia.org/wiki/Utah_teapot for the history of the teapot
*
*/
class TeapotGeometry : public BufferGeometry {
public:
    using sptr = std::shared_ptr<TeapotGeometry>;

    TeapotGeometry(int size = 50, int segments = 10, bool bottom = true, bool lid = true, bool body = true, bool fitLid = true, bool blinn = true);

    static sptr create(int size = 50, int segments = 10, bool bottom = true, bool lid = true, bool body = true, bool fitLid = true, bool blinn = true){
        return std::make_shared<TeapotGeometry>(size,segments,bottom,lid,body,fitLid,blinn);
    }
};

#endif //THREE_CPP_TEAPOT_GEOMETRY_H
