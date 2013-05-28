#include <iostream>
#include <cstdio>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cassert>
#include "LinkedList.h"
using namespace std;

struct DynamicPart{
    DynamicPart(int x = 0):
        num(x), ok(){ ++Balance; ok[0] = ok[1] = true; }
    DynamicPart(const DynamicPart& rhs):num(rhs.num){ ++Balance; memcpy(ok, rhs.ok ,sizeof(bool) * 2); }
    ~DynamicPart() { --Balance; }

    int num;
    bool ok[2];

    static int Balance;
};

int DynamicPart::Balance = 0;

struct BasePart{
    string name;
    BasePart():name(){ ++Balance; }
    BasePart(const string& str):name(str){ ++Balance; }
    BasePart(const BasePart& rhs):name(rhs.name){ ++Balance; }

    virtual ~BasePart(){ --Balance; }

    virtual ostream& PrintName(ostream& out){
        return out << name;
    }

    static int Balance;
};

int BasePart::Balance = 0;

struct Derived:public BasePart{
    DynamicPart* dp;
    double extended;
    Derived():BasePart(), dp(new DynamicPart()), extended(){ ++Balance; }
    Derived(const Derived& rhs):BasePart(rhs), dp(new DynamicPart(*(rhs.dp))), extended(rhs.extended){ ++Balance; }
    Derived(const string& str):BasePart(str), dp(new DynamicPart()), extended(){ ++Balance; }

    Derived& operator=(const Derived& rhs){
        if (&rhs == this) return *this;
        BasePart::operator=(rhs);
        *dp = *(rhs.dp);
        extended = rhs.extended;
        return *this;
    }

    bool operator==(const Derived& rhs) const{
        return  (extended == rhs.extended) &&
                 (dp->num == rhs.dp->num) && (dp->ok[0] == rhs.dp->ok[0])
                && (dp->ok[1] == rhs.dp->ok[1]);
    }

    ~Derived(){ --Balance; delete dp; }

    static int Balance;
};

istream& operator>>(istream& in, Derived& d){
    return in >> d.extended >> d.dp->num >> d.dp->ok[0] >> d.dp->ok[1];
}

ostream& operator<<(ostream& os, const Derived& d){
    return os << d.name;
}

int Derived::Balance = 0;

struct BalanceWatcher{
    BalanceWatcher(){
        checkBalanceZero();
    }

    static void checkBalanceZero(){
        assert(DynamicPart::Balance == 0);
        assert(BasePart::Balance == 0);
        assert(Derived::Balance == 0);
    }

    static ostream& printBalance(ostream& out){
        return out << "Balance status: DynamicPart == " << DynamicPart::Balance
                   << " BasePart == " << BasePart::Balance
                   << " Derived == " << Derived::Balance << endl;
    }

    ~BalanceWatcher(){
        printBalance(cout);
        checkBalanceZero();
    }

} globalBalanceWatcher;

void print(LinkedList<Derived> a)
{
	LinkedList<Derived> b;
	Derived value("");
	b.add(value);
	cout << "Get & Size:           ";
	for (int i=0; i<a.size(); ++i)
	 cout << ' ' << a.get(i);
	cout << endl;

	cout << "Iterator:             ";
	LinkedList<Derived>::Iterator itr=a.iterator();
	while (itr.hasNext()) cout << ' ' << itr.next();
	cout << "   empty = " ;
	if (a.isEmpty())
	 cout << "yes" << endl;
	else
	 cout << "no" << endl;

	cout << "Operator= <G&S>       ";
	b=a;
	for (int i=0; i<b.size(); ++i)
	 cout << ' ' << b.get(i);
	cout << endl;

	cout << "Operator= <ITR>       ";
	itr=b.iterator();
	while (itr.hasNext()) cout << ' ' << itr.next();
	cout << "   empty = " ;
	if (b.isEmpty())
	 cout << "yes" << endl;
	else
	 cout << "no" << endl;

	cout << "Operator= <G&S>       ";
	LinkedList<Derived> c(a);
	for (int i=0; i<c.size(); ++i)
	 cout << ' ' << c.get(i);
	cout << endl;

	cout << "Copy-constructor <ITR>";
	itr=c.iterator();
	while (itr.hasNext()) cout << ' ' << itr.next();
	cout << "   empty = " ;
	if (c.isEmpty())
	 cout << "yes" << endl;
	else
	 cout << "no" << endl;

	cout << "Get First:  " << a.getFirst() << "\nGet Last:   " << a.getLast() << endl;
}

int main()
{
	freopen("input.txt","r",stdin);
	freopen("output.txt","w",stdout);
	string command;
	char ch;
	string x;
	long y,z;
	{
	LinkedList<Derived> *a1 = new LinkedList<Derived>;
	LinkedList<Derived> a = *a1;
	LinkedList<Derived>::Iterator itr = a.iterator();
	while (1)
	{
	try
	 {
	 	cout << endl;
	 	cin >> command;
	 	if (command == "add")
	 	 {
	 	 	cin >> x;
	 	 	a.add(x);

	 	 }
	 	else if (command=="addfirst")
	 	 {
	 	 	cin >> x;
	 	 	a.addFirst(x);
	 	 }
	 	else if (command=="addlast")
	 	 {
	 	 	cin >> x;
	 	 	a.addLast(x);
	 	 }
	 	else if (command=="addindex")
	 	 {
	 	 	cin >> y >> x;
	 	 	a.add(y,x);
	 	 }
	 	else if (command=="clear")
	 	 a.clear();
	 	else if (command=="contains")
	 	 {
	 	 	cin >> x;
	 	 	if (a.contains(x))
	 	 	 cout << "yes" << endl;
	 	 	else
	 	 	 cout << "no" << endl;
	 	 }
	 	else if (command=="removeindex")
	 	 {
	 	 	cin >> y ;
	 	 	a.removeIndex(y);
	 	 }
	 	else if (command=="removefirst")
	 	 a.removeFirst();
	 	else if (command=="removelast")
	 	 a.removeLast();
	 	else if (command=="remove")
	 	 {
	 	 	cin >> x ;
	 	 	if (a.remove(x)) cout << "success" << endl;
	 	 	else cout << "failed" << endl;
	 	 }
	 	else if (command=="set")
	 	 {
	 	 	cin >> y >> x;
	 	 	a.set(y,x);
	 	 }
	 	else if (command=="new")
	 	 itr = a.iterator();
	 	else if (command=="hasnext")
	 	 cout << ((itr.hasNext())?"yes":"no") << endl;
	 	else if (command=="next")
	 	 cout << (itr.next()) << endl;
	 	else if (command=="removeitr")
	 	 itr.remove();
	 	else if (command=="print")
	 	 print(a);
		else
		 break;
	  }
	 catch (IndexOutOfBound error) { cout << "IndexOutOfBound" << endl; }
	 catch (ElementNotExist error) { cout << "ElementNotExist" << endl; }
	}
	delete a1;
	}
	//------check memory leak------
	globalBalanceWatcher.checkBalanceZero();
	return 0;
}

