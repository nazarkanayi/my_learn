#include <iostream>


template <typename T>
class Vector
{

    public:

    typedef T* iterator;
    Vector():buffer{nullptr},size{0},capacity{0}
    {

    }

    Vector(std::size_t l_size)
    {
        size= l_size;
        capacity = l_size;

        buffer = new T[capacity];

    }

     Vector( std::initializer_list<T> init)
     {
         size = init.size();
         capacity = init.size();
            buffer = new T[size];

            for(int i=0 ; i< size; i++)
            {
                buffer[i] = *(init.begin()+i);
            }
         
     }

    ~Vector()
    {
        delete[] buffer;
        
    }

    void reserve(std::size_t l_size)
    {

        if(l_size > capacity)
        {
            T * newBuffer = new T[l_size];
            for(int i =0 ; i< size ; i++)
            {
                newBuffer[i] = buffer[i];
            }

            capacity = l_size;
            delete[] buffer;

            buffer = newBuffer;

        }
    }

    void push_back( const T& value )
    {
        if(size >= capacity)
        {
            reserve(capacity+10);
        }
        buffer[size++] = value;
    }

    iterator end()
    {
       return buffer+size;
    }

    iterator begin()
    {
        return buffer;
    }
    private:
    T * buffer;
    int size;
    int capacity;
};


int main()
{
    Vector<int> array_v= {1,3};

    array_v.push_back(1);
    array_v.push_back(2);
    array_v.push_back(5);
    array_v.push_back(6);


    for(auto it =array_v.begin(); it!= array_v.end();it++ )
    {
        std::cout << *it << std::endl;
    }
}
