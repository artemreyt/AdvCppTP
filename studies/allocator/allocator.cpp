#include <memory>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <utility>


int return_int() {
    return int();
}

template <typename T>
class my_allocator : public std::allocator<T>
{
public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef std::size_t size_type;


    my_allocator() = default;

    T *allocate(std::size_t n)
    {
        std::cout << "Allocator here: " << typeid(T).name() << std::endl;
        return reinterpret_cast<T *>(::operator new(n*sizeof(T)));
    }

    void deallocate(T *p, std::size_t n)
    {
       ::operator delete(p);
    }

    template<class Other>
    struct rebind
    {
        typedef my_allocator<Other> other;
    };

};
//
//    template <class K, class T>
//    std::pair<K, T> *my_allocator<std::pair<K, T>>::allocate(std::size_t n) {
//        std::cout << "Allocator here" << std::endl;
//        return reinterpret_cast<T*>(new T);
//    }


namespace art {
    template <class CharT>
    using string = std::basic_string<CharT, std::char_traits<CharT>, my_allocator<char>>;


}


int main() {

//    using allocator = my_allocator<std::pair<const int, art::string<char>>>;
//    std::map<int, art::string<char>,  std::less<int>, allocator> map{allocator()};
//    map[1] = "lalalalalalalalalashshshhsshshshshshhajhsjahsjashsajhs";
//    map[2] = "ahahahahahahahahahaah";
//
//    std::cout << map[2] << " " << map[1] << std::endl;

    std::map<int, std::string, std::less<int>, std::allocator<std::pair<const int, std::string>>> map((std::allocator<std::pair<const int, std::string>>()));
//    std::vector<std::basic_string<char, std::char_traits<char>, my_allocator<char>>,
//            my_allocator<std::basic_string<char, std::char_traits<char>, my_allocator<char>>>> vec;
//    std::vector<art::string<char> , my_allocator<art::string<char>>> vec(my_allocator<art::string<char>>());
    std::vector<int, my_allocator<int>> vec(my_allocator<int>{});



    vec.push_back(2);


    std::cout << std::boolalpha << std::is_default_constructible_v<int> << std::endl;

//    vec.push_back("first lalalalalaaaalalalallalalalalallalaalalalalalalalalalalalaalal");
//    vec.push_back("second hahahahahaahahahahaah");
//    vec.push_back("third trololololololololo");
//    std::cout << vec[0] << std::endl;
//    std::cout << vec[1] << std::endl;
//    std::cout << vec[2] << std::endl;
}




