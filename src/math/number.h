#include<limits>
struct Number{
    //when for const error: in-class initializer for static data member of type 'const double' requires 'constexpr' specifier [-Wstatic-float-init]
    static constexpr double EPSILON = std::numeric_limits<double>::epsilon();
};
