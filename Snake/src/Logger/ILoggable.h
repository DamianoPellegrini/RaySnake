#ifndef ILOGGABLE_H
#define ILOGGABLE_H

#include <string>

__interface ILoggable {
	virtual std::string ToString() const = 0;
};

#endif // !ILOGGABLE_H
