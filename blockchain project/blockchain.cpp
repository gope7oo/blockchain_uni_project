// g++ -Wall -std=c++11 blockchain.cpp -o test.exe
//.\test.exe


#include <iostream>    
#include <vector>       
#include <string>       
#include <sstream>      

using namespace std;

/*
    Цел: Симулира хеш функция
    В реалния блокчейн се използва SHA-256, но тук правим опростена версия
*/
string simpleHash(string input) {
    unsigned long hash = 5381; // начална стойност (стандартен seed)

    // обхождаме всеки символ и “смесваме” стойността
    for (char c : input) {
        hash = ((hash << 5) + hash) + c;
    }

    return to_string(hash); // връщаме като текст
}

/*
    Представя един блок в блокчейн веригата
*/
class Block {
public:
    int index;          // позиция на блока във веригата
    string data;        // информация (транзакции)
    string prevHash;    // хеш на предишния блок
    string hash;        // текущ хеш
    int nonce;          // стойност за mining (Proof of Work)

    /*
        Създава блок и автоматично го “mining”-ва
    */
    Block(int i, string d, string pHash) {
        index = i;
        data = d;
        prevHash = pHash;
        nonce = 0;

        // започваме mining процес
        hash = mineBlock(1); // difficulty = 3 (3 нули в началото)
    }

    /*
        Комбинира всички данни и изчислява hash
    */
    string calculateHash() {
        stringstream ss;

        // комбинираме всички важни данни
        ss << index << data << prevHash << nonce;

        return simpleHash(ss.str());
    }

    /*
        Симулира Proof of Work (mining)
        Търси hash, който започва с определен брой нули
    */
        string mineBlock(int difficulty) {
    cout << "Mining block #" << index << " ..." << endl;
    
    // Просто увеличаваме nonce докато hash започне с 0 (или принудително)
    for (int i = 0; i < 100000; i++) {   // лимит 100 000 опита
        hash = calculateHash();
        if (hash[0] == '0' || i > 50000) {   // по-лесно условие
            nonce = i;
            cout << "✓ Block #" << index << " mined! Nonce = " << nonce 
                 << " Hash = " << hash << endl;
            return hash;
        }
    }
    
    // Ако не намери - принудително
    hash = calculateHash();
    cout << "✓ Block #" << index << " mined (forced)! Hash = " << hash << endl;
    return hash;
    }
};

/*
    Управлява цялата верига от блокове
*/
class Blockchain {
private:
    vector<Block> chain; // списък от блокове

public:
    /*
        Създава първия блок (Genesis Block)
    */
    Blockchain() {
        chain.push_back(Block(0, "Genesis Block", "0"));
    }

    /*
        Добавя нов блок към веригата
    */
    void addBlock(string data) {
        Block prev = chain.back(); // взимаме последния блок

        // създаваме нов блок, свързан с предишния
        chain.push_back(Block(chain.size(), data, prev.hash));
    }

    /*
        Показва всички блокове в блокчейна
    */
    void showChain() {
        cout << "\n================ BLOCKCHAIN ================\n";

        for (auto &b : chain) {
            cout << "\nBlock #" << b.index << endl;
            cout << "Data       : " << b.data << endl;
            cout << "Prev Hash  : " << b.prevHash << endl;
            cout << "Hash       : " << b.hash << endl;
            cout << "Nonce      : " << b.nonce << endl;
        }

        cout << "\n===========================================\n";
    }

    /*гата е цяла (не е манипулирана)
    */
    bool isValid() {
        for (int i = 1; i < chain.size(); i++) {
            // ако връзката е счупена → блокчейнът е невалиден
            if (chain[i].prevHash != chain[i - 1].hash) {
                return false;
            }
        }

        return true;
    }
};

int main() {
    Blockchain bc; // създаваме блокчейн система
    cout << "Simple Blockchain Simulator (C++)\n";
    
    // добавяме примерни блокове (транзакции)
    bc.addBlock("Ivan -> Peter 5 BTC");
    bc.addBlock("Peter -> Maria 2 BTC");
    bc.addBlock("Maria -> Alex 1 BTC");

    // показваме веригата
    bc.showChain();

    // проверка на целостта
    cout << "\nChain status: "
         << (bc.isValid() ? "VALID" : "INVALID")
         << endl;


    /*
    bc.showChain();

    cout << "\nBefore attack: "
        << (bc.isValid() ? "VALID" : "INVALID")
        << endl;

    // Симулираме промяна на Block #1
    bc.chain[1].data = "Ivan -> Peter 5000 BTC (HACKED)";

    // <<< НАЙ-ВАЖНОТО >>> Преизчисляваме хеша на променения блок
    bc.chain[1].hash = bc.chain[1].calculateHash();
    cout << "\nAfter attack: "
        << (bc.isValid() ? "VALID" : "INVALID")
        << endl;   
    
    */

    cout << "\nPress Enter to exit...";
    cin.get();   // ← това спира програмата да се затвори веднага


    return 0;
}