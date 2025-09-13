#include <iostream>
#include <vector>
#include "chainhash.h"

using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// crear una lista desde un vector
ListNode* createList(const vector<int>& values) {
    if (values.empty()) return nullptr;
    
    ListNode* head = new ListNode(values[0]);
    ListNode* current = head;
    
    for (size_t i = 1; i < values.size(); i++) {
        current->next = new ListNode(values[i]);
        current = current->next;
    }
    
    return head;
}

// imprimir una lista
void printList(ListNode* head, int maxNodes = 10) {
    ListNode* current = head;
    int count = 0;
    
    cout << "[";
    while (current && count < maxNodes) {
        cout << current->val;
        if (current->next && count < maxNodes - 1) cout << " -> ";
        current = current->next;
        count++;
    }
    if (current) cout << " -> ...";
    cout << "]";
}

// encontrar un nodo por valor en una lista
ListNode* findNode(ListNode* head, int value) {
    while (head) {
        if (head->val == value) return head;
        head = head->next;
    }
    return nullptr;
}

// TODO: implementar la conexión de las listas en el nodo con valor intersectVal
// Si intersectVal no se encuentra, las listas permanecen separadas   
void connectLists(ListNode*& listA, ListNode*& listB, int intersectVal) {
    //TODO: implemente aqui
    
    // encontramos el nodo en A
    auto connectTo = findNode(listA, intersectVal);
    // si ese nodo es nullptr, o listB es nullptr, no hacer nada
    if (connectTo == nullptr || listB == nullptr) { return; }
    // si listB ya tiene el valor de A, entonces solo basta con cambiar el valor
    // notese que la firma de la funcion tiene pasa por referencia para que esto sea posible
    if (listB->val == intersectVal) { listB = connectTo; return; }

    // buscar el nodo en listB
    while (listB && listB->next) {
        // si lo encontramos, conectarlo con el nodo en A
        if (listB->next->val == intersectVal) { listB->next = connectTo; return; }
        listB = listB->next;
    }
}


// TODO: implementar el algoritmo para encontrar la intersección de dos listas
ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {

    // para usar el archivo chainhash, aqui creamos un mapa en vez de un set
    // es nodo:char porque char es el tipo de dato mas pequenyo
    ChainHash<ListNode*, char> visited;

    // guardamos todos los valores de A en el mapa
    while (headA != nullptr) {
        // los valores realmente no importan, por eso los guardo como ' '
        visited.set(headA, ' ');
        headA = headA->next;
    }

    // iteramos B
    while (headB != nullptr) {
        // si encontramos el nodo actual en el mapa, retornamos ese
        if (visited.contains(headB)) { return headB; }
        headB = headB->next;
    }

    // si llegamos aqui es que no lo hemos encontrado
    return nullptr;
}

void testIntersection(const vector<int>& listA, const vector<int>& listB, 
                     int intersectVal, const string& testName) {
    cout << "\n" << testName << "\n";
    
    ListNode* headA = createList(listA);
    ListNode* headB = createList(listB);
    
    // crear interseccion 
    if (intersectVal != -1) {
        connectLists(headA, headB, intersectVal);
    }
    
    cout << "Lista A: ";
    printList(headA);
    cout << "\nLista B: ";
    printList(headB);
    cout << "\n";
    
    // encontrar interseccion
    ListNode* intersection = getIntersectionNode(headA, headB);
    
    if (intersection) {
        cout << "Intersección encontrada en nodo con valor: " << intersection->val << "\n";
    } else {
        cout << "No hay intersección\n";
    }
    
    if (intersectVal == -1) {
        if (intersection == nullptr) {
            cout << "CORRECTO: No se esperaba intersección\n";
        } else {
            cout << "ERROR: Se encontró intersección cuando no debería\n";
        }
    } else {
        if (intersection && intersection->val == intersectVal) {
            cout << "CORRECTO: Intersección encontrada en el nodo esperado\n";
        } else {
            cout << "ERROR: Intersección incorrecta o no encontrada\n";
        }
    }
}

int main() {
    cout << "INTERSECCION DE LISTAS ENLAZADAS\n";
    cout << "Problema: Encontrar el nodo donde dos listas se intersectan\n";    
    
    // Caso 1: Intersección existe
    testIntersection({4, 1, 8, 4, 5}, {5, 6, 1, 8, 4, 5}, 8, 
                    "Caso 1: Con intersección");
    
    // Caso 2: Sin intersección
    testIntersection({2, 6, 4}, {1, 5}, -1, 
                    "Caso 2: Sin intersección");
    
    // Caso 3: Intersección al inicio
    testIntersection({3, 7, 1}, {3, 7, 1}, 3, 
                    "Caso 3: Intersección al inicio");
    
    // Caso 4: Una lista vacía
    testIntersection({1, 2, 3}, {}, -1, 
                    "Caso 4: Lista B vacía");
    
    // Caso 5: Listas de diferente longitud con intersección
    testIntersection({1, 9, 1, 2, 4}, {3, 2, 4}, 2, 
                    "Caso 5: Diferentes longitudes con intersección");
    
    return 0;
}
