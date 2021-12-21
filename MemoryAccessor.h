#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>

typedef unsigned char byte;
typedef long long int64;

// 直接記憶體操作工具
template <class C>
class MemoryAccessor {
private:
protected:
	byte* start;
	int position;

	static void outputMemory(std::ostream& output, byte* start, byte* end) {
		for (byte* i = start; i < end; ++i) {
			output << std::setfill('0') << std::setw(2) << std::uppercase << std::hex << (short)*i;
			if (i < end - 1) {
				output << " ";
			}
		}
	}
public:
	MemoryAccessor(C* target) {
		start = (byte*)target;
		position = 0;
	}

	virtual int getPosition() {
		return position;
	}

	virtual byte* getStartLocation() {
		return start;
	}

	virtual byte* getCurrentLocation() {
		return start + position;
	}

	virtual void reset() {
		position = 0;
	}

	template <typename T>
	T readRaw() {
		T ret = *(T*)(getCurrentLocation());
		position += sizeof(T);
		return ret;
	}

	template <typename T>
	void unRead() {
		position -= sizeof(T);
	}

	virtual byte readByte() {
		return readRaw<byte>();
	}

	virtual short readShort() {
		return readRaw<short>();
	}

	virtual int readInt() {
		return readRaw<int>();
	}

	virtual float readFloat() {
		return readRaw<float>();
	}

	virtual int64 readLong() {
		return readRaw<int64>();
	}

	virtual double readDouble() {
		return readRaw<double>();
	}

	virtual const char* readCString(int buffer = 0) {
		char* ret;
		const char* location = (const char*)getCurrentLocation();
		ret = _strdup(location);

		int length = strlen(location) + 1;
		position += length;
		if (length < buffer) {
			position += buffer - length;
		}
		return ret;
	}

	virtual byte* read(int number) {
		byte* ret = new byte[number];
		for (int i = 0; i < number; ++i) {
			ret[i] = readByte();
		}
		return ret;
	}

	virtual void unReadByte() {
		unRead<byte>();
	}

	virtual void unReadShort() {
		unRead<short>();
	}

	virtual void unReadInt() {
		unRead<int>();
	}

	virtual void unReadLong() {
		unRead<int64>();
	}

	virtual void unRead(int number) {
		position -= number;
	}

	virtual void skip(int number) {
		position += number;
	}

	virtual void seek(int value) {
		position = value;
	}

	template <typename T>
	void writeRaw(T value) {
		*(T*)(getCurrentLocation()) = value;
		position += sizeof(T);
	}

	virtual void write(byte* value, int length) {
		for (int i = 0; i < length; ++i) {
			writeRaw(value[i]);
		}
	}

	virtual void writeBoolean(bool value) {
		writeRaw(value);
	}

	virtual void writeByte(byte value) {
		writeRaw(value);
	}

	virtual void writeShort(short value) {
		writeRaw(value);
	}

	virtual void writeInt(int value) {
		writeRaw(value);
	}

	virtual void writeFloat(float value) {
		writeRaw(value);
	}

	virtual void writeLong(int64 value) {
		writeRaw(value);
	}

	virtual void writeDouble(double value) {
		writeRaw(value);
	}

	virtual void writeCString(const char* value, int buffer = 0) {
		int length = strlen(value) + 1;
		write((byte*)value, length);

		if (length < buffer) {
			int fill = buffer - length;
			writeZeroByte(fill);
		}
	}

	virtual void writeSame(byte value, int length) {
		for (int i = 0; i < length; ++i) {
			writeByte(value);
		}
	}

	virtual void writeZeroByte(int length = 1) {
		writeSame(0, length);
	}

	virtual std::string toString() {
		std::stringstream now;
		std::stringstream all;

		int length = sizeof(C);
		byte* end = start + length;
		byte* location = getCurrentLocation();
		outputMemory(now, location, end);
		outputMemory(all, start, end);

		std::stringstream ret;
		ret << "全部 : " << all.str();
		std::string nowData = now.str();
		if (nowData != "") {
			ret << std::endl << "目前 : " << nowData;
		}
		return ret.str();
	}
};