#pragma once

#include <stdexcept>

template<typename DataType, typename EnumType, int Size>

class EnumeratedArray
{
public:
	EnumeratedArray()
	{
		for (int i = 0; i < Size; ++i) dataArray[i] = DataType();
	}

	EnumeratedArray(std::initializer_list<DataType> data)
	{
		std::copy(data.begin(), data.end(), dataArray);
	}

	inline DataType& operator[](const EnumType index)
	{
		int intIndex = static_cast<int>(index);
		if (intIndex > Size) throw std::out_of_range("Enumerated Array");
		else return dataArray[intIndex];
	}

	inline const DataType& operator[](const EnumType index) const
	{
		int intIndex = static_cast<int>(index);
		if (intIndex > Size) throw std::out_of_range("Enumerated Array");
		else return dataArray[intIndex];
	}

private:
	DataType dataArray[Size];
};

// Partial specialization for Matrix.
// If this work as it is, consider me impressed.
template<typename DataType, typename EnumType, int Size, int internalSize> 
class EnumeratedArray<DataType[internalSize], EnumType, Size>
{
public:
	EnumeratedArray()
	{
		for (int i = 0; i < Size; ++i)
		{
			dataArray[i] = EnumeratedArray<DataType, int, internalSize>()
		}
	}

	EnumeratedArray(std::initializer_list<std::initializer_list<DataType>> data)
	{
		int i = 0;
		for (auto ilist : data)
		{
			dataArray[i] = EnumeratedArray<DataType, int, internalSize>(ilist);
			++i;
		}
	}

	inline EnumeratedArray<DataType, int, internalSize>& operator[](const EnumType index)
	{
		int intIndex = static_cast<int>(index);
		if (intIndex > Size) throw std::out_of_range("Enumerated Array");
		else return dataArray[intIndex];
	}

	inline const EnumeratedArray<DataType, int, internalSize>& operator[](const EnumType index) const
	{
		int intIndex = static_cast<int>(index);
		if (intIndex > Size) throw std::out_of_range("Enumerated Array");
		else return dataArray[intIndex];
	}

private:
	EnumeratedArray<DataType, int, internalSize> dataArray[Size];
};