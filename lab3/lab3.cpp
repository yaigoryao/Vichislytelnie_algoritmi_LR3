#include <iostream>
#include <string>
#include <list>
#include <algorithm>
#include <stack>
#include <functional>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <chrono>
/*Класс Double Linked List*/
template <class T>
class DLL
{
public:
    class Item
    {
    public:
        T _value;
        Item* prev;
        Item* next;
        /*prev = next = nullptr*/
        Item() : prev{ nullptr }, next{ nullptr } {};
        /*_value = value
        prev = next = nullptr*/
        Item(T value) : Item(), _value{ value } {};
        /*_value = value
        prev = prev_
        next = next_*/
        Item(T value, Item* prev_, Item* next_) : _value{ value }, prev{ prev_ }, next{ next_ } {};
    };
protected:
    /*Класс Item элементы DLL*/
    struct DLLIter
    {
        explicit DLLIter(Item* ptr) : _ptr{ ptr } {};
        T& operator*() const { return this->_ptr->_value; };
        T* operator->() const { return &(this->_ptr->_value); };
        DLLIter& operator++() { this->_ptr = this->_ptr->next; return *this; };
        DLLIter& operator++(int) { auto ret = *this; this->_ptr = this->_ptr->next; return ret; };
        DLLIter& operator--() { this->_ptr = this->_ptr->prev; return *this; };
        DLLIter& operator--(int) { auto ret = *this; this->_ptr = this->_ptr->prev; return ret; };
        friend bool operator == (const DLLIter& first, const DLLIter& second) { return first._ptr == second._ptr; }
        friend bool operator != (const DLLIter& first, const DLLIter& second) { return first._ptr != second._ptr; }
    private:
        Item* _ptr;
    };

    struct const_DLLIter
    {
        explicit const_DLLIter(Item* ptr) : _ptr{ ptr } {};
        const_DLLIter(const const_DLLIter& it) : _ptr{ it._ptr } {};
        T& operator*() const { return this->_ptr->_value; };
        T* operator->() const { return &(this->_ptr->_value); };
        const_DLLIter& operator++() { this->_ptr = this->_ptr->next; return *this; };
        const_DLLIter& operator++(int) { auto ret = *this; this->_ptr = this->_ptr->next; return ret; };
        const_DLLIter& operator--() { this->_ptr = this->_ptr->prev; return *this; };
        const_DLLIter& operator--(int) { auto ret = *this; this->_ptr = this->_ptr->prev; return ret; };
        friend bool operator == (const const_DLLIter& first, const const_DLLIter& second) { return first._ptr == second._ptr; }
        friend bool operator != (const const_DLLIter& first, const const_DLLIter& second) { return first._ptr != second._ptr; }
    private:
        Item* _ptr;
    };

    Item* head = nullptr;
    Item* tail = nullptr;
    size_t _length = 0;
public:

    DLLIter begin() noexcept { return DLLIter(this->head); };
    DLLIter end() noexcept { return DLLIter(this->tail == nullptr ? nullptr : this->tail->next); };

    const_DLLIter cbegin() const noexcept { return const_DLLIter(this->head); };
    const_DLLIter cend() const noexcept { return const_DLLIter(this->tail == nullptr ? nullptr : this->tail->next); };

    DLL() : head{ nullptr }, tail{ nullptr }, _length{ 0 } {/* std::cout << "Construct: " << this << std::endl;*/ };

    DLL(std::initializer_list<T> li)
    {
        /*std::cout << "Construct: " << this << std::endl;*/
        auto begin = li.begin();
        while (begin != li.end())
        {
            this->emplace_back(*begin);
            begin++;
        }
    }
    ~DLL()
    {
        if (this->_length > 0)
        {
            /*std::cout << "Destroy: " << this << std::endl;*/
            Item* cur = this->head;
            Item* next = nullptr;
            while (cur != nullptr)
            {
                next = cur->next;
                delete cur;
                cur = next;
            }
            this->head = this->tail = nullptr;
            this->_length = 0;
        }
        /*delete cur;*/
    }

    DLL(const DLL& copy)
    {
        const_DLLIter begin = copy.cbegin();
        const_DLLIter end = copy.cend();
        while (begin != end)
        {
            this->emplace_back(*begin);
            begin++;
        }
    }

    DLL& operator=(const DLL& copy)
    {
        const_DLLIter begin = copy.cbegin();
        const_DLLIter end = copy.cend();
        while (begin != end)
        {
            this->emplace_back(*begin);
            begin++;
        }
        return *this;
    }

    DLL(DLL&& other)
    {
        this->head = other.head;
        this->tail = other.tail;
        this->_length = other._length;
        other._length = 0;
    }
    void emplace_back(T value)
    {
        if (!this->head) { this->head = new Item(value, nullptr, nullptr); }
        else if (!this->tail)
        {
            this->tail = new Item(value, this->head, nullptr);
            this->head->next = this->tail;
        }
        else
        {
            Item* cur = new Item(value, this->tail, nullptr);
            this->tail->next = cur;
            this->tail = cur;
        }
        this->_length++;
    }

    /*void for_each(void(*fun)(T _value) mutable)
    {
        if (this->_length > 0)
        {
            Item* cur = this->head;
            while (cur != this->tail)
            {
                fun(cur->_value);
                cur = cur->next;
            }
            fun(cur->_value);
        }
    }*/
    void for_each(std::function<void(T _value)> fun)
    {
        if (this->_length > 0)
        {
            Item* cur = this->head;
            while (cur != this->tail)
            {
                fun(cur->_value);
                cur = cur->next;
            }
            fun(cur->_value);
        }
    }

    void remove_back()
    {
        if (this->_length == 0) return;
        else if (this->_length > 2)
        {
            Item* remove = this->tail;
            this->tail = this->tail->prev;
            delete remove;
            this->tail->next = nullptr;
        }
        else if (this->_length == 2)
        {
            delete this->tail;
            this->tail = nullptr;
        }
        else
        {
            delete this->head;
            this->head = nullptr;
        }
        this->_length--;
    }

    void remove_front()
    {
        if (this->_length == 0) return;
        else if (this->_length > 2)
        {
            Item* remove = this->head;
            this->head = this->head->next;
            this->head->prev = nullptr;
            delete remove;
        }
        else if (this->_length == 2)
        {
            delete this->head;
            this->head = this->tail;
            this->head->prev = nullptr;
            this->tail = nullptr;
            this->head->next = this->tail;
        }
        else
        {
            delete this->head;
            this->head = nullptr;
        }
        this->_length--;
    }

    size_t length() const { return this->_length; }

    bool empty() const { return this->_length == 0; }
    void emplace_front(T value)
    {
        if (!this->head)
        {
            this->head = new Item(value, nullptr, nullptr);
        }
        else if (!this->tail)
        {
            this->tail = new Item(value, nullptr, this->head);
            this->head->prev = this->tail;
            Item* temp = this->head;
            this->head = this->tail;
            this->tail = temp;
        }
        else
        {
            Item* cur = new Item(value, nullptr, this->head);
            this->head->prev = cur;
            this->head = cur;
        }
        this->_length++;
    }

    /*bool any_of(void (*func)())
    {
        if (this->_length > 0)
        {
            Item* cur = this->head;
            while (cur != this->tail)
            {
                if (fun(cur->_value)) return true;
                cur = cur->next;
            }
            if (fun(cur->_value)) return true;
        }
        return false;
    }*/
    bool any_of(std::function<bool(T _value)> fun)
    {
        if (this->_length > 0)
        {
            Item* cur = this->head;
            while (cur != this->tail)
            {
                if (fun(cur->_value)) return true;
                cur = cur->next;
            }
            if (fun(cur->_value)) return true;
        }
        return false;
    }
    T front()
    {
        if (this->head) return this->head->_value;
    }
    T back()
    {
        if (this->tail) return this->tail->_value;
        else if (this->head) return this->head->_value;
    }
    T& operator[] (int index)
    {
        if (index >= 0 && index <= this->_length)
        {
            Item* cur = this->head;
            for (int i = 0; i < index; i++)
            {
                cur = cur->next;
            }
            return cur->_value;
        }
    }
};

template<class T>
class Stack
{
private:
    DLL<T> _container;
public:
    template< class T>
    friend class DLL;
    Stack(const DLL<T>& container) { this->_container = container; }
    void push(T value) { _container.emplace_front(value); }
    T top() { return _container.front(); }
    T pop()
    {
        T temp = _container.front();
        _container.remove_front();
        return temp;
    }
    bool empty(){ return _container.empty(); }
    size_t count() { return _container.length(); }
    DLL<T>& get_container() { return this->_container; }
};

template<class T>
class Queue
{
private:
    DLL<T> _container;
public:
    template< class T>
    friend class DLL;
    Queue(const DLL<T>& container) { this->_container = container; }
    void push(T value) { _container.emplace_front(value); }
    T last() { return _container.back(); }
    T shift()
    {
        T temp = _container.back();
        _container.remove_back();
        return temp;
    }
    bool empty() { return _container.empty(); }
    size_t count() { return _container.length(); }
    DLL<T>& get_container() { return this->_container; }

    T min(T _min)
    {
        T mn = _min;
        try 
        {
            _container.for_each([&mn](T value) {if (value < mn) mn = value; });
        }
        catch (...)
        {
            mn = {};
        }
        return mn;
    }

    T max(T _max)
    {
        T mx = _max;
        try
        {
            _container.for_each([&mx](T value) {if (value > mx) mx = value; });
        }
        catch (...)
        {
            mx = {};
        }
        return mx;
    }
};

class Person
{
private:
    std::string _surname = "";
    std::string _patronymics = "";
    std::string _name = "";
    std::string _job = "";
public:
    Person() : _surname{ "" }, _patronymics{ "" }, _name{ "" }, _job{""}{};
    Person(std::string name, std::string surname, std::string patronymics, std::string job) :
        _name{ name }, _surname{ surname }, _patronymics{ patronymics }, _job{ job }{};
    std::string getJob() const { return this->_job; };
    std::string getFullName() const { return (this->_name + " " + this->_surname + " " + this->_patronymics); }
    friend std::ostream& operator<< (std::ostream& os, const Person& person)
    {
        return os << person.getFullName();
    }
};

std::list<Person> matchPersonProfessions(std::list<Person> people, std::list<std::string> professions)
{
    std::list<Person> filtered = std::list<Person>();
    std::for_each(people.begin(), people.end(), [&filtered, professions](Person person)
        {
            if (std::any_of(professions.begin(), professions.end(), [person](std::string profession) {return person.getJob() == profession; }))
            {
                filtered.emplace_back(person);
            }
        });
    return filtered;
}

DLL<Person> matchPersonProfessions(DLL<Person> people, DLL<std::string> professions)
{
    /*DLL<Person> filtered = DLL<Person>();
    std::for_each(people.begin(), people.end(), [&filtered, &professions](Person person)
        {
            if (std::any_of(professions.begin(), professions.end(), [person](std::string profession) {return person.getJob() == profession; }))
            {
                filtered.emplace_back(person);
            }
        });
    return filtered;*/
    DLL<Person> filtered = DLL<Person>();
    people.for_each([&filtered, &professions](Person person)->void 
        {
            if (professions.any_of([&person](std::string profession) {return person.getJob() == profession; }))
            {
                filtered.emplace_back(person);
            }
        } );
    return filtered;
}

bool openSk(char sk)
{
    if (sk == '(' || sk == '[' || sk == '{') return true;
    else return false;
}

bool closeSk(char sk)
{
    if (sk == ')' || sk == ']' || sk == '}') return true;
    else return false;
}

char oppositeSk(char sk)
{
    switch (sk)
    {
    case '(':
        return ')';
        break;
    case '[':
        return ']';
        break;
    case '{':
        return '}';
        break;
    case ')':
        return '(';
        break;
    case ']':
        return '[';
        break;
    case '}':
        return '{';
        break;
    }
}

bool isValidSTL(std::string skobki)
{
    if (skobki.size() % 2 != 0) return false;
    std::stack<char> sequence = std::stack<char>();
    for (auto el : skobki)
    {
        if (openSk(el)) sequence.push(el);
        else if (closeSk(el))
        {
            if (sequence.empty() || sequence.top() != oppositeSk(el)) return false;
            else sequence.pop();
        }
    }
    return true;
}

bool isValid(std::string skobki)
{
    if (skobki.size() % 2 != 0) return false;
    Stack<char> sequence = Stack<char>(DLL<char>());
    for (auto el : skobki)
    {
        if (openSk(el)) sequence.push(el);
        else if (closeSk(el))
        {
            if (sequence.empty() || sequence.top() != oppositeSk(el)) return false;
            else sequence.pop();
        }
    }
    return true;
}

template<class T1, class T2>
struct Tuple
{
public:
    T1 _value1;
    T2 _value2;
    Tuple(T1 v1 = {}, T2 v2 = {}): _value1 { v1 }, _value2{ v2 } {};
};

Tuple<int, int> segmentWhereMinMultMaxGreatest(DLL<int>& seq, int length)
{
    if (length > seq.length()) return Tuple<int, int>(-1, -1);
    Queue<int> queue = Queue<int>(DLL<int>());
    int mult = -INT_MAX;
    Tuple <int, int> bord = Tuple<int, int>();
    for (int i = 0; i < length; i++)
    {
        queue.push(seq[i]);
    }
    for (int i = length; i < seq.length() - length; i++)
    {
        int cur = queue.max(-INT_MAX) * queue.min(INT_MAX);
        if (cur > mult)
        {
            mult = cur;
            bord._value1 = i - length;
            bord._value2 = i;
        }
        queue.shift();
        queue.push(seq[i]);
    }
    return bord;
    //int localmx = -INT_MAX;
    //int localmn = INT_MAX;
    //for (int i = 0; i < length; i++)
    //{
    //    int cur = values.shift();
    //    if (cur > localmx) localmx = cur;
    //    else if (cur < localmn) localmn = cur;
    //}
    //int mult = -INT_MAX;
    //while (values.count())
    //{
    //    int localmult = localmx * localmn;
    //    if (localmult > mult) mult = localmult;
    //    /*int localmult = localmx * localmn;
    //    if (localmult > mult) mult = localmult;
    //    int cur = values.shift();
    //    if (cur > localmx) localmx = cur;
    //    else if (cur < localmn) localmn = cur;*/
    //}
};

int main(int argc, char** argv)
{
    setlocale(0, "ru");
    
    //DLL<int> list = {-2, -1, 0};
    //std::for_each(list.begin(), list.end(), [](DLL<int>::Item el) {std::cout << el._value << std::endl; });
    /*list.for_each([](int &value) {std::cout << value << std::endl; });
    std::cout << "===========================" << std::endl;
    list.remove_front();
    list.remove_front();
    list.remove_front();
    list.for_each([](int& value) {std::cout << value << std::endl; });*/
    //list.remove_back();
    //list.remove_back();
    //list.remove_back();
    //list.remove_back();
    //list.for_each([](int& value) {std::cout << value << std::endl; });




    ////1 
    DLL<Person> employee = {
        Person("Архип", "Архипов", "Архипович", "Слесарь"),
        Person("Борис", "Борисов", "Борисович", "Директор"),
        Person("Валентин", "Валентинов", "Валентинович", "Повар"),
        Person("Геннадий", "Геннадьев", "Геннадьевич", "Сварщик"),
        Person("Дмитрий", "Дмитриев", "Дмитриевич", "Повар"),
        Person("Егор", "Егоров", "Егорович", "Повар"),
        Person("Жора", "Жоров", "Жорович", "Автомеханик"),
        Person("Заур", "Зауров", "Заурович", "Грузчик"),
        Person("Игорь", "Игорев", "Игоревич", "Охранник"),
        Person("Константин", "Константинов", "Константинович", "Грузчик"),
        Person("Любовь", "Любовина", "Любовьевна", "Библиотекарь"),
        Person("Михаил", "Михаилов", "Михаилович", "Слесарь"),
        Person("Николай", "Николаев", "Николаевич", "Уборщик"),
        Person("Олег", "Олегов", "Олегович", "Охранник"),
        Person("Петр", "Петров", "Петрович", "Уборщик"),
    };
    DLL<std::string> jobs = {
        "Повар",
        "Директор",
        "Охранник"
    };

    auto begin = std::chrono::high_resolution_clock::now();
    auto filtered = matchPersonProfessions(employee, jobs);
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed_ms = ((std::chrono::nanoseconds)(end-begin)).count();
    std::cout << "Время работы алгоритма (собственные структуры): " << elapsed_ms / std::pow(10, 6) << std::endl;
    for (auto& person : filtered) { std::cout << person << std::endl; } 
    



    /*std::list <Person> employee = {
        Person("Архип", "Архипов", "Архипович", "Слесарь"),
        Person("Борис", "Борисов", "Борисович", "Директор"),
        Person("Валентин", "Валентинов", "Валентинович", "Повар"),
        Person("Геннадий", "Геннадьев", "Геннадьевич", "Сварщик"),
        Person("Дмитрий", "Дмитриев", "Дмитриевич", "Повар")
    };
    std::list<std::string> jobs = {
        "Повар",
        "Директор"
    };
    auto filtered = matchPersonProfessions(employee, jobs);
    for (auto& person : filtered) { std::cout << person << std::endl; }*/


    //2
    std::cout << std::endl;
    
    int count = 0;
    std::cout << "Введите длину последовательности: ";
    std::cin >> count;
    std::cout << std::endl;
    std::string seq = "";
    std::cout << "Введите последовательность скобок: ";
    for (int i = 0; i < count; i++)
    {
        char sk = '\0';
        std::cin >> sk;
        seq += sk;
    }
    std::cout << std::endl;

    begin = std::chrono::high_resolution_clock::now();
    bool r1 = isValid(seq);
    end = std::chrono::high_resolution_clock::now();
    elapsed_ms = ((std::chrono::nanoseconds)(end - begin)).count();
    std::cout << "Время работы алгоритма (собственные структуры): " << elapsed_ms / std::pow(10, 6) << std::endl;
    std::cout << "Последовательность верна (собственные структуры)?" << (r1 ? " да" : " нет") << std::endl;

    begin = std::chrono::high_resolution_clock::now();
    bool r2 = isValidSTL(seq);
    end = std::chrono::high_resolution_clock::now();
    elapsed_ms = ((std::chrono::nanoseconds)(end - begin)).count();
    std::cout << "Время работы алгоритма (STL-структуры): " << elapsed_ms / std::pow(10, 6) << std::endl;
    std::cout << "Последовательность верна (STL-структуры)?" << (r2 ? " да" : " нет") << std::endl;
   
    ////3 
    DLL<int> sequence = DLL<int>();

    int size = 0;
    std::cout << "Введите длину последовательности: ";
    std::cin >> size;
    int sub_size = 0;
    std::cout << "Введите длину подпоследовательности: ";
    std::cin >> sub_size;
    std::cout << std::endl;
    for (int i = 0; i < size; i++)
    {
        int el = 0;
        std::cout << "Введите элемент " + std::to_string(i+1) + " последовательности: ";
        std::cin >> el;
        sequence.emplace_back(el);
    }

    begin = std::chrono::high_resolution_clock::now();
    Tuple<int, int> bord = segmentWhereMinMultMaxGreatest(sequence, sub_size);
    end = std::chrono::high_resolution_clock::now();
    elapsed_ms = ((std::chrono::nanoseconds)(end - begin)).count();
    std::cout << "Время работы алгоритма (собственные структуры): " << elapsed_ms / std::pow(10, 6) << std::endl;
    std::cout << "Отрезок с максимальным произведением min*max: " << "[" << bord._value1 << "," << bord._value2 << "]" << std::endl;
     
    //std::cout << std::endl;
    //int count = 0;
    //std::cout << "Введите длину последовательности: ";
    //std::cin >> count;
    //std::cout << std::endl;
    //std::string seq = "";
    //std::cout << "Введите последовательность скобок: ";
    //for (int i = 0; i < count; i++)
    //{
    //    char sk = '\0';
    //    std::cin >> sk;
    //    seq += sk;
    //}
    //std::cout << std::endl;
    //std::cout << "Последовательность верна?" << (isValid(seq) ? " Да" : " Нет") << std::endl;
    //_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    //_CrtDumpMemoryLeaks();
    return 0;
}