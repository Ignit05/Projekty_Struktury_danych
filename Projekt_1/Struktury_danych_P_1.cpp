#include <iostream>
#include <stdexcept>
#include <chrono>
using namespace std;
//Tablica dyamiczna
class DynamicArray {
    int* data;
    int size;
    int capacity;

    void resize() {
        capacity *= 2;
        int* newData = new int[capacity];
        for (int i = 0; i < size; i++) newData[i] = data[i];
        delete[] data;
        data = newData;
    }
public:
    DynamicArray(int n = 2) : size(0), capacity(n) {
    data = new int[capacity];
    }
    ~DynamicArray() { delete[] data; }
    void addEnd(int value) {
    if (size == capacity) resize();
    data[size++] = value;
    }
    void addStart(int value) {
    if (size == capacity) resize();
    for (int i = size; i > 0; i--) data[i] = data[i - 1];
    data[0] = value;
    size++;
    }
    void addAtIndex(int index, int value) {
    if (index < 0 || index > size) throw out_of_range("Index out of bounds");
    if (size == capacity) resize();
    for (int i = size; i > index; i--) data[i] = data[i - 1];
    data[index] = value;
    size++;
    }

    void removeEnd() { if (size > 0) size--; }
    void removeStart() {
    if (size == 0) return;
    for (int i = 0; i < size - 1; i++) data[i] = data[i + 1];
    size--;
    }
    void removeAtIndex(int index) {
    if (index < 0 || index >= size) return;
    for (int i = index; i < size - 1; i++) data[i] = data[i + 1];
    size--;
    }
    int search(int value) {
    for (int i = 0; i < size; i++) if (data[i] == value) return i;
    return -1;
    }
    //test
    void display() {
    cout << "[ ";
    for (int i = 0; i < size; i++) cout << data[i] << " ";
    cout << "]" << endl;
    }
};
//Lista wiazana
struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
};
class LinkedList {
    Node* head;
public:
    LinkedList() : head(nullptr) {}
    void addStart(int value) {
    Node* newNode = new Node(value);
    newNode->next = head;
    head = newNode;
    }
    void addEnd(int value) {
    Node* newNode = new Node(value);
    if (!head) { head = newNode; return; }
    Node* temp = head;
    while (temp->next) temp = temp->next;
    temp->next = newNode;
    }
void removeStart() {
if (!head) return;
Node* temp = head;
head = head->next;
delete temp;
}
int search(int value) {
Node* temp = head;
int idx = 0;
while (temp) {
if (temp->data == value) return idx;
temp = temp->next;
idx++;
}
return -1;
}
//test
void display() {
Node* temp = head;
cout << "( ";
while (temp) {
cout << temp->data << " -> ";
temp = temp->next;
}
cout << "NULL )" << endl;
}
};
int main() {
//Test tablica dynamiczna
cout << "TEST: Tablica dynamiczna" << endl;
DynamicArray arr(2);
cout << "Dodawanie na koniec (10, 20): ";
arr.addEnd(10);
arr.addEnd(20);
arr.display();
cout << "Dodawanie na poczatek (5): ";
arr.addStart(5);
arr.display();

cout << "Dodawanie na indeks 1 (15): ";
arr.addAtIndex(1, 15);
arr.display();
cout << "Wyszukiwanie wartosci 20: Pozycja " << arr.search(20) << endl;
cout << "Usuwanie z poczatku i konca: ";
arr.removeStart();
arr.removeEnd();
arr.display();
cout << endl;
//Test lsit wiazanej
cout << "TEST: Lista wiazana" << endl;
LinkedList list;
cout << "Dodawanie na start (100) i koniec (200, 300):" << endl;
list.addStart(100);
list.addEnd(200);
list.addEnd(300);
list.display();
cout << "Wyszukiwanie wartosci 200: Pozycja " << list.search(200) << endl;
cout << "Usuwanie z poczatku:" << endl;
list.removeStart();
list.display();
cout << "\nDziała, git" << endl;
return 0;
}