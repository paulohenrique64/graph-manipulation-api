#pragma once
#include <iostream> 

using namespace std; 

template <typename T>
class List {
    public:
        List() = default; 
        List(int size, T value); 
        List(const List<T>& other); 
        bool operator==(const List<T>& other) const; 
        bool operator!=(const List<T>& other) const; 
        List<T>& operator=(const List<T>& other); 
        T& operator[](const int index) const;
        template <typename U> friend ostream& operator<<(ostream& os, const List<U>& list);
        ~List(); 
        void insert(T element);
        void insertAt(int index, T element);
        void remove(T element);
        T removeFirst();
        bool has(T element) const;
        int indexOf(T element) const;
        T& at(int index) const;
        int size() const;
        void printList() const;
        bool empty();
        void sort();
        void reverse();
        void clear();

    private:
        T* list = nullptr;
        int numElements = 0;
        void deleteList();
};


template <typename T>
List<T>::List(const List<T>& other) {
    if (this != &other) {
        this->numElements = other.size();
        this->list = new T[other.size()];

        for (int i = 0; i < other.size(); i++) {
            this->list[i] = other[i];
        }
    }
}

template <typename T>
List<T>::List(int size, T value) {
    numElements = size;
    list = new T[numElements];

    for (int i = 0; i < numElements; i++)
        list[i] = value;

} 

template <typename T>
List<T>::~List() {
    this->deleteList();
}

template <typename T>
void List<T>::insert(T element) {
    T *newList = new T[this->numElements+1];

    for (int i = 0; i < this->numElements; i++) {
        newList[i] = this->list[i];
    }
    newList[this->numElements] = element;

    delete[] this->list;
    this->list = newList;
    this->numElements++;
}

template <typename T>
void List<T>::insertAt(int index, T element) {
    this->list[index] = element;
}

template <typename T>
void List<T>::remove(T element) {
    if (!this->has(element)) return;

    T *newList = new T[this->numElements-1];

    int i = 0;
    int count = 0;

    while (this->list[i] != element) 
        i++;

    for (int j = 0; j < this->numElements; j++) {
        if (j != i)
            newList[count++] = this->list[j];
    }

    delete[] this->list;
    this->list = newList;
    this->numElements--;
}

template <typename T>
T List<T>::removeFirst() {
    T element = this->list[0];
    this->remove(element);
    return element;
}

template <typename T>
bool List<T>::has(T element) const {
    for (int i = 0; i < this->numElements; i++) {
        if (this->list[i] == element) return true;
    }

    return false;
}

template <typename T>
int List<T>::indexOf(T element) const {
    for (int i = 0; i < this->numElements; i++) {
        if (this->list[i] == element) return i;
    }

    return -1;
}

template <typename T>
T& List<T>::at(int index) const {
    return this->list[index];
}

template <typename T>
void List<T>::deleteList() {
    this->numElements = 0;
    
    if (this->list != nullptr) {
        delete[] this->list;
        this->list = nullptr;
    }
}

template <typename T>
void List<T>::printList() const {
    for (int i = 0; i < this->numElements; i++) {
        cout << this->list[i] << " ";
    }
    cout << endl;
}

template <typename T>
int List<T>::size() const {
    return this->numElements;
}

template <typename T>
bool List<T>::operator==(const List<T>& other) const {
    for (int i = 0; i < other.size(); i++) {
        if (this->list[i] != other[i]) return false;
    }
    return true;
}

template <typename T>
bool List<T>::operator!=(const List<T>& other) const {
    for (int i = 0; i < other.size(); i++) {
        if (this->list[i] != other[i]) return true;
    }
    return false;
}

template <typename T>
List<T>& List<T>::operator=(const List<T>& other) {
    if (this != &other) {
        this->numElements = other.size();
        this->list = new T[other.size()];

        for (int i = 0; i < other.size(); i++) {
            this->list[i] = other[i];
        }
    }

    return *this;
}

template <typename T>
ostream& operator<<(ostream& os, const List<T>& list) {
    for (int i = 0; i < list.size(); i++) {
        os << list[i] << " ";
    }
    cout << endl;
    return os;      
}

template <typename T>
T& List<T>::operator[](const int index) const {
    return this->list[index];
}

template <typename T>
bool List<T>::empty() {
    return this->numElements == 0;
}

template <typename T>
void List<T>::sort() {
    T* list = this->list;
    int size = this->numElements;

    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            if (list[i] > list[j]) {
                T aux = list[j];
                list[j] = list[i];
                list[i] = aux;
            }
        }
    }
}

template <typename T>
void List<T>::reverse() {
    int j = this->numElements - 1;
    for (int i = 0; i < this->numElements / 2; i++) {
        T aux = this->list[i];
        this->list[i] = this->list[j];
        this->list[j--] = aux;
    }
}

template <typename T>
void List<T>::clear() {
    this->deleteList();
}
 