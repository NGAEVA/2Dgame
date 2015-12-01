#ifndef INCLUDED_NONCOPYABLE_H
#define INCLUDED_NONCOPYABLE_H

class NonCopyable
{
protected:
	NonCopyable() {}
	~NonCopyable() {}

private:
	// Explicitly block copy/assign operations
	NonCopyable(const NonCopyable&);
	NonCopyable& operator=(const NonCopyable&);
};

#endif // #ifndef INCLUDED_NONCOPYABLE_H