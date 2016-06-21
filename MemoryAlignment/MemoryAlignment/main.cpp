/*
Title: Memory Alignment
File Name: main.cpp
Copyright © 2016
Original authors: Luna Meier
Written under the supervision of David I. Schwartz, Ph.D., and
supported by a professional development seed grant from the B. Thomas
Golisano College of Computing & Information Sciences
(https://www.rit.edu/gccis) at the Rochester Institute of Technology.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

Description:
This example shows off multiple ways to create aligned memory.  Aligned memory is
useful for getting a speedup in applications, and is necessary for some instructions,
like SSE instructions.

References:
https://msdn.microsoft.com/library/83ythb65.aspx
https://msdn.microsoft.com/en-us/library/8z34s9c6.aspx
*/

#include <intrin.h>

int main(){

	//There are a few ways to get aligned memory.

	_declspec(align(16)) float array1[4];
	// You can declspec align to define aligned data.
	array1[0] = 0;
	array1[1] = 0;
	array1[2] = 0;
	array1[3] = 0;
	
	// You can also allocate the memory on the heap with aligned malloc.
	float* array2 = (float*)_aligned_malloc(sizeof(float) * 4, 16);
	array2[0] = 0;
	array2[1] = 0;
	array2[2] = 0;
	array2[3] = 0;
	// don't forget to use _aligned_free to deallocate it.

	// Those two methods handle the step of alignment for you, but what if you want to
	// write your own allocator?  How do you align that memory?

	// Let's assume you want to align to a boundary that's a multiple of 2, because
	// that's really almost always what you need.

	// Let's do 16.
	uintptr_t tempPointer = (uintptr_t)malloc(sizeof(float) * 4 + 15);
	// First you allocate the memory you need plus one less than the alignment you want.
	// this way at least some pointer in that block is aligned.
	// We cast to a uintptr_t so we can manipulate it algebraically.

	// Next we have to add to the original pointer one less than the alignment we want.
	// we'll cut off down to the right alignment.

	
	float* array3 = (float*)((tempPointer + 15)&(~15)); //add fifteen, and and it with ~15.  
	array3[0] = 0;
	array3[1] = 0;
	array3[2] = 0;
	array3[3] = 0;
	// That way it gets cut to the nearest multiple of 16.
	// The data is now aligned for sure.

	// remember, if you want to deallocate this memory, you actually need to go
	// free((void*)tempPointer);
	// This is because that's the pointer you allocated on.
	// yes, this leads to some internal fragmentation.  But that's the price to pay for aligned
	// memory.

	__m128 arr1 = _mm_load_ps(array1);
	__m128 arr2 = _mm_load_ps(array2);
	__m128 arr3 = _mm_load_ps(array3);

	// These crash if they're not aligned.  Look.
	float* array4 = (float*)malloc(sizeof(float) * 4);
	array4[0] = 0;
	array4[1] = 0;
	array4[2] = 0;
	array4[3] = 0;
	__m128 arr4 = _mm_load_ps(array4); //Run it a couple times, just in case.  Sometimes
	// you randomly happen to have aligned memory, in which case it actually works.  You shouldn't
	// rely on it working though.


	return 1;
}