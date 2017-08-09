#pragma once

#include <memory>

template <class T, int N>
class PeekQueue
{
public:
	PeekQueue();

	std::shared_ptr<T> pop();
	bool push(std::shared_ptr<T> item);

	std::shared_ptr<T> peek(size_t index) const;

private:
	std::shared_ptr<T> items[N];

	void incrIndex(bool firstOrLast);
	void decrIndex(bool firstOrLast);

	int first;
	int last;
};

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

	if (first == last)
	{
		first = -1;
		last = -1;
	}
	else incrIndex(true);

	return ret;
}

template<class T, int N>
bool PeekQueue<T, N>::push(std::shared_ptr<T> item)
{
	incrIndex(false);
	if (last == first)
	{
		decrIndex(false);
		return false;
	}

	items[last] = item;

	if (first == -1) incrIndex(true);

	return true;
}

template<class T, int N>
std::shared_ptr<T> PeekQueue<T, N>::peek(size_t index) const
{
	if (first == -1) return nullptr;

	return items[(first + index) % N];
}

template<class T, int N>
void PeekQueue<T, N>::incrIndex(bool firstOrlast)
{
	int& index = firstOrlast ? first : last;

	if (++index == N) index = 0;
}

template<class T, int N>
void PeekQueue<T, N>::decrIndex(bool firstOrLast)
{
	int& index = firstOrLast ? first : last;

	if (--index < 0) index = N;
}


