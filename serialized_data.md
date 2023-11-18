## Working with serialized data
deserialization:
* DONT_1: struct to define data structure, allocate struct object, cast it to (char*) and memcpy/read buffer into the pointed location <-- compiler can do padding for non word length data members and wrongly offset the read data (solution can be #pragma pack(1) // gcc only )
* DONT_2: cast (reinterpret_cast) the read buffer data into the struct <-- can be undefined behaviour, see cppreference: reinterpret_cast
* POSIB_1: like reinterpret_cast but with bit_cast (C++20)