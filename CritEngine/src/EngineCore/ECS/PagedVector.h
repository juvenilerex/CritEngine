#pragma once

#include <array>
#include <vector>
#include <memory>
#include <optional>

namespace Engine {

	template<typename T, size_t PageSize>
	class PagedVector
	{
	public:

		PagedVector() {
			pages.resize(1);
		};

		class Iterator
		{
		public:
			using Pages = std::vector<std::unique_ptr<std::array<T, PageSize>>>;

			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T&;

			Iterator(Pages& pages, size_t pageIndex, size_t elementIndex) : pages(pages), pageIndex(pageIndex), elementIndex(elementIndex) {}

			reference operator*() const { return (*pages[pageIndex])[elementIndex]; }

			Iterator& operator++() { 
				do
				{
					++this->elementIndex;
					if (elementIndex >= PageSize)
					{
						++this->pageIndex;
						this->elementIndex = 0;
					}
				} while (pageIndex < this->pages.size() && pages[pageIndex] == nullptr);
				return *this;
			}
			Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

			friend bool operator== (const Iterator& a, const Iterator& b) { return a.pageIndex == b.pageIndex && a.elementIndex == b.elementIndex; };
			friend bool operator!= (const Iterator& a, const Iterator& b) { return a.pageIndex != b.pageIndex || a.elementIndex != b.elementIndex; };

		private:
			Pages& pages;
			size_t pageIndex;
			size_t elementIndex;
		};

		Iterator begin() { 
			
			for (size_t i = 0; i < this->pages.size(); ++i)
			{
				if (this->pages[i] == nullptr) continue;
				return Iterator(this->pages, i, 0);
			}
			return this->end();
		}

		Iterator end() { 
			for (size_t i = this->pages.size(); --i > 0;)
			{
				if (this->pages[i] == nullptr) continue;
				return Iterator(this->pages, i+1, 0);
			}
			return Iterator(pages, 0, 0);
		}

		size_t Size()
		{
			return this->pages.size() * PageSize;
		}

		std::optional<T> Get(size_t index)
		{
			size_t pageIndex = index / PageSize;
			size_t offset = index % PageSize;

			if (this->pages.size() <= pageIndex || this->pages[pageIndex] == nullptr)
			{
				return std::nullopt;
			}
			return std::optional<T>{(*this->pages[pageIndex])[offset]};
		}

		void Insert(size_t index, T value)
		{
			size_t pageIndex = index / PageSize;
			size_t offset = index % PageSize;
			
			if (this->pages.size() <= pageIndex)
			{
				this->pages.resize(pageIndex + 1);
			}
			
			if (this->pages[pageIndex] == nullptr)
			{
				this->pages[pageIndex] = std::make_unique<std::array<T, PageSize>>();
			}
			
			(*this->pages[pageIndex])[offset] = value;
		}

	private:
		std::vector<std::unique_ptr<std::array<T, PageSize>>> pages;
	};

}