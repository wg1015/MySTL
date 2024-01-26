#include <cstddef>
#include <cstring>
#include <limits>
#include <initializer_list>
#include <algorithm>
#include <stdexcept>
#include <iterator>
#include <memory>


//TODO
//bug1:该Vector在接受没有无参初始化器的地方没有办法使用，问题出现在resize中使用了无参初始化

template<class T,class allocator = std::allocator<T>>
class Vector{
using value_type = T;
using allocator_type = allocator;
using size_type = size_t;
using difference_type = ptrdiff_t;
using reference = T &;
using const_reference =T const &;
using pointer = T*;
using const_pointer = T const *;
using iterator = T*;
using const_iterator = T const *;
using reverse_iterator = std::reverse_iterator<T *>;
using const_reverse_iterator = std::reverse_iterator<T const *>;


private:
    T* m_data       = nullptr;
    size_t m_size   = 0;
    size_t m_capa   = 0;
    [[no_unique_address]] allocator m_alloc;
public:
    Vector(const allocator& alloc = allocator()) noexcept:m_alloc(alloc) {
        m_data = nullptr;
        m_size = 0;
        m_capa = 0;
    }

    explicit Vector(size_t n,const T& value,const allocator& alloc = allocator()):m_alloc(alloc){
        resize(n);
        for(size_t i = 0; i < n; i++){
            ::new(static_cast<void*>(&m_data[i])) T(value);
        }
    }

    explicit Vector(size_t n,const allocator& alloc = allocator()):m_alloc(alloc){
        resize(n);
    }
    
    Vector(const_iterator first, const_iterator last,const allocator& alloc = allocator()):m_alloc(alloc){
        size_t n = last - first;
        resize(n);
        for(size_t i=0; i<n; i++){
            ::new(static_cast<void*>(&m_data[i])) T(*first);
            first++;
        }
    }

    Vector(std::initializer_list<T>&& ilist,const allocator& alloc = allocator()):Vector(ilist.begin(),ilist.end(),alloc){
    }

    Vector(const Vector& other){
        if(other.m_data == nullptr){
            this->m_data = nullptr;
            this->m_size = 0;
            this->m_capa = 0;
        }
        
        this->resize(m_size);
        this->m_size = other.m_size;
        this->m_capa = other.m_capa;
        this->m_alloc = other.m_alloc;
        for(size_t i = 0; i < this->m_size ; i++){
            this->m_data[i] = other.m_data[i];
        }
    }

    Vector& operator=(const Vector& other){  
        if(&other == this)[[unlikely]] return *this;
        return Vector(other);
    }
    
    Vector(Vector&& other) noexcept:m_alloc(std::move(other.m_alloc)){      
        m_data = other.m_data;
        m_size = other.m_size;
        m_capa = other.m_capa;
        other.m_data = nullptr;
        other.m_sisze = 0;
        other.m_capa = 0;
    }

    Vector& operator=(Vector&& other) noexcept{
        if(&other == this) return *this; //右值取引用是什么？
        return Vector(other);
    }

    void assign(size_t count, const T& value){
        resize(count);
        for(size_t i = 0; i <count; i++){
            printf("%d\n",1);
            ::new(static_cast<void*>(&m_data[i])) T(value);
        }
    }

    allocator get_allocator() const noexcept{
        return m_alloc;
    }

    void resize(size_t n){
        if(n < m_size){
            for(size_t i = m_size; i!=n;i--){
                std::destroy_at(&m_data[i-1]);
            }
        }
        
        if(n>m_size){
            reserve(n);
            for(size_t i = m_size; i!=n; i++){
                ::new(static_cast<void*>(&m_data[i])) T();
            }
        }
        m_size = n;
    }

    T& at(size_t i){
        if(i>=m_size) [[unlikely]]throw std::out_of_range("Vector::at");
        return m_data[i];
    }
    
    T const& at(size_t i) const{
        if(i>=m_size) [[unlikely]]throw std::out_of_range("Vector::at");
        return m_data[i];
    }

    T& operator[](size_t i){
        return m_data[i];
    }

    T const& operator[](size_t i) const {
        return m_data[i];
    }

    T &front(){
        return m_data[0];
    }

    T const& front() const{
        return m_data[0];
    }

    T &back(){
        return m_data[m_size-1];
    }

    T const& back() const{
        return m_data[m_size-1];
    }

    T* data(){
        return m_data;
    }

    const T* data() const{
        return m_data;
    }

    iterator begin() noexcept{
        return m_data;
    } 

    const_iterator begin() const noexcept{
        return m_data;
    }

    const_iterator cbegin() const noexcept{
        return m_data;
    }
    
    iterator end() noexcept{
        return m_data + m_size;
    }
    
    const_iterator end() const noexcept{
        return m_data + m_size;
    }

    const_iterator cend() const noexcept{
        return m_data;
    }
    
    iterator rbegin() noexcept{
        return m_data + m_size - 1;
    }

    const_iterator rbegin() const noexcept{
        return m_data + m_size - 1;
    }

    const_iterator crbegin() const noexcept{
        return m_data + m_size - 1;
    }

    iterator rend() noexcept{
        return m_data - 1;
    }

    const_iterator rend() const noexcept{
        return m_data - 1;
    }

    const_iterator crend() const noexcept{
        return m_data - 1;
    }

    bool empty() const noexcept{
        return m_size == 0;
    }
    
    size_t size() const noexcept{
        return m_size;
    }

    void reserve(size_t n){
        if(n <= m_capa) return;

        auto old_data = m_data;
        auto old_capa = m_capa;

        n = std::max(n,m_capa*2);
        if(n == 0){
            m_data  = nullptr;
            m_capa  = 0;
        }else{
            m_data = m_alloc.allocate(n);
            m_capa = n;
        }

        if(old_capa != 0){  
            for(size_t i = 0 ; i < m_size; i++){
                ::new(static_cast<void*>(&m_data[i])) T(std::move(old_data[i]));
            }
            for(size_t i = 0 ; i < m_size; i++){
                std::destroy_at(&old_data[i]);
            }
            m_alloc.deallocate(old_data,old_capa);
        }

    }

    size_t max_size() const noexcept{
        return std::numeric_limits<size_t>::max()/sizeof(T);
    }

    size_t capacity() const{
        return m_capa;
    }
    
    void shrink_to_fit(){
        reserve(m_size);
    }
    
    void clear(){
        resize(0);
    }
    
    iterator insert(const_iterator pos,const T& value){
        size_t insert_pos = pos - this->begin();
        size_t old_size   = m_size;
        if(m_size + 1 > m_capa)
            resize(m_size + 1);
        else
            m_size = m_size + 1;
        for(size_t i = old_size; i > insert_pos;  i--){
            m_data[i] = std::move(m_data[i-1]);
        }
        ::new(static_cast<void*>(&m_data[insert_pos])) T(value);
        return m_data + insert_pos;
    }

    iterator insert(const_iterator pos,const T&& value){
        size_t insert_pos = pos - this->begin();
        size_t old_size   = m_size;
        if(m_size + 1 > m_capa)
            resize(m_size + 1);
        else
            m_size = m_size + 1;

        for(size_t i = old_size ; i > insert_pos;  i--){
            m_data[i] = std::move(m_data[i-1]);
        }
        ::new(static_cast<void*>(&m_data[insert_pos])) T(value);
        return m_data + insert_pos;
    }

    iterator insert(const_iterator pos, size_t count, const T& value)
    {
        size_t insert_pos = pos - this->begin();
        size_t old_size   = m_size;
        if(m_size + count > m_capa)
            resize(m_size + count);
        else
            m_size = m_size + count;
        
        for(size_t i = old_size ; i > insert_pos;  i--){
            m_data[ i + count - 1] = std::move(m_data[ i - 1]);
        }
        for(size_t i = 0; i < count ; i++){
            ::new(static_cast<void*>(&m_data[insert_pos+i])) T(value);
        }
        return m_data + insert_pos;
    }

    iterator insert(const_iterator pos, const_iterator first, const_iterator last){
        size_t count = last - first;
        size_t insert_pos = pos - this->begin();
        size_t old_size = m_size;
        if(m_size + count > m_capa)
            resize(m_size + count);
        else
            m_size = m_size + count;

        for(size_t i = old_size; i > insert_pos; i--){
            m_data[ i + count - 1]  = std::move(m_data[i - 1]);
        }

        for(size_t i = 0; i < count; i++){
            ::new(static_cast<void*>(&m_data[insert_pos+i])) T(*first);
            first++;
        }
        return this->begin() + insert_pos;
    }

    iterator insert(const_iterator pos, std::initializer_list<T> ilist){
        return insert(pos,ilist.begin(),ilist.end());
    }

    template<typename... Args>
    iterator emplace(const_iterator pos,Args&&... args){
        size_t insert_pos = pos - this->begin();
        size_t old_size   = m_size;
        if(m_size + 1 > m_capa)
            resize(m_size + 1);
        else
            m_size = m_size + 1;

        for(size_t i = old_size; i > insert_pos; i--){
            m_data[i]  = std::move(m_data[i - 1]);
        }

        ::new(static_cast<void*>(&m_data[insert_pos])) T(args...);
        return m_data + insert_pos;
    }

    iterator erase(const_iterator pos){
        size_t erase_pos = pos - this->begin();
        size_t old_size = m_size;
        m_size = m_size - 1;

        for(size_t i = erase_pos; i < old_size; i++){
            m_data[i] = std::move(m_data[i+1]);
        }
        std::destroy_at(&m_data[old_size-1]);
        return this->begin() + erase_pos;
    }

    iterator erase(const_iterator first, const_iterator last){
        size_t erase_count = last - first;
        size_t erase_start = first - this->begin();
        size_t old_size    = m_size;
        m_size = m_size - erase_count;

        for(size_t i = erase_start; i < old_size - erase_count + 1; i++){
            m_data[i] = std::move(m_data[i + erase_count]);
        }

        for(size_t i = old_size - erase_count; i < old_size; i++){
            std::destroy_at(&m_data[i]);
        }
        return this->begin() + erase_start;
    }
    
    void push_back(const T& value){
        insert(this->end(),value);
    }

    void push_back(T&& value){
        insert(this->end(),value);
    }

    template<typename... Args>
    void emplace_back(Args&&... args){
        emplace(this->end(),args);
    }

    void pop_back(){
        erase(this->end());
    }

    void swap(Vector<T>& other){
        std::swap(m_data,other.m_data);
        std::swap(m_size,other.m_size);
        std::swap(m_capa,other.m_capa);
        std::swap(m_alloc,other.m_alloc);
    }

    ~Vector(){
        for(size_t i = 0; i < m_size; i++){
            std::destroy_at(&m_data[i]);
        }
        m_alloc.deallocate(m_data,m_capa);
        std::destroy_at(&m_alloc);
    }
};