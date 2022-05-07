#pragma once
#include <vector>

template<class T> class SignedIndexArray final {
public:
    SignedIndexArray();
    SignedIndexArray(size_t elements);
    size_t Size() const;

    void Append(const T t);
    void Reserve(const size_t elements);

    T& operator[](int index) const;
    T& operator[](int index);
private:
    size_t elements;
    std::vector<T> vector;
};

template<class T> 
SignedIndexArray<T>::SignedIndexArray() {
    vector = std::vector<T>();
}

template<class T>
SignedIndexArray<T>::SignedIndexArray(size_t elements) {
    vector = std::vector<T>(elements);
}

template<class T>
size_t SignedIndexArray<T>::Size() const {
    return elements;
}

template<class T>
void SignedIndexArray<T>::Append(const T t) {
    vector.push_back(t);
}

template<class T>
void SignedIndexArray<T>::Reserve(const size_t elements) {
    vector.reserve(elements);
}

template <class T>
T& SignedIndexArray<T>::operator[](int index) const {
    index *= 2;
    if (index < 0)
        index = -(index + 1);
    return vector[index];
}

template <class T>
T& SignedIndexArray<T>::operator[](int index) {
    index *= 2;
    if (index < 0)
        index = -(index + 1);
    return vector[index];
}