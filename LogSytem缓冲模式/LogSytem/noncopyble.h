#ifndef _NONCOPYBLE__H
#define _NONCOPYBLE__H

class noncopyble
{
public:
	noncopyble() {};
	noncopyble(const noncopyble&) = delete;
	noncopyble& operator=(const noncopyble&) = delete;
};

#endif
