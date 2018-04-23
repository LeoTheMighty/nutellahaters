/*
 * Cache.h
 *
 *  Created on: Apr 19, 2018
 *      Author: lbelyi
 */

#ifndef CACHE_H_
#define CACHE_H_

#define uint8_t unsigned char

struct CacheLine;
struct CacheSet;
struct Cache;
struct Cache* initCache(unsigned int E, unsigned int b, unsigned int s);
struct CacheSet initSet(unsigned int E);
struct CacheLine initLine();
unsigned int pw2(unsigned int p);

#endif /* CACHE_H_ */
