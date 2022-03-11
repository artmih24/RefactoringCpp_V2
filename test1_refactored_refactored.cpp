#include <iostream>  
#include <cstdlib> // для system  
using namespace std;  
 
int add(int a, int b) {  
	return a + b; 
}  
 
int getTen(int a) {  
	int new_var_a = a;  
	new_var_a = 10;  
	return new_var_a; 
}  
 
int add2(int a) {  
	int new_var_a = a;  
	new_var_a += 2;  
	return new_var_a; 
}  
 
int getFive() {  
	return 5; 
}  
 
void func(int x) {  
	int new_var_x = x;  
	int m = new_var_x;  
	new_var_x = 22;  
	new_var_x = m; 
}  
 
int mul2(int a) {  
	int new_var_a = a;  
	new_var_a = 2;  
	return add(new_var_a, new_var_a); 
}  
 
int mul2v2(int a) {  
	int new_var_a = a;  
	int res = add(new_var_a, new_var_a);  
	new_var_a = 3;  
	return res; 
}  
 
int sqr(int a) {  
	return a * a; 
}  
 
int rec(int x) {  
	if (x == 1)  
		return 1;  
	else  
		return rec(x - 1); 
}  
 
int rec_ref(int x) {  
	if (x == 1)  
		return 1;  
	else  
		return rec(x - 1); 
}  
 
int rec_new(int x) {  
	if (x == 1)  
		return 1;  
	else  
		return rec_new(x - 1); 
}  
 
int rec1(int x) {  
	if (x == 1)  
		return 1;  
	else  
		return rec(x - 1); 
}  
 
int rec1_new(int x) {  
	if (x == 1)  
		return 1;  
	else  
		return rec(x - 1); 
}  
 
int rec2(int x, int y) {  
	int z = y;  
	if (x == 1)  
		return 1;  
	else  
		return rec2(x - 1, y); 
}  
 
int main() {  
	int a = sqr(2);  
	int b = getFive();  
	int c = rec(5);  
	cout << "Hello, world!" << endl;  
	system("pause"); // Только для тех, у кого MS Visual Studio  
	return 0; 
}  
 
  
