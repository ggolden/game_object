#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

class GameObject {
public:
    GameObject() { cout << "+GameObject: " << this << endl; }

    virtual ~GameObject() { cout << "-GameObject: " << this << endl; };

    virtual bool isValuable() { return false; }

    virtual int value() { return 0; }

    virtual bool isTakable() { return false; }

    virtual bool isDamaging() { return false; }

    virtual int damageLevel() { return 0; }

    virtual bool isBlocking() { return false; }

    virtual string kind() { return "GameObject"; }
};

class Key : public GameObject {
public:
    Key() { cout << "+Key: " << this << endl; }

    ~Key() override { cout << "-Key: " << this << endl; }

    string kind() override { return "Key"; }

    bool isTakable() override { return true; }
};

class Coin : public GameObject {
    int coinValue;
public:
    Coin(int value) : coinValue(value) { cout << "+Coin: " << this << endl; }

    ~Coin() override { cout << "-Coin: " << this << endl; }

    bool isValuable() override { return true; }

    int value() override { return coinValue; }

    string kind() override { return "Coin"; }

    bool isTakable() override { return true; }
};

class Thornbush : public GameObject {
    int damageLevelValue;
public:
    explicit Thornbush(int damageLevel) : damageLevelValue(damageLevel) { cout << "+Thornbush: " << this << endl; }

    ~Thornbush() override { cout << "-Thornbush: " << this << endl; }

    bool isDamaging() override { return true; }

    int damageLevel() override { return damageLevelValue; }

    bool isBlocking() override { return true; }

    string kind() override { return "Thornbush"; }
};

class Wall : public GameObject {
public:
    explicit Wall() { cout << "+Wall: " << this << endl; }

    ~Wall() override { cout << "-Wall: " << this << endl; }

    bool isBlocking() override { return true; }

    string kind() override { return "Wall"; }
};

int main() {
    {
        cout << "start block 1" << endl;
        GameObject game;
        Key key;
        Coin coin{22};
        cout << "end block 1" << endl;
    }

    {
        cout << endl << "start block 2" << endl;
        Key key;
        Coin coin{22};
        if (key.isValuable()) {
            cout << "key has value: " << key.value() << endl;
        } else {
            cout << "key no value" << endl;
        }

        if (coin.isValuable()) {
            cout << "coin has value: " << coin.value() << endl;
        } else {
            cout << "coin has no value" << endl;
        }
        cout << "end block 2" << endl;
    }

    {
        cout << endl << "start block 3" << endl;
        Key key;
        Coin coin{22};
        GameObject &o1 = coin;
        GameObject &o2 = key;

        if (o1.isValuable()) {
            cout << "o1 has value: " << o1.value() << endl;
        } else {
            cout << "o1 no value" << endl;
        }

        if (o2.isValuable()) {
            cout << "o2 has value: " << o2.value() << endl;
        } else {
            cout << "o2 no value" << endl;
        }

        cout << "end block 3" << endl;
    }

    {
        cout << endl << "start block 4" << endl;
        vector<GameObject *> objects;
        objects.push_back(new Key);
        objects.push_back(new Coin{22});

        for (auto object: objects) {
            if (object->isValuable()) {
                cout << "object has value: " << object->value() << endl;
            } else {
                cout << "object no value" << endl;
            }
        }
        cout << "end block 4" << endl;
    }

    {
        cout << endl << "start block 5" << endl;
        vector<unique_ptr<GameObject>> objects;
        objects.push_back(unique_ptr<GameObject>{new Key});
        objects.push_back(unique_ptr<GameObject>{new Coin{22}});

        for (unique_ptr<GameObject> &object: objects) {
            if (object->isValuable()) {
                cout << "object " << object->kind() << " has value: " << object->value() << endl;
            } else {
                cout << "object " << object->kind() << " has no value" << endl;
            }
        }
        cout << "end block 5" << endl;
    }

    {
        cout << endl << "start block 6" << endl;
        vector<unique_ptr<GameObject>> objects;
        objects.push_back(unique_ptr<GameObject>{new Key});
        objects.push_back(unique_ptr<GameObject>{new Coin{22}});
        objects.push_back(unique_ptr<GameObject>{new Thornbush{2}});
        objects.push_back(unique_ptr<GameObject>{new Wall});

        for (unique_ptr<GameObject> &object: objects) {
            if (object->isTakable()) {
                cout << "object " << object->kind() << " is takable" << endl;
            } else {
                cout << "object " << object->kind() << " not takable" << endl;
            }
        }

        for (unique_ptr<GameObject> &object: objects) {
            if (object->isDamaging()) {
                cout << "object " << object->kind() << " is damaging: " << object->damageLevel() << endl;
            } else {
                cout << "object " << object->kind() << " not damaging" << endl;
            }
        }

        for (unique_ptr<GameObject> &object: objects) {
            if (object->isBlocking()) {
                cout << "object " << object->kind() << " is blocking" << endl;
            } else {
                cout << "object " << object->kind() << " not blocking" << endl;
            }
        }
        cout << "end block 6" << endl;
    }
}
