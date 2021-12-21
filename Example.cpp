#include <iostream>
#include "MemoryAccessor.h"

using namespace std;

class Example {
private:
	int a = 123;
	char b[15] = "Meow";
	int c = 456;
	float d = 12.34F;
	short e = 789;
protected:
public:
	virtual void print() {
		cout << "a = " << a << endl;
		cout << "b = " << b << endl;
		cout << "c = " << c << endl;
		cout << "d = " << d << endl;
		cout << "e = " << e << endl;
	}
};

int main() {
	Example example;
	example.print();
	cout << "大小 = " << sizeof(example) << endl << endl;

	MemoryAccessor<Example> read(&example);
	cout << "讀取範例" << endl;
	read.skip(4); // 虛擬函式標頭
	cout << "a = " << read.readInt() << endl;

	const char* readString = read.readCString(15);
	cout << "b = " << readString << endl;
	free((void*)readString); // 釋放

	read.skip(1); // 記憶體對齊
	cout << "c = " << read.readInt() << endl;
	cout << "d = " << read.readFloat() << endl;
	cout << "e = " << read.readShort() << endl;
	cout << read.toString() << endl << endl;

	MemoryAccessor<Example> write(&example);
	cout << "寫入範例" << endl;
	write.skip(4);
	write.writeInt(99);
	write.writeCString("Hello World", 15);
	write.skip(1);
	write.writeInt(80923);
	write.writeFloat(33.33F);
	write.writeShort(32767);
	example.print();

	system("pause");
	return EXIT_SUCCESS;
}