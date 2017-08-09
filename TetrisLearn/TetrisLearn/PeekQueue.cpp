#include "PeekQueue.hpp"

template<class T, int N>
PeekQueue<T, N>::PeekQueue() : first(-1), last(-1)
{
	for (int i = 0; i < N; ++i) items[i] = nullptr;
}

template<class T, int N>
std::shared_ptr<T> PeekQueue<T, N>::pop()
{
	std::shared_ptr<T> ret;

	if (first == -1) return nullptr;

	ret = items[first];
	decrIndex(first);

	return ret;
}

template<class T, int N>
bool PeekQueue<T, N>::push(std::shared_ptr<T> item)
{
	incrIndex(last);
	if (last == first)
	{
		decrIndex(last);
		return false;
	}

	items[last] = item;
}

template<class T, int N>
std::shared_ptr<T> PeekQueue<T, N>::peek(int index) const
{
	if (first == -1) return nullptr;

	return items[first];
}

template<class T, int N>

