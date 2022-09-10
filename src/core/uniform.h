#ifndef SRC_CORE_UNIFORM_H_
#define SRC_CORE_UNIFORM_H_

template <typename T>
class Uniform {
    public:
        T value;

        Uniform(T v):value(v){}

        Uniform(const Uniform& u):value(u.value){}

        Uniform clone(){
            return Uniform(*this);
        }

}

#endif /* SRC_CORE_UNIFORM_H_ */
