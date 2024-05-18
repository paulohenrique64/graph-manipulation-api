#pragma once
#include <iostream> 

using namespace std; 

template <typename T>
class List {
    public:
        List() = default; // construtor
        List(const List<T>& other); // construtor de copia
        bool operator==(const List<T>& other) const; 
        bool operator!=(const List<T>& other) const; 
        List<T>& operator=(const List<T>& other); 
        template <typename U> friend ostream& operator<<(ostream& os, const List<U>& list);
        ~List(); // destrutor
        void insert(T element);
        void insertAt(int index, T element);
        void remove(T element);
        bool has(T element) const;
        int indexOf(T element) const;
        T& at(int index) const;
        int length() const;
        void printList() const;

    private:
        T* list = nullptr;
        int numElements = 0;
        void deleteList();
};


template <typename T>
List<T>::List(const List<T>& other) {
    if (this != &other) {
        this->numElements = other.length();
        this->list = new T[other.length()];

        for (int i = 0; i < other.length(); i++) {
            this->list[i] = other.at(i);
        }
    }
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
    exception e;

    if (index < 0 or index >= this->numElements) throw e;

    this->list[index] = element;
}

template <typename T>
void List<T>::remove(T element) {
    if (!this->has(element)) return;

    T *newList = new T[this->numElements-1];
    int count = 0;

    for (int i = 0; i < this->numElements; i++) {
        if (this->list[i] != element) {
            newList[count++] = this->list[i];
        }
    }

    delete[] this->list;
    this->list = newList;
    this->numElements--;
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
    if (index >= 0 and index < this->numElements) {
        return this->list[index];
    } else {
        exception e;
        throw e;
    }
}

template <typename T>
void List<T>::deleteList() {
    this->numElements = 0;
    delete[] this->list;
}

template <typename T>
void List<T>::printList() const {
    for (int i = 0; i < this->numElements; i++) {
        cout << this->list[i] << " ";
    }
    cout << endl;
}

template <typename T>
int List<T>::length() const {
    return this->numElements;
}

template <typename T>
bool List<T>::operator==(const List<T>& other) const {
    for (int i = 0; i < other.length(); i++) {
        if (this->list[i] != other.at(i)) return false;
    }
    return true;
}

template <typename T>
bool List<T>::operator!=(const List<T>& other) const {
    for (int i = 0; i < other.length(); i++) {
        if (this->list[i] != other.at(i)) return true;
    }
    return false;
}

template <typename T>
List<T>& List<T>::operator=(const List<T>& other) {
    if (this != &other) {
        this->numElements = other.length();
        this->list = new T[other.length()];

        for (int i = 0; i < other.length(); i++) {
            this->list[i] = other.at(i);
        }
    }

    return *this;
}

template <typename T>
ostream& operator<<(ostream& os, const List<T>& list) {
    for (int i = 0; i < list.length(); i++) {
        os << list.at(i) << " ";
    }
    cout << endl;
    return os;      
}
