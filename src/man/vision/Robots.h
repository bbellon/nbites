// This file is part of Man, a robotic perception, locomotion, and
// team strategy application created by the Northern Bites RoboCup
// team of Bowdoin College in Brunswick, Maine, for the Aldebaran
// Nao robot.
//
// Man is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Man is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser Public License for more details.
//
// You should have received a copy of the GNU General Public License
// and the GNU Lesser Public License along with Man.  If not, see
// <http://www.gnu.org/licenses/>.


#ifndef Robots_h_DEFINED
#define Robots_h_DEFINED

#include <stdlib.h>

class Robots; // forward reference
#include "Threshold.h"
#include "Context.h"
#include "VisionStructs.h"
#include "VisualLine.h"
#include "Blob.h"
#include "Blobs.h"
#include "Field.h"
#include "Cross.h"


class Robots {
public:
    Robots(Vision* vis, Threshold* thr, Field* fie, Context* con, int c);
    virtual ~Robots() {}

    // Helper method that just returns whether the thresholded color is a
    // white color
    static inline const bool isWhite(unsigned char threshColor)
        {
			return threshColor & WHITE_BIT;
        }


	void init();
	void preprocess();
	void robot(Cross *cross);
	void expandRobotBlob(int which);
	bool noWhite(Blob b);
	void expandHorizontally(int which, int dir);
	int expandVertically(int which, int dir);
	bool goodScan(int c, int w, int o, int g, int gr, int t);
	void updateRobots(int w, int i);
	void mergeBigBlobs();
    void checkMerge(int i, int j);
	bool closeEnough(int i, int j);
    bool sanityChecks(Blob candidate, Cross* cross);
	bool bigEnough(Blob a, Blob b);
	bool noGreen(Blob a, Blob b);
    bool whiteBelow(Blob a);
    bool whiteAbove(Blob b);
	bool checkHorizontal(int l, int r, int t, int b);
	bool checkVertical(int l, int r, int t, int b);
	bool viableRobot(Blob a);
	void createObject();
	void newRun(int x, int y, int h);
	void setColor(int c);
	void allocateColorRuns();
	int distance(int x, int x1, int x2, int x3);
	void printBlob(Blob a);

    // Helper method that just returns whether the thresholded color is a
    // green color
    static inline const bool isGreen(unsigned char threshColor)
        {
			return threshColor & GREEN_BIT;
        }
    static inline const bool isSameColor(unsigned char threshColor, int col)
        {
            if (col == RED) {
                return threshColor & RED_BIT;
            } else {
                return threshColor & NAVY_BIT;
            }
        }
#ifdef OFFLINE
    void setDebugRobots(bool debug) {debugRobots = debug;}
#endif


private:
    // class pointers
    Vision* vision;
    Threshold* thresh;
	Field* field;
    Context* context;

	Blobs* blobs;
	int numberOfRuns, runsize;
	int color;
	Blob* topBlob;
	run* runs;
#ifdef OFFLINE
    bool debugRobots;
#else
    static const bool debugRobots = false;
#endif
};
#endif
