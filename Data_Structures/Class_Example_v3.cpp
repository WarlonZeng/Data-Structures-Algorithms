#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Pet {
	string name;
	int age;
	string color;
	vector<Pet*> friends;
public:
	Pet() { age = 0; }
	void addYear() { age++; }
	string getName()const { return name; }
	void setName(string newName) { name = newName; }
	int getAge()const { return age; }
	void speak()const { cout << "Meow, woof, or whatever."; }
	void paintPet(string newcolor) { color = newcolor; }
	string getColor()const { return color; }
	bool isFriend(Pet* otherPet)const;
	void makeFriend(Pet* otherPet);
	void printFriendNames()const;
};

bool Pet::isFriend(Pet* otherPet) const {
	for (int i = 0; i < friends.size(); i++)
		if (otherPet == friends[i])
			return true;
	return false;
}

void Pet::printFriendNames() const {
	for (int i = 0; i < friends.size(); i++)
		cout << friends[i]->name << endl;
}

void Pet::makeFriend(Pet* otherPet) {
	if (isFriend(otherPet)) //test for stupidity
		return;
	friends.push_back(otherPet);
	otherPet->friends.push_back(this);
}

int main() {
	Pet a, b, c, d;
	a.setName("Honey");
	b.setName("Daisy");
	c.setName("Emma");
	d.setName("Rosie");

	a.makeFriend(&b);
	cout << "We made " << a.getName() << " a friend of " << b.getName() << "..." << endl;
	cout << a.getName() << "\'s friends: ";
	a.printFriendNames();

	cout << "-------------------------------------------" << endl;
	cout << b.getName() << "\'s friends: ";
	b.printFriendNames();

	b.makeFriend(&c);
	cout << "We made " << b.getName() << " a friend of " << c.getName() << "..." << endl;
	cout << b.getName() << "\'s friends: ";
	b.printFriendNames();

	cout << "-------------------------------------------" << endl;
	cout << c.getName() << "\'s friends: ";
	c.printFriendNames();

}