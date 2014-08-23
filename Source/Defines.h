/*
 * Copyright 2014 Martin Hansen
 *
 * This file is part of JLickshot.
 *
 * JLickshot is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * JLickshot is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with JLickshot.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef DEFINES_H_INCLUDED
#define DEFINES_H_INCLUDED

#ifndef NUMBER_OF_NOTES
#define NUMBER_OF_NOTES 128 /**< How many MIDI notes there are. */
#endif

#ifndef VERSION_STRING
#define VERSION_STRING "0.1.1 alpha"
#endif

#ifndef SATURATE
#define SATURATE(lower, upper, value) ((value) < (lower)) ? (lower) \
                                    : ((value) > (upper)) ? (upper) : (value)
#endif

#ifndef NORMALIZE
#define NORMALIZE(min, max, val) ((val)-(min)) / ((max)-(min))
#endif

#ifndef DENORMALIZE
#define DENORMALIZE(min, max, val) ((val) * ((max)-(min))) + (min)
#endif

#endif  // DEFINES_H_INCLUDED
