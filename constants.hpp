//
// Created by imper on 9/16/22.
//

#ifndef FILESTORAGE_CONSTANTS_HPP
#define FILESTORAGE_CONSTANTS_HPP


# define _STR(s) #s
# define MACRO_STR(v) _STR(v)


# ifndef VERSION
#  define VERSION "(devel)"
# endif

# ifndef APPNAME
#  define APPNAME "percarch"
# endif

# ifndef INPUTS_COUNT
#  define INPUTS_COUNT 32
# endif

# ifndef OUTPUTS_COUNT
#  define OUTPUTS_COUNT 8
# endif

# ifndef LEARNING_RATE
#  define LEARNING_RATE 0.01
# endif

#endif //FILESTORAGE_CONSTANTS_HPP
