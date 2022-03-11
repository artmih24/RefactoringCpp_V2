#include <iostream>
#include <cstdlib> // для system
using namespace std;

int add(int a, int b) {
	return a + b;
}

int getTen(int a) {
	a = 10;
	return a;
}

int add2(int a) {
	a += 2;
	return a;
}

int getFive(int a) {
	return 5;
}

void func(int x) {
	int m = x;
	x = 22;
	x = m;
}

int mul2(int a) {
	a = 2;
	return add(a, a);
}

int mul2v2(int a) {
	int res = add(a, a);
	a = 3;
	return res;
}

int sqr(int a, int b) {
	return a * a;
}

int rec(int x, int y) {
	if (x == 1)
		return 1;
	else
		return rec(x - 1, y);
}

int rec_ref(int x) {   
	if (x == 1)  
		return 1;  
	else  
		return rec(x - 1, y); 
}  

int rec_new(int x) {   
	if (x == 1)  
		return 1;  
	else  
		return rec_new(x - 1, y); 
}  

int rec1(int x, int y) {
	if (x == 1)
		return 1;
	else
		return rec(x - 1, y + 1);
} 

int rec1_new(int x, int y) {  
	if (x == 1)  
		return 1;  
	else  
		return rec(x - 1); 
}  

int main() { 
	int a = sqr(2, 3);
	int b = getFive(a);
	int c = rec(5, 2);
	cout << "Hello, world!" << endl;
	system("pause"); // Только для тех, у кого MS Visual Studio
	return 0; 
}