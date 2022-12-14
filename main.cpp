#include <iostream>
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
    explicit Coin(int value) : coinValue(value) { cout << "+Coin: " << this << endl; }

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
    // objects on the stack, created and then destroyed by scope
    {
        cout << "start block 1" << endl;
        GameObject game;
        Key key;
        Coin coin{22};
        cout << "end block 1" << endl;
    }

    // specific class calls work
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

    // class polymorphism (GameObject&) works
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

    // storing polymorphic objects in a base class array
    // note the warnings about "slicing" ... slicing = reducing a subclass to a base class.
    {
        cout << endl << "start block 4" << endl;
        GameObject objects[3];
        cout << "... objects[3] created" << endl;
        objects[0] = GameObject{};
        objects[1] = Key{};
        objects[2] = Coin{22};

        for (auto object: objects) {
            cout << object.kind() << endl;
        }

        cout << "end block 4" << endl;
    }

    // explore a vector's array part re: memory management
    {
        cout << endl << "start block 5" << endl;
        vector<GameObject> objects(3);
        cout << "...vector created" << endl;

        cout << "end block 5" << endl;
    }

    // (2) explore a vector's array part re: memory management
    {
        cout << endl << "start block 6" << endl;
        vector<GameObject *> objects(3);
        cout << "...vector created" << endl;

        cout << "end block 6" << endl;
    }

    // vectors of GameObjects are just GameObjects, not the full specific class
    // Note: some additional GameObjects are being destroyed .. the vector's GameObject array?
    // each push_back creates and destroys the Key or Coin
    {
        cout << endl << "start block 7" << endl;
        vector<GameObject> objects;
        cout << "... vector<GameObject> created" << endl;
        objects.push_back(Key{});
        objects.push_back(Coin{22});

        for (auto object: objects) {
            if (object.isValuable()) {
                cout << "object " << object.kind() << " has value: " << object.value() << endl;
            } else {
                cout << "object " << object.kind() << " no value" << endl;
            }
        }
        cout << "objects.capacity() " << objects.capacity() << endl;
        cout << "end block 7" << endl;
    }

    // vector holds pointers to the actual new Key and Coin, but those objects are never deleted
    {
        cout << endl << "start block 8" << endl;
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
        cout << "end block 8" << endl;
    }

    // vector holds pointers to the actual new Key and Coin, deleted manually
    {
        cout << endl << "start block 9" << endl;
        vector<GameObject *> objects;
        objects.push_back(new Key);
        objects.push_back(new Coin{22});

        for (auto object: objects) {
            if (object->isValuable()) {
                cout << "object has value: " << object->value() << endl;
            } else {
                cout << "object not valuable" << endl;
            }
        }

        for (auto object: objects) {
            delete object;
        }

        cout << "end block 9" << endl;
    }

    // unique_ptr takes care of managing the Key and Coin deletes
    {
        cout << endl << "start block 10" << endl;
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
        cout << "end block 10" << endl;
    }

    // full polymorphic treatment of GameObject
    {
        cout << endl << "start block 11" << endl;
        vector<unique_ptr<GameObject>> objects;
        objects.push_back(unique_ptr<GameObject>{new Key});
        objects.push_back(unique_ptr<GameObject>{new Coin{22}});
        objects.push_back(unique_ptr<GameObject>{new Thornbush{2}});
        objects.push_back(unique_ptr<GameObject>{new Wall});

        for (auto &object: objects) {
            if (object->isTakable()) {
                cout << "object " << object->kind() << " is takable" << endl;
            } else {
                cout << "object " << object->kind() << " not takable" << endl;
            }
        }

        for (auto &object: objects) {
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
        cout << "end block 11" << endl;
    }

    // how to remove a unique_ptr<> from a vector (disappear from Level, like door)
    // pop_back version
    {
        cout << endl << "start block 12" << endl;
        vector<unique_ptr<GameObject>> objects;
        objects.push_back(unique_ptr<GameObject>{new Coin{22}});
        objects.push_back(unique_ptr<GameObject>{new Key});
        cout << "pop_back()" << endl;
        objects.pop_back();
        cout << "end block 12" << endl;
    }

    // how to remove a unique_ptr<> from a vector (disappear from Level, like door)
    // erase() at an index (i.e. begin + index) version
    {
        cout << endl << "start block 13" << endl;
        vector<unique_ptr<GameObject>> objects;
        objects.push_back(unique_ptr<GameObject>{new Coin{22}});
        objects.push_back(unique_ptr<GameObject>{new Key});
        cout << "erase(begin+1)" << endl;
        objects.erase(objects.begin() + 1);
        cout << "end block 13" << endl;
    }

    // erase() a range from begin to just before the last
    {
        cout << endl << "start block 14" << endl;
        vector<unique_ptr<GameObject>> objects;
        objects.push_back(unique_ptr<GameObject>{new Coin{22}});
        objects.push_back(unique_ptr<GameObject>{new Key});
        objects.push_back(unique_ptr<GameObject>{new Thornbush{2}});
        cout << "erase(begin ..] begin+2)" << endl;
        objects.erase(objects.begin(), objects.begin() + 2);
        cout << "end block 14" << endl;
    }

    // accessing an item from the vector
    // has to be by reference if using unique_ptr, because they don't copy
    // ownership of the GameObject does not transfer
    {
        cout << endl << "start block 15" << endl;
        vector<unique_ptr<GameObject>> objects;
        objects.push_back(unique_ptr<GameObject>{new Coin{22}});
        objects.push_back(unique_ptr<GameObject>{new Key});
        objects.push_back(unique_ptr<GameObject>{new Thornbush{2}});

        // unique_ptr<GameObject> object = objects[1];
        // ^^^ does not compile, requires copying the unique_ptr, which is disallowed by the class

        auto &object = objects[1];
        cout << "[1] " << object->kind() << endl;

        cout << "end block 15" << endl;
    }

    // how to move a unique_ptr<> from one vector into another (i.e. level to inventory)
    // std::move(some unique pointer ref) - will transfer ownership of the item in the unique_ptr to another
    // this leaves a hole (unique_ptr with nullptr) in the original vector
    {
        cout << endl << "start block 16" << endl;

        vector<unique_ptr<GameObject>> objects;
        objects.push_back(unique_ptr<GameObject>{new Coin{22}});

        unique_ptr<GameObject> &coin = objects[0];

        vector<unique_ptr<GameObject>> objects2;
        objects2.push_back(std::move(coin));
        // ^^^ moves the ownership and object from original unique_ptr (objects, coin) to a new one (in objects2)
        // at this point, objects has a single unique_ptr, pointing at nullptr
        // coin is still pointing to the same unique_ptr from the vector, now pointing at nullptr
        // objects2 has the original Coin in its one unique_ptr item
        unique_ptr<GameObject> &coin2 = objects2[0];

        cout << "end block 16" << endl;
    }

    // full move from one vector to another
    {
        cout << endl << "start block 17" << endl;

        vector<unique_ptr<GameObject>> objects1;
        vector<unique_ptr<GameObject>> objects2;

        objects1.push_back(unique_ptr<GameObject>{new Coin{22}});
        objects1.push_back(unique_ptr<GameObject>{new Key{}});

        objects2.push_back(unique_ptr<GameObject>(new Thornbush{10}));

        cout << " ... before ..." << endl;
        for (auto &object: objects1) {
            cout << "1: " << object->kind() << endl;
        }
        for (auto &object: objects2) {
            cout << "2: " << object->kind() << endl;
        }

        // move through a temp
        unique_ptr<GameObject> tempCoin = std::move(objects1[0]);
        objects1.erase(objects1.begin() + 0);
        objects2.push_back(std::move(tempCoin));

        cout << " ... after move ..." << endl;
        for (auto &object: objects1) {
            cout << "1: " << object->kind() << endl;
        }
        for (auto &object: objects2) {
            cout << "2: " << object->kind() << endl;
        }

        cout << "end block 17" << endl;
    }

    // can we dynamic cast a unique_ptr<GameObject> to Coin or Key?
    {
        cout << endl << "start block 18" << endl;

        vector<unique_ptr<GameObject>> objects;
        objects.push_back(unique_ptr<GameObject>{new Key{}});
        objects.push_back(unique_ptr<GameObject>(new Thornbush{10}));
        objects.push_back(unique_ptr<GameObject>{new Coin{22}});

        for (auto &object: objects) {
            Thornbush *thornbush = dynamic_cast<Thornbush *>(object.get());
            if (thornbush) {
                cout << object->kind() << " is a thornbush " << thornbush->damageLevel() << endl;
            } else {
                cout << object->kind() << " is not a thornbush" << endl;
            }
        }

        cout << "end block 18" << endl;
    }
}
