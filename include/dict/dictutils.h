#ifndef DICTUTILS_H
#define DICTUTILS_H


#include <utility>
#include <cassert>


namespace Dict
{
	struct Word{
		const char* word = nullptr;
		size_t index = size_t(-1);
		
		constexpr size_t hashWord() const noexcept{
			const char* str = word;
			size_t hash = 0;
			while (*str)
				hash = hash * 1339157 + *str++;//15331037
			// hash = hash * 131;
			return hash;
			
		}
		
		constexpr size_t hashIndex() const noexcept{
			return index;//index*index * (2* 1339157) + index;
		}
		
	};
	
	//a < b	=	negative
	//a ==b =	0
	//a > b =	positive
	constexpr int compareWord(const Word& A, const Word &B) noexcept {
		const char* a = A.word;
		const char* b = B.word;
		//assert(a);
		//assert(b);
		while ((*a) && (*b))
		{
			if ((*a) != (*b))
				return (int)*a - (int)*b;
			++a; 
			++b;
		}
		return (int)*a - (int)*b;
	}
	
	//a < b	=	negative
	//a ==b =	0
	//a > b =	positive
	constexpr int compareIndex(const Word& A, const Word &B) noexcept {
		//assert(A.index != size_t(-1));
		//assert(B.index != size_t(-1));
		return (intptr_t)A.index - (intptr_t)B.index;
	}
	
	inline constexpr size_t HashTableIndex(size_t ghash, size_t i, size_t size) noexcept {
		return (ghash+i*16367179)% size;//
		//return (ghash + i*i*2*16367179+i) % size;//only for size == 2^n
	}
	
	constexpr size_t InsertWordIndex(Word dict[], size_t SIZE, const Word & word, size_t &misses) noexcept {
		size_t ghash = word.hashWord();
		
		for (size_t i = 0; i < SIZE; i++)
		{
			size_t hash = HashTableIndex(ghash, i, SIZE);
			if (dict[hash].word == nullptr)
			{
				dict[hash] = word;
				return hash;
			}
			else if (compareWord(dict[hash], word) == 0)	//already exist
			{
				return hash;
			}
			misses++;
		}
		//#ifdef NDEBUG
		//assert(false); // table is full or size is not power 2
		//#endif
		return size_t(-1);
	}
	
	constexpr size_t InsertIndexWord(Word dict[], size_t SIZE, const Word & word, size_t &misses) noexcept{
		size_t ghash = word.hashIndex();
		
		for (size_t i = 0; i < SIZE; i++)
		{
			size_t hash = HashTableIndex(ghash, i, SIZE);
			if (dict[hash].index == size_t(-1))
			{
				dict[hash] = word;
				return hash;
			}
			else if (compareIndex(dict[hash], word) == 0)	//already exist
			{
				return hash;
			}
			misses++;
		}
		//#ifdef NDEBUG
		//assert(false); // table is full or size is not power 2
		//#endif
		return size_t(-1);
	}
	
	constexpr size_t FindWordIndex(const Word dict[], size_t SIZE, const Word & word) noexcept {
		size_t ghash = word.hashWord();

		for (size_t i = 0; i < SIZE; i++)
		{
			size_t hash = HashTableIndex(ghash, i, SIZE);
			if (dict[hash].word == nullptr)
			{
				return size_t(-1);
			}
			else if (compareWord(dict[hash], word) == 0)//equal
			{
				return hash;
			}
			
		}
		//assert(false); // table is full or size is not power 2
		return size_t(-1);
	}
	
	constexpr size_t FindIndexWord(const Word dict[], size_t SIZE, const Word & word) noexcept {
		size_t ghash = word.hashIndex();

		for (size_t i = 0; i < SIZE; i++)
		{
			size_t hash = HashTableIndex(ghash, i, SIZE);
			if (dict[hash].index == size_t(-1))
			{
				return size_t(-1);
			}
			else if (compareIndex(dict[hash], word) == 0)//equal
			{
				return hash;
			}
			
		}
		//assert(false); // table is full or size is not power 2
		return size_t(-1);
	}
	
	
	
	
	
	
	template <size_t SIZE>
	struct Dictionary
	{
		static constexpr size_t size = SIZE;
		
		Word wtoi[SIZE];	//word to index hashmap
		Word itow[SIZE];	//index to word hashmap
		size_t count;
		size_t misses;
		
		constexpr size_t findIndex(const char* word) const noexcept {
			size_t i = FindWordIndex(wtoi, size, {word, size_t(-1)});
			return (i < size ? wtoi[i].index : size_t(-1));
		}
		
		constexpr const char* findWord(size_t index) const noexcept {
			size_t i = FindIndexWord(itow, size, {nullptr, index});
			return (i < size ? itow[i].word : nullptr);
		}
		
		constexpr size_t insertWordIndex(const Word &word) noexcept {
			return InsertWordIndex(wtoi, size, word, misses);
		}
		
		constexpr size_t insertIndexWord(const Word &word) noexcept {
			return InsertIndexWord(itow, size, word, misses);
		}
	};







}

#endif
