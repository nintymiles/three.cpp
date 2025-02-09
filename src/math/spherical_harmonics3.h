//
// Created by SeanR on 2023/6/28.
//

//spherical harmonics 球函数
#ifndef THREE_CPP_SPHERICAL_HARMONICS3_H
#define THREE_CPP_SPHERICAL_HARMONICS3_H

#include "vector3.h"

class SphericalHarmonics3 {
public:
    std::vector<Vector3> coefficients;

    SphericalHarmonics3() {}

    SphericalHarmonics3(std::vector<Vector3>& coefficients) :coefficients(coefficients) {};

    virtual ~SphericalHarmonics3() = default;

    SphericalHarmonics3& set(const std::vector<Vector3>& coefficients) {
        return *this;
    }

    SphericalHarmonics3& zero() {
        return *this;
    }

    SphericalHarmonics3& add(const SphericalHarmonics3& sh)
    {
        return *this;
    }

    SphericalHarmonics3& scale(float s) {
        return *this;
    }

    SphericalHarmonics3& lerp(const SphericalHarmonics3& sh, float alpha) {
        return *this;
    }

    bool equals(const SphericalHarmonics3& sh) {
        return false;
    }

    SphericalHarmonics3& copy(const SphericalHarmonics3& sh) {
        return *this;
    }

    SphericalHarmonics3* clone() {
        return nullptr;
    }

    /**
     * Sets the values of this spherical harmonics from the provided array.
     * @param array the source array.
     * @param offset (optional) offset into the array. Default is 0.
     */
    SphericalHarmonics3& fromArray(float* array, unsigned offset = 0) {
        return *this;
    }




    /**
     * Returns an array with the values of this spherical harmonics, or copies them into the provided array.
     * @param array (optional) array to store the spherical harmonics to. If this is not provided, a new array will be created.
     * @param offset (optional) optional offset into the array.
     * @return The created or provided array.
     */
    void toArray(float* array, unsigned offset = 0) {}



    Vector3 getAt(const Vector3& normal, const Vector3& target) {
        return Vector3();
    }

    Vector3 getIrradianceAt(const Vector3& normal, const Vector3& target) {
        return Vector3();
    }

    static void getBasisAt(const Vector3& normal, float* shBasis) {}


};

//#include <vector>
//#include <array>
//
//#include "vector3.h"
//
///**
// * Primary reference:
// *   https://graphics.stanford.edu/papers/envmap/envmap.pdf
// *
// * Secondary reference:
// *   https://www.ppsloan.org/publications/StupidSH36.pdf
// */
//
//// 3-band SH defined by 9 coefficients
//class SphericalHarmonics3 {
//public:
//    SphericalHarmonics3(){
//        for(int i=0;i<9;i++){
//            coefficients.push_back(std::make_shared<Vector3>());
//        }
//    }
//
//    SphericalHarmonics3& set(std::vector<Vector3Sptr>& efficients ) {
//        for ( int i = 0; i < 9; i ++ ) {
//            this->coefficients[ i ]->copy(*efficients[ i ]) ;
//        }
//
//        return *this;
//    }
//
//    SphericalHarmonics3& zero() {
//        for ( int i = 0; i < 9; i ++ ) {
//            coefficients[ i ]->set( 0, 0, 0 );
//        }
//
//        return *this;
//    }
//
//    // get the radiance in the direction of the normal
//    // target is a Vector3
//    Vector3& getAt( Vector3& normal, Vector3& target ) {
//        // normal is assumed to be unit length
//        double x = normal.x, y = normal.y, z = normal.z;
//
//        std::vector<Vector3Sptr> coeff = coefficients;
//
//        // band 0
//        target.copy( *coeff[0] ).multiplyScalar( 0.282095 );
//
//        // band 1
//        target.addScaledVector( *coeff[ 1 ], 0.488603 * y );
//        target.addScaledVector( *coeff[ 2 ], 0.488603 * z );
//        target.addScaledVector( *coeff[ 3 ], 0.488603 * x );
//
//        // band 2
//        target.addScaledVector( *coeff[ 4 ], 1.092548 * ( x * y ) );
//        target.addScaledVector( *coeff[ 5 ], 1.092548 * ( y * z ) );
//        target.addScaledVector( *coeff[ 6 ], 0.315392 * ( 3.0 * z * z - 1.0 ) );
//        target.addScaledVector( *coeff[ 7 ], 1.092548 * ( x * z ) );
//        target.addScaledVector( *coeff[ 8 ], 0.546274 * ( x * x - y * y ) );
//
//        return target;
//    }
//
//    // get the irradiance (radiance convolved with cosine lobe) in the direction of the normal
//    // target is a Vector3
//    // https://graphics.stanford.edu/papers/envmap/envmap.pdf
//    Vector3& getIrradianceAt( const Vector3& normal, Vector3& target ) {
//        // normal is assumed to be unit length
//        double x = normal.x, y = normal.y, z = normal.z;
//
//        std::vector<Vector3Sptr> coeff = coefficients;
//
//        // band 0
//        target.copy( *coeff[ 0 ] ).multiplyScalar( 0.886227 ); // π * 0.282095
//
//        // band 1
//        target.addScaledVector( *coeff[ 1 ], 2.0 * 0.511664 * y ); // ( 2 * π / 3 ) * 0.488603
//        target.addScaledVector( *coeff[ 2 ], 2.0 * 0.511664 * z );
//        target.addScaledVector( *coeff[ 3 ], 2.0 * 0.511664 * x );
//
//        // band 2
//        target.addScaledVector( *coeff[ 4 ], 2.0 * 0.429043 * x * y ); // ( π / 4 ) * 1.092548
//        target.addScaledVector( *coeff[ 5 ], 2.0 * 0.429043 * y * z );
//        target.addScaledVector( *coeff[ 6 ], 0.743125 * z * z - 0.247708 ); // ( π / 4 ) * 0.315392 * 3
//        target.addScaledVector( *coeff[ 7 ], 2.0 * 0.429043 * x * z );
//        target.addScaledVector( *coeff[ 8 ], 0.429043 * ( x * x - y * y ) ); // ( π / 4 ) * 0.546274
//
//        return target;
//    }
//
//    SphericalHarmonics3& add( SphericalHarmonics3& sh ) {
//        for ( int i = 0; i < 9; i ++ ) {
//            this->coefficients[ i ]->add( *sh.coefficients[ i ] );
//        }
//
//        return *this;
//    }
//
//    SphericalHarmonics3& addScaledSH( SphericalHarmonics3& sh, double s ) {
//        for ( int i = 0; i < 9; i ++ ) {
//            this->coefficients[ i ]->addScaledVector( *sh.coefficients[ i ], s );
//        }
//
//        return *this;
//    }
//
//    SphericalHarmonics3& scale( double s ) {
//        for ( int i = 0; i < 9; i ++ ) {
//            this->coefficients[ i ]->multiplyScalar( s );
//        }
//
//        return *this;
//    }
//
//    SphericalHarmonics3& lerp( SphericalHarmonics3& sh, double alpha ) {
//        for ( int i = 0; i < 9; i ++ ) {
//            this->coefficients[ i ]->lerp( *sh.coefficients[ i ], alpha );
//        }
//
//        return *this;
//    }
//
//    bool equals( SphericalHarmonics3& sh ) {
//        for ( int i = 0; i < 9; i ++ ) {
//            if ( ! this->coefficients[ i ]->equals( *sh.coefficients[ i ] ) ) {
//                return false;
//            }
//        }
//
//        return true;
//    }
//
//    SphericalHarmonics3& copy( SphericalHarmonics3& sh ) {
//        return set( sh.coefficients );
//    }
//
//    SphericalHarmonics3 clone() {
//        return SphericalHarmonics3().copy( *this );
//    }
//
//
//    // evaluate the basis functions
//    // shBasis is an Array[ 9 ]
//    static void getBasisAt( Vector3& normal, std::array<double,9>& shBasis ) {
//        // normal is assumed to be unit length
//        double x = normal.x, y = normal.y, z = normal.z;
//
//        // band 0
//        shBasis[ 0 ] = 0.282095;
//
//        // band 1
//        shBasis[ 1 ] = 0.488603 * y;
//        shBasis[ 2 ] = 0.488603 * z;
//        shBasis[ 3 ] = 0.488603 * x;
//
//        // band 2
//        shBasis[ 4 ] = 1.092548 * x * y;
//        shBasis[ 5 ] = 1.092548 * y * z;
//        shBasis[ 6 ] = 0.315392 * ( 3 * z * z - 1 );
//        shBasis[ 7 ] = 1.092548 * x * z;
//        shBasis[ 8 ] = 0.546274 * ( x * x - y * y );
//    }
//
//    SphericalHarmonics3& fromArray( double array[], int offset = 0 ) {
//        for ( int i = 0; i < 9; i ++ ) {
//            coefficients[ i ]->fromArray( array, offset + ( i * 3 ) );
//        }
//
//        return *this;
//    }
//
//    double* toArray( double array[], int offset = 0 ) {
//        for ( int i = 0; i < 9; i ++ ) {
//            coefficients[ i ]->toArray( array, offset + ( i * 3 ) );
//        }
//
//        return array;
//    }
//
//
//public:
//    bool isSphericalHarmonics3 = true;
//    std::vector<Vector3Sptr> coefficients{};
//};
//
//using SphericalHarmonics3Sptr = std::shared_ptr<SphericalHarmonics3>;

#endif //THREE_CPP_SPHERICAL_HARMONICS3_H
