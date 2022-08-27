/**
 * @Author: Michael K. Steinberg
 * @Created: 26/08/2022
 * @Purpose: Declerations for commonly used structs and enums.
 */
#ifndef __COMMON_H
#define __COMMON_H

#include <Arduino.h>
#include <exception>
#include <stdexcept>
#include "config.h"

enum class ReturnCode 
{
  Success,
  UnknownError,

  InvalidParameters,
};

#endif // __COMMON_H