#ifndef _BUFFERL__H
#define _BUFFERL__H

template<size_t SIZE>
class BufferL
{
public:
	BufferL()
	{
		data_ = new char[SIZE];
		cur_ = data_;
	};

	~BufferL()
	{
		delete [] data_;
		data_ = nullptr;
		cur_ = data_;
	}

	void bzero()
	{
		memset(data_, 0, SIZE);
	}

	int unuse()
	{
		return data_ + SIZE - cur_;
	}

	void reset()
	{
		cur_ = data_;
	}

	void append(const char* data, int len)
	{
		if (unuse() >= len)
		{
			memcpy(cur_, data, len);
			cur_ += len;
		}
	}

	int length()
	{
		return cur_ - data_;
	}

	char* data() const
	{
		return data_;
	}

private:
	char* cur_;
	char* data_;
};

#endif