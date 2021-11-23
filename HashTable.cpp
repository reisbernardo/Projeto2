/*
Nome: Bernardo Siqueira Esteves dos Reis
TIA: 32019270
Nome:  Pedro Henrique Ikeda
TIA: 32016344
Nome: Lucas Kenzo Akiyama      
TIA: 32025165
*/

#include <iostream>
#include <string>
#include <fstream>
#include <time.h>
#define PRIME 86969 // Prime number to Hash Function
#define MAX 128 // Max string value
#define TABLE 16384 // Max table size = 2 ^ 14
using namespace std;

// Class of each instance (node) in the Hash Table
class HashNode {
    public:
        char name[MAX];
        char country[MAX];
        char discipline[MAX];
        int deleted;

        /* Constructor function that copies each char array from 
        parameter using strncpy() into each class attribute */
        HashNode(char *name, char *country, char *discipline, int deleted){
            strncpy(this->name, name, MAX);
            strncpy(this->country, country, MAX);
            strncpy(this->discipline, discipline, MAX);
            this->deleted = deleted;
        }
    };
    
// Class representing the Hash Table  
class HashTable {
    private:
        HashNode** table; // Array of HashNode objects
        int max; // Max Hash Table size
        int current_size; // Current Hash Table size
        int collisions; // Number of collisions

    public:
        /* Constructor function that creates the Array of HasHNode objects
        then to each element in the array assigns the NULL value to sinalize
        it is empty */
        HashTable(int max){
            this->max = max;
            collisions = 0;
            current_size = 0;
            table = new HashNode* [max];
            for (int i = 0; i < max; i++) table[i] = NULL;
        }

        /* Hash Function that starts the hash_value with the macro FIRST and 
        for each iteration from the for it multiplies the hash_value with 33 (magic number) 
        and do the power of the current character from string its ASCII code value, 
        then do hash_value mod (%) PRIME (prime number),
        and returns that value mod (%) TABLE size. */ 
        unsigned int hashFunc(char *name){
            int n = strnlen_s(name, MAX);
            unsigned int hash_value = TABLE / 2;
            for (int i = 0; i < n; i++)
                hash_value = (hash_value * 33) ^ name[i];
            hash_value %= PRIME;
            return hash_value % TABLE;
        }

        // Insert function that uses Linear Probing for collisions
        bool insertNode(char *name, char *country, char *discipline){
            if (max == current_size) return false;
            HashNode* temp = new HashNode(name, country, discipline, 1);
            int index = hashFunc(name);
            for(int i=0; i < max; i++){
                int next = (i + index) % max;
                if(table[next] == NULL || table[next]->deleted == -1){
                    table[next] = temp;
                    current_size++;
                    return true;
                }
                collisions++;
            }
            return false;
        }

        /* Search function that gets Node using its Hash Function, 
        and continues iteration if it was deleted to check if it was later added again, because of collision */
        HashNode* searchNode(char *name){
            int index = hashFunc(name);
            for(int i=0; i < max; i++){
                int next = (i + index) % max;
                if(table[next] == NULL) return NULL;
                if(table[next]->deleted == -1) continue;
                if(strcmp(table[next]->name, name) == 0) return table[next];
            }
            return NULL;
        }

        // Delete function that deletes value and assigns its deleted attribute to -1
        HashNode* deleteNode(char *name){
            int index = hashFunc(name);
            for(int i=0; i < max; i++){
                int next = (i + index) % max;
                if(table[next] == NULL) return NULL;
                if(table[next]->deleted == -1) continue;
                if(strcmp(table[next]->name, name) == 0){
                    HashNode* temp = table[next];
                    table[next]->deleted = -1;
                    current_size--;
                    return temp;
                }
            }
            return NULL;
        }
        
        // Prints all table elements
        void printTable(){
            cout << "START\n";
            for(int i = 0; i < max; i++){
                if(table[i] == NULL || table[i]->deleted == -1) cout << i << "\t---\n";
                else cout << i << "\t" << table[i]->name << "\t" << 
                    table[i]->country << "\t" << table[i]->discipline << "\n";
            }
            cout << "END\n";
        }

        // Function that reads an csv file and inserts it to the Hash Table
        void insertDataset(char* filename){
            fstream myfile;
            myfile.open(filename);
            char c1[MAX], c2[MAX], c3[MAX];
            
            while(myfile.good()){
                string line;
                int cont = 1, index = 0;
                getline(myfile, line);

                for (int i = 0; i < line.length(); i++) {
                    if (line[i] == ','){
                        cont++;
                        index = 0;
                    }
                    else if (line[i] != '"')
                        switch (cont) {
                            case 1:
                                c1[index++] = line[i];
                                c1[index] = '\0';
                                break;
                            case 2:
                                c2[index++] = line[i];
                                c2[index] = '\0';
                                break;
                            case 3:
                                c3[index++] = line[i];
                                c3[index] = '\0';
                                break;
                        }
                }
                this->insertNode(c1, c2, c3);
            }
        }

        int currentTableSize() { return current_size; }

        int collisionsCount() { return collisions; }

        float collisionsPercent() { return ((float)collisions / (float)current_size) * 100; }

        float idleSpace() { return ((float)(max - current_size) / (float)max) * 100; }
    };




int main(){
    HashTable* h = new HashTable(TABLE);

    cout << "Inserting Athletes.csv dataset to Hash Table.";

    h->insertDataset("Athletes.csv");

    while(true) {
        int opc;
        char c1[MAX], c2[MAX], c3[MAX];
        string s1, s2, s3;
        HashNode* aux;
        cout << "\n\n0 - End program;";
        cout << "\n1 - Insert Athlete in Hash Table;" ;
        cout << "\n2 - Search Athlete on Hash Table;";
        cout << "\n3 - Delete Athlete from Hash Table;";
        cout << "\n4 - Print Hash Table;";
        cout << "\n5 - Print Hash Table stats.";
        cout << "\n\nSelect option: ";
        cin >> opc;

        if (opc == 0)  break;

        else if (opc == 1){
            cout << "Type Athlete name to insert in Hash Table: ";
            cin >> s1;
            strncpy(c1, s1.c_str(), MAX);

            cout << "Type Athlete country to insert in Hash Table: ";
            cin >> s2;
            strncpy(c2, s2.c_str(), MAX);

            cout << "Type Athlete discipline to insert in Hash Table: ";
            cin >> s3;
            strncpy(c3, s3.c_str(), MAX);

            if(h->insertNode(c1, c2, c3)) cout << "\nAthlete" << c1 << "inserted in table in table.";

            else cout << "\nCan not insert in table or table is full.";

        } else if (opc == 2){
            cout << "Type Athlete name to insert in Hash Table: ";
            cin >> s1;
            strncpy(c1, s1.c_str(), MAX);

            aux = h->searchNode(c1);
            if(aux == NULL) cout << "\nAthlete not in table.";

            else cout << "\nAthlete " << aux->name << " found.";
            
        } else if (opc == 3) {
            cout << "Type Athlete name to delete from Hash Table: ";
            cin >> s1;
            strncpy(c1, s1.c_str(), MAX);

            aux = h->deleteNode(c1);
            if(aux == NULL) cout << "\nAthlete not in table.";

            else cout << "\nAthlete " << aux->name << " removed.";
            
        } else if (opc == 4) {
            cout << "Printing entire table:\n";
            h->printTable();
        }

        else if (opc == 5) {
            cout << "\nPrinting results:\n";
            cout << "Current table size: " << h->currentTableSize() << ".\n";
            cout << "Max table size: " << TABLE << ".\n";
            cout << "Number of collisions: " << h->collisionsCount() << ".\n";
            cout << "Percentage of collisions per table size: " << h->collisionsPercent() << "%.\n";
            cout << "Percentage of idle space: " << h->idleSpace() << "%.";
        }
    }

    return 0;
}