#include <stdexcept>
#include <vector>

using namespace std;

const int maxColision = 3;
const float maxFillFactor = 0.8;

template<typename TK, typename TV>
struct ChainHashNode {
    TK key;
    TV value;
    ChainHashNode* next = nullptr;

    ChainHashNode() = default;
    ChainHashNode(TK _key, TV _value) : key(_key), value(_value) { }
};

template<typename TK, typename TV>
class ChainHashListIterator {
 	typedef ChainHashNode<TK, TV> Node;
    Node* current;

    public:

    ChainHashListIterator(Node* current) { this->current = current; }

    ChainHashListIterator operator++() {
        this->current = this->current->next;
        return *this;
    }

    bool operator!=(const ChainHashListIterator& other) const { return this->current != other.current; }

    Node& operator*() { return *this->current; }


};

template<typename TK, typename TV>
class ChainHash {
private:    
    typedef ChainHashNode<TK, TV> Node;
    typedef ChainHashListIterator<TK, TV> Iterator;

	Node** array;  // array de punteros a Node
    int nsize; // total de elementos <key:value> insertados
	int capacity; // tamanio del array
	int *bucket_sizes; // guarda la cantidad de elementos en cada bucket
	int usedBuckets; // cantidad de buckets ocupados (con al menos un elemento)

public:
    ChainHash(int initialCapacity = 10){
		this->capacity = initialCapacity; 
		this->array = new Node*[capacity]();  
		this->bucket_sizes = new int[capacity]();
		this->nsize = 0;
		this->usedBuckets = 0;
	}

	TV get(TK key){
		size_t hashcode = getHashCode(key);
		size_t index = hashcode % capacity;
		
		Node* current = this->array[index];
		while(current != nullptr){
			if(current->key == key) return current->value;
			current = current->next;
		}
		throw std::out_of_range("Key no encontrado");
	}
	
	int size(){ return this->nsize; }	

	int bucket_count(){ return this->capacity; }

	int bucket_size(int index) { 
		if(index < 0 || index >= this->capacity) throw std::out_of_range("Indice de bucket invalido");
		return this->bucket_sizes[index]; 
	}	
	
	// TODO: implementar los siguientes métodos
	void set(TK key, TV value) {
        if (fillFactor() > maxFillFactor) { rehashing(); }

        // hallar index
        size_t hashcode = getHashCode(key);
        size_t index = hashcode % capacity;

        // si es que esta vacio el actual
        if (array[index] == nullptr) { ++usedBuckets; }

        // si la llave ya existe, actualizar value
    	Node* current = this->array[index];
		while(current != nullptr){
			if(current->key == key) {
                current->value = value;
                return;
            }
			current = current->next;
		}

        // si no, push front a la lista enlazada en array[index]
        Node* temp = new Node(key, value);
        temp->next = array[index];
        array[index] = temp;

        // actualizar contadores
        nsize++;
        bucket_sizes[index]++;

        // verificar si hay demasiados elementos en un index
        if (bucket_sizes[index] > maxColision) { rehashing(); }
    }

	bool remove(TK key) {
        size_t hashcode = getHashCode(key);
        size_t index = hashcode % capacity;

        Node* current = this->array[index];

        if (current == nullptr) { return false; }

        if (current->key == key) {
            this->array[index] = current->next;
            delete current;
            --nsize;
            bucket_sizes[index]--;
            if (array[index] == nullptr) { --usedBuckets; }
            return true;
        }

        while (current->next != nullptr) {
            if (current->next->key == key) { 
                Node* newNext = current->next->next;
                delete current->next;
                current->next = newNext;
                --nsize;
                bucket_sizes[index]--;
                return true;
            }
            current = current->next;
        }

        return false;
    }

	bool contains(TK key) {
        size_t hashcode = getHashCode(key);
        size_t index = hashcode % capacity;

        Node* current = this->array[index];

        while (current != nullptr) {
            if (current->key == key) { return true; }
            current = current->next;
        }

        return false;
    }

	Iterator begin(int index) {
        return Iterator(this->array[index]);
    }	

	Iterator end(int index) {
        return Iterator(nullptr);
    }

private:
	double fillFactor(){
		return (double)this->usedBuckets / (double)this->capacity;
	}	

	size_t getHashCode(TK key){
		std::hash<TK> ptr_hash;
		return ptr_hash(key);
	}

	//TODO: implementar rehashing
	void rehashing() {
        Node** oldArray = this->array;
        int oldCapacity = this->capacity;

        this->capacity = this->capacity * 2;
        this->array = new Node*[this->capacity]();
        this->bucket_sizes = new int[this->capacity]();
        this->nsize = 0;
        this->usedBuckets = 0;

        for (int i = 0; i < oldCapacity; ++i) {
            Node* current = oldArray[i];
            while (current != nullptr) {
                set(current->key, current->value);
                Node* next = current->next;
                current = next;
            }
        }

        delete[] oldArray;
    }

public:
	// TODO: implementar destructor

	~ChainHash() {
        for (int i = 0; i < this->capacity; ++i) {
            Node* current = this->array[i];
            while (current != nullptr) {
                Node* next = current->next;
                delete current;
                current = next;
            }
        }
        delete[] this->array;
        delete[] this->bucket_sizes;
    }
};
