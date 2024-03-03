//
// Created by SeanR on 3/3/2024.
//

#ifndef THREE_CPP_IMPROVEDNOISE_H
#define THREE_CPP_IMPROVEDNOISE_H

#include "math_utils.h"

// https://cs.nyu.edu/~perlin/noise/
class ImprovedNoise {
public:

    float noise( float x, float y, float z );


private:

    float fade( float t ) {
        return t * t * t * ( t * ( t * 6 - 15 ) + 10 );
    }

    float lerp( float t, float a, float b ) {
        return a + t * ( b - a );
    }

    int grad( int hash, int x, int y, int z ) {
        const int h = hash & 15;
        const int u = h < 8 ? x : y, v = h < 4 ? y : h == 12 || h == 14 ? x : z;
        return ( ( h & 1 ) == 0 ? u : - u ) + ( ( h & 2 ) == 0 ? v : - v );
    }


};


#endif //THREE_CPP_IMPROVEDNOISE_H