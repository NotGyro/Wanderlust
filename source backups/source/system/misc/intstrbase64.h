#ifndef INTSTR64_H
#define INTSTR64_H

#include <string>

namespace Wanderlust
{
	std::string IntToBase64(int);
	int Base64ToInt(std::string);
}

#endif