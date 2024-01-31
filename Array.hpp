#include <cstddef>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <initializer_list>
#include <memory>

template<class T, size_t N>
class Array
{
using value_type                    = T;
using size_type                     = size_t;
using difference_type               = ptrdiff_t;
using reference                     = T&;
using const_reference               = const T&;
using pointer                       = T*;
using const_pointer                 = const T*;
using iterator                      = T*;
using const_iterator                = const T*;
using reverse_iterator              = std::reverse_iterator<T *>;
using const_reverse_iterator        = std::reverse_iterator<T const *>;

private:
    T m_data[N];
public:
    T& at(size_t i){
        if(i>=N) [[unlikely]] throw std::out_of_range("Array::at");
        return m_data[i];
    }

    const T& at(size_t i) const{
        if(i>=N) [[unlikely]] throw std::out_of_range("Array::at");
        return m_data[i];
    }

    T& operator[](size_t i){
        return m_data[i];
    }

    const T& operator[](size_t i) const{
        return m_data[i];
    }

    T& front(size_t i){
        return m_data[0];
    }

    const T& front(size_t i) const{
        return m_data[0]; 
    }

    T& back(size_t i){
        return m_data[N-1];
    }

    const T& back(size_t i) const{
        return m_data[N-1];
    }

    T* data(){
        return m_data;
    }

    const T* data() const{
        return m_data;
    }

    iterator begin(){
        return m_data;
    }

    const_iterator begin() const{
        return m_data;
    }

    const_iterator cbegin() const{
        return m_data;
    }

    iterator end(){
        return m_data + N;
    }

    const_iterator end() const{
        return m_data + N;
    }

    const_iterator cend() const{
        return m_data + N;
    }

    iterator rbegin(){
        return m_data - 1;
    }

    const_iterator rbegin() const{
        return m_data - 1;
    }

    const_iterator crbegin() const{
        return m_data - 1;
    }

    iterator rend(){
        return m_data + N - 1;
    }

    const_iterator rend() const{
        return m_data + N - 1;
    }

    const_iterator crend() const{
        return m_data + N - 1;
    }

    bool empty() const{
        return N == 0;
    }

    size_t size() const{
        return N;
    }
    
    size_t max_size() const{
        return N;
    }

    void fill(const T& value){
        for(size_t i = 0 ; i < N; i++){
            m_data[i] = value;
        }
    }

    void swap(Array<T,N>& other){
        for(size_t i = 0 ; i < N ; i++){
            std::swap(other.m_data[i],m_data[i]);
        }
    }
};

template<class T, class... Ts>
Array(T,Ts...)->Array<T,1 + sizeof...(Ts)>;  
