/*
 * bench.cpp
 *
 * Benchmarks of libsrt vs C++ STL
 *
 * Copyright (c) 2015-2016, F. Aragon. All rights reserved. Released under
 * the BSD 3-Clause License (see the doc/LICENSE file included).
 */

#include "../src/libsrt.h"
#include "utf8_examples.h"
#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <vector>
#if __cplusplus >= 201103L
#include <unordered_map>
#endif

#if (defined(__linux__) || defined(__unix__) || defined(_POSIX_VERSION)) && \
    !defined(__APPLE_CC__)
#include <time.h>
#include <stdio.h>
#define BENCH_INIT		\
	struct timespec ta, tb;	\
	bool t_res = false
#define BENCH_TIME_US				\
	((tb.tv_sec - ta.tv_sec) * 1000000 +	\
	 (tb.tv_nsec - ta.tv_nsec) / 1000)
#define BENCH_FN(test_fn, count, nread, delete_all)		\
	clock_gettime(CLOCK_REALTIME, &ta);			\
	t_res = test_fn(count, nread, delete_all);		\
	clock_gettime(CLOCK_REALTIME, &tb);			\
	if (t_res)						\
		printf("| %s | %zu | %zu | - | %u.%06u |\n",	\
		       #test_fn, count, count * nread,		\
		       BENCH_TIME_US_I, BENCH_TIME_US_D)
#else
#define BENCH_INIT
#define BENCH_FN(test_fn, count, nread, delete_all)	\
	test_fn(count, nread, delete_all)
#define BENCH_TIME_US 0
#endif
#define BENCH_TIME_MS (BENCH_TIME_US / 1000)
#define BENCH_TIME_US_I ((unsigned)(BENCH_TIME_US / 1000000))
#define BENCH_TIME_US_D ((unsigned)(BENCH_TIME_US % 1000000))

bool libsrt_map_ii32(size_t count, size_t read_ntimes, bool delete_all)
{
	sm_t *m = sm_alloc(SM_II32, 0);
	for (size_t i = 0; i < count; i++)
		sm_insert_ii32(&m, (int32_t)i, (int32_t)i);
	for (size_t j = 0; j < read_ntimes; j++)
		for (size_t i = 0; i < count; i++)
			(void)sm_at_ii32(m, (int32_t)i);
	if (delete_all)
		for (size_t i = 0; i < count; i++)
			sm_delete_i(m, (int32_t)i);
	sm_free(&m);
	return true;
}

bool cxx_map_ii32(size_t count, size_t read_ntimes, bool delete_all)
{
	std::map <int32_t, int32_t> m;
	for (size_t i = 0; i < count; i++)
		m[i] = (int32_t)i;
	for (size_t j = 0; j < read_ntimes; j++)
		for (size_t i = 0; i < count; i++)
			(void)m[i];
	if (delete_all)
		for (size_t i = 0; i < count; i++)
			m.erase((int32_t)i);
	return true;
}

bool libsrt_map_ii64(size_t count, size_t read_ntimes, bool delete_all)
{
	sm_t *m = sm_alloc(SM_II, 0);
	for (size_t i = 0; i < count; i++)
		sm_insert_ii(&m, (int64_t)i, (int64_t)i);
	for (size_t j = 0; j < read_ntimes; j++)
		for (size_t i = 0; i < count; i++)
			(void)sm_at_ii(m, (int64_t)i);
	if (delete_all)
		for (size_t i = 0; i < count; i++)
			sm_delete_i(m, (int64_t)i);
	sm_free(&m);
	return true;
}

bool cxx_map_ii64(size_t count, size_t read_ntimes, bool delete_all)
{
	std::map <int64_t, int64_t> m;
	for (size_t i = 0; i < count; i++)
		m[i] = (int64_t)i;
	for (size_t j = 0; j < read_ntimes; j++)
		for (size_t i = 0; i < count; i++)
			(void)m[i];
	if (delete_all)
		for (size_t i = 0; i < count; i++)
			m.erase((int64_t)i);
	return true;
}

bool libsrt_map_s16(size_t count, size_t read_ntimes, bool delete_all)
{
	ss_t *btmp = ss_alloca(512);
	sm_t *m = sm_alloc(SM_SS, 0);
	for (size_t i = 0; i < count; i++) {
		ss_printf(&btmp, 512, "%016i", (int)i);
		sm_insert_ss(&m, btmp, btmp);
	}
	for (size_t j = 0; j < read_ntimes; j++)
		for (size_t i = 0; i < count; i++) {
			ss_printf(&btmp, 512, "%016i", (int)i);
			(void)sm_at_ss(m, btmp);
		}
	if (delete_all)
		for (size_t i = 0; i < count; i++) {
			ss_printf(&btmp, 512, "%016i", (int)i);
			sm_delete_s(m, btmp);
		}
	sm_free(&m);
	return true;
}

bool cxx_map_s16(size_t count, size_t read_ntimes, bool delete_all)
{
	char btmp[512];
	std::map <std::string, std::string> m;
	for (size_t i = 0; i < count; i++) {
		sprintf(btmp, "%016i", (int)i);
		m[btmp] = btmp;
	}
	for (size_t j = 0; j < read_ntimes; j++)
		for (size_t i = 0; i < count; i++) {
			sprintf(btmp, "%016i", (int)i);
			(void)m[btmp];
		}
	if (delete_all)
		for (size_t i = 0; i < count; i++) {
			sprintf(btmp, "%016i", (int)i);
			m.erase(btmp);
		}
	return true;
}

bool libsrt_map_s64(size_t count, size_t read_ntimes, bool delete_all)
{
	ss_t *btmp = ss_alloca(512);
	sm_t *m = sm_alloc(SM_SS, 0);
	for (size_t i = 0; i < count; i++) {
		ss_printf(&btmp, 512, "%064i", (int)i);
		sm_insert_ss(&m, btmp, btmp);
	}
	for (size_t j = 0; j < read_ntimes; j++)
		for (size_t i = 0; i < count; i++) {
			ss_printf(&btmp, 512, "%064i", (int)i);
			(void)sm_at_ss(m, btmp);
		}
	if (delete_all)
		for (size_t i = 0; i < count; i++) {
			ss_printf(&btmp, 512, "%064i", (int)i);
			sm_delete_s(m, btmp);
		}
	sm_free(&m);
	return true;
}

bool cxx_map_s64(size_t count, size_t read_ntimes, bool delete_all)
{
	char btmp[512];
	std::map <std::string, std::string> m;
	for (size_t i = 0; i < count; i++) {
		sprintf(btmp, "%064i", (int)i);
		m[btmp] = btmp;
	}
	for (size_t j = 0; j < read_ntimes; j++)
		for (size_t i = 0; i < count; i++) {
			sprintf(btmp, "%064i", (int)i);
			(void)m[btmp];
		}
	if (delete_all)
		for (size_t i = 0; i < count; i++) {
			sprintf(btmp, "%064i", (int)i);
			m.erase(btmp);
		}
	return true;
}

#if __cplusplus >= 201103L

bool cxx_umap_ii32(size_t count, size_t read_ntimes, bool delete_all)
{
	std::unordered_map <int32_t, int32_t> m;
	for (size_t i = 0; i < count; i++)
		m[i] = (int32_t)i;
	for (size_t j = 0; j < read_ntimes; j++)
		for (size_t i = 0; i < count; i++)
			(void)m.count(i);
	if (delete_all)
		for (size_t i = 0; i < count; i++)
			m.erase((int32_t)i);
	return true;
}

bool cxx_umap_ii64(size_t count, size_t read_ntimes, bool delete_all)
{
	std::unordered_map <int64_t, int64_t> m;
	for (size_t i = 0; i < count; i++)
		m[i] = (int64_t)i;
	for (size_t j = 0; j < read_ntimes; j++)
		for (size_t i = 0; i < count; i++)
			(void)m.count(i);
	if (delete_all)
		for (size_t i = 0; i < count; i++)
			m.erase((int64_t)i);
	return true;
}

bool cxx_umap_s16(size_t count, size_t read_ntimes, bool delete_all)
{
	char btmp[512];
	std::unordered_map <std::string, std::string> m;
	for (size_t i = 0; i < count; i++) {
		sprintf(btmp, "%016i", (int)i);
		m[btmp] = btmp;
	}
	for (size_t j = 0; j < read_ntimes; j++)
		for (size_t i = 0; i < count; i++) {
			sprintf(btmp, "%016i", (int)i);
			(void)m.count(btmp);
		}
	if (delete_all)
		for (size_t i = 0; i < count; i++) {
			sprintf(btmp, "%016i", (int)i);
			m.erase(btmp);
		}
	return true;
}

bool cxx_umap_s64(size_t count, size_t read_ntimes, bool delete_all)
{
	char btmp[512];
	std::unordered_map <std::string, std::string> m;
	for (size_t i = 0; i < count; i++) {
		sprintf(btmp, "%064i", (int)i);
		m[btmp] = btmp;
	}
	for (size_t j = 0; j < read_ntimes; j++)
		for (size_t i = 0; i < count; i++) {
			sprintf(btmp, "%064i", (int)i);
			(void)m.count(btmp);
		}
	if (delete_all)
		for (size_t i = 0; i < count; i++) {
			sprintf(btmp, "%064i", (int)i);
			m.erase(btmp);
		}
	return true;
}

#endif

bool libsrt_set_i32(size_t count, size_t read_ntimes, bool delete_all)
{
	sms_t *m = sms_alloc(SMS_I32, 0);
	for (size_t i = 0; i < count; i++)
		sms_insert_i32(&m, (int32_t)i);
	for (size_t j = 0; j < read_ntimes; j++)
		for (size_t i = 0; i < count; i++)
			(void)sms_count_i(m, (int32_t)i);
	if (delete_all)
		for (size_t i = 0; i < count; i++)
			sms_delete_i(m, (int32_t)i);
	sms_free(&m);
	return true;
}

bool cxx_set_i32(size_t count, size_t read_ntimes, bool delete_all)
{
	std::set <int32_t> m;
	for (size_t i = 0; i < count; i++)
		m.insert((int32_t)i);
	for (size_t j = 0; j < read_ntimes; j++)
		for (size_t i = 0; i < count; i++)
			(void)m.count(i);
	if (delete_all)
		for (size_t i = 0; i < count; i++)
			m.erase((int32_t)i);
	return true;
}

bool libsrt_set_i64(size_t count, size_t read_ntimes, bool delete_all)
{
	sms_t *m = sms_alloc(SMS_I, 0);
	for (size_t i = 0; i < count; i++)
		sms_insert_i32(&m, (int64_t)i);
	for (size_t j = 0; j < read_ntimes; j++)
		for (size_t i = 0; i < count; i++)
			(void)sms_count_i(m, (int64_t)i);
	if (delete_all)
		for (size_t i = 0; i < count; i++)
			sms_delete_i(m, (int64_t)i);
	sms_free(&m);
	return true;
}

bool cxx_set_i64(size_t count, size_t read_ntimes, bool delete_all)
{
	std::set <int64_t> m;
	for (size_t i = 0; i < count; i++)
		m.insert((int64_t)i);
	for (size_t j = 0; j < read_ntimes; j++)
		for (size_t i = 0; i < count; i++)
			(void)m.count(i);
	if (delete_all)
		for (size_t i = 0; i < count; i++)
			m.erase((int64_t)i);
	return true;
}

bool libsrt_set_s16(size_t count, size_t read_ntimes, bool delete_all)
{
	ss_t *btmp = ss_alloca(512);
	sms_t *m = sms_alloc(SMS_S, 0);
	for (size_t i = 0; i < count; i++) {
		ss_printf(&btmp, 512, "%016i", (int)i);
		sms_insert_s(&m, btmp);
	}
	for (size_t j = 0; j < read_ntimes; j++)
		for (size_t i = 0; i < count; i++) {
			ss_printf(&btmp, 512, "%016i", (int)i);
			(void)sms_count_s(m, btmp);
		}
	if (delete_all)
		for (size_t i = 0; i < count; i++) {
			ss_printf(&btmp, 512, "%016i", (int)i);
			sms_delete_s(m, btmp);
		}
	sms_free(&m);
	return true;
}

bool cxx_set_s16(size_t count, size_t read_ntimes, bool delete_all)
{
	char btmp[512];
	std::set <std::string> m;
	for (size_t i = 0; i < count; i++) {
		sprintf(btmp, "%016i", (int)i);
		m.insert(btmp);
	}
	for (size_t j = 0; j < read_ntimes; j++)
		for (size_t i = 0; i < count; i++) {
			sprintf(btmp, "%016i", (int)i);
			(void)m.count(btmp);
		}
	if (delete_all)
		for (size_t i = 0; i < count; i++) {
			sprintf(btmp, "%016i", (int)i);
			m.erase(btmp);
		}
	return true;
}

bool libsrt_set_s64(size_t count, size_t read_ntimes, bool delete_all)
{
	ss_t *btmp = ss_alloca(512);
	sms_t *m = sms_alloc(SMS_S, 0);
	for (size_t i = 0; i < count; i++) {
		ss_printf(&btmp, 512, "%064i", (int)i);
		sms_insert_s(&m, btmp);
	}
	for (size_t j = 0; j < read_ntimes; j++)
		for (size_t i = 0; i < count; i++) {
			ss_printf(&btmp, 512, "%064i", (int)i);
			(void)sms_count_s(m, btmp);
		}
	if (delete_all)
		for (size_t i = 0; i < count; i++) {
			ss_printf(&btmp, 512, "%064i", (int)i);
			sms_delete_s(m, btmp);
		}
	sms_free(&m);
	return true;
}

bool cxx_set_s64(size_t count, size_t read_ntimes, bool delete_all)
{
	char btmp[512];
	std::set <std::string> m;
	for (size_t i = 0; i < count; i++) {
		sprintf(btmp, "%064i", (int)i);
		m.insert(btmp);
	}
	for (size_t j = 0; j < read_ntimes; j++)
		for (size_t i = 0; i < count; i++) {
			sprintf(btmp, "%064i", (int)i);
			(void)m.count(btmp);
		}
	if (delete_all)
		for (size_t i = 0; i < count; i++) {
			sprintf(btmp, "%064i", (int)i);
			m.erase(btmp);
		}
	return true;
}

bool libsrt_vector_i(enum eSV_Type t, size_t count, size_t read_ntimes, bool delete_all)
{
	sv_t *v = sv_alloc_t(t, 0);
	for (size_t i = 0; i < count; i++)
		sv_push_i(&v, (int32_t)i);
	for (size_t j = 0; j < read_ntimes; j++)
		for (size_t i = 0; i < count; i++)
			(void)sv_at_i(v, i);
	if (delete_all)
		for (size_t i = 0; i < count; i++)
			(void)sv_pop_i(v);
	sv_free(&v);
	return true;
}

bool libsrt_vector_i8(size_t count, size_t read_ntimes, bool delete_all)
{
	return libsrt_vector_i(SV_I32, count, read_ntimes, delete_all);
}

bool libsrt_vector_i16(size_t count, size_t read_ntimes, bool delete_all)
{
	return libsrt_vector_i(SV_I16, count, read_ntimes, delete_all);
}

bool libsrt_vector_i32(size_t count, size_t read_ntimes, bool delete_all)
{
	return libsrt_vector_i(SV_I32, count, read_ntimes, delete_all);
}

bool libsrt_vector_i64(size_t count, size_t read_ntimes, bool delete_all)
{
	return libsrt_vector_i(SV_I64, count, read_ntimes, delete_all);
}

template <typename T>
bool cxx_vector(size_t count, size_t read_ntimes, bool delete_all)
{
	std::vector <T> v;
	for (size_t i = 0; i < count; i++)
		v.push_back((T)i);
	for (size_t j = 0; j < read_ntimes; j++)
		for (size_t i = 0; i < count; i++)
			(void)v.at(i);
	if (delete_all)
		for (size_t i = 0; i < count; i++)
			(void)v.pop_back();
	return true;
}

bool cxx_vector_i8(size_t count, size_t read_ntimes, bool delete_all)
{
	return cxx_vector<int8_t>(count, read_ntimes, delete_all);
}

bool cxx_vector_i16(size_t count, size_t read_ntimes, bool delete_all)
{
	return cxx_vector<int16_t>(count, read_ntimes, delete_all);
}

bool cxx_vector_i32(size_t count, size_t read_ntimes, bool delete_all)
{
	return cxx_vector<int32_t>(count, read_ntimes, delete_all);
}

bool cxx_vector_i64(size_t count, size_t read_ntimes, bool delete_all)
{
	return cxx_vector<int64_t>(count, read_ntimes, delete_all);
}

struct StrGenTest
{
	uint8_t raw[32];
};

bool libsrt_vector_gen(size_t count, size_t read_ntimes, bool delete_all)
{
	struct StrGenTest aux;
	sv_t *v = sv_alloc(sizeof(struct StrGenTest), 0, NULL);
	for (size_t i = 0; i < count; i++)
		sv_push(&v, &aux);
	for (size_t j = 0; j < read_ntimes; j++)
		for (size_t i = 0; i < count; i++)
			(void)sv_at(v, i);
	if (delete_all)
		for (size_t i = 0; i < count; i++)
			(void)sv_pop(v);
	sv_free(&v);
	return true;
}

bool cxx_vector_gen(size_t count, size_t read_ntimes, bool delete_all)
{
	struct StrGenTest aux;
	std::vector <struct StrGenTest> v;
	for (size_t i = 0; i < count; i++)
		v.push_back(aux);
	for (size_t j = 0; j < read_ntimes; j++)
		for (size_t i = 0; i < count; i++)
			(void)v.at(i);
	if (delete_all)
		for (size_t i = 0; i < count; i++)
			(void)v.pop_back();
	return true;
}

const char
	*haystack_easymatch1_long =
	"Alice was beginning to get very tired of sitting by her sister on the"
	" bank, and of having nothing to do. Once or twice she had peeped into"
	" the book her sister was reading, but it had no pictures or conversat"
	"ions in it, \"and what is the use of a book,\" thought Alice, \"witho"
	"ut pictures or conversations?\"",
	*haystack_easymatch2_long =
	U8_MIX1 U8_MIX1 U8_MIX1 U8_MIX1 U8_MIX1 U8_MIX1 U8_MIX1 U8_MIX1 U8_MIX1
	U8_MIX1 U8_MIX1 U8_MIX1 U8_MIX1 U8_MIX1 U8_MIX1 U8_MIX1 U8_MIX1 U8_MIX1
	U8_HAN_611B U8_MIX1 U8_MIX1 U8_MIX1 U8_MIX1 U8_MIX1 U8_MIX1 U8_MIX1,
	*haystack_hardmatch1_long =
	"111111x11111131111111111111111111111111111111111111111111111111111111"
	"111111111111111111111111411111111111111111111111111111111111111111111"
	"111111111111111111111111111111111111111111111111111111111111111111111"
	"12k1",
	*haystack_hardmatch1_short = "11111111111111111112k1",
	*haystack_hardmatch2_long =
	"abcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcda"
	"bcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdab"
	"cdabcdabcdabcdabcdabcdabcdabcddcbadcbadcbadcba",
	*haystack_hardmatch3_long =
	U8_MANY_UNDERSCORES U8_MANY_UNDERSCORES U8_MANY_UNDERSCORES
	U8_MANY_UNDERSCORES U8_MANY_UNDERSCORES "1234567890",
	*needle_easymatch1a = " a ",
	*needle_easymatch1b = "conversations?",
	*needle_easymatch2a = U8_HAN_611B,
	*needle_easymatch2b = U8_MIX1 U8_HAN_611B,
	*needle_hardmatch1a = "1111111112k1",
	*needle_hardmatch1b = "112k1",
	*needle_hardmatch2 = "dcba",
	*needle_hardmatch3 = U8_MANY_UNDERSCORES "123";

bool libsrt_string_search(const char *haystack, const char *needle,
			  size_t count, size_t read_ntimes, bool delete_all)
{
	RETURN_IF(delete_all, false);
	const ss_t *h = ss_crefa(haystack), *n = ss_crefa(needle);
	for (size_t i = 0; i < count; i++)
		ss_find(h, 0, n);
	for (size_t j = 0; j < read_ntimes; j++)
		for (size_t i = 0; i < count; i++)
			ss_find(h, 0, n);
	return true;
}

bool c_string_search(const char *haystack, const char *needle,
		     size_t count, size_t read_ntimes, bool delete_all)
{
	RETURN_IF(delete_all, false);
	for (size_t i = 0; i < count; i++)
		strstr(haystack, needle) || putchar(0);
	for (size_t j = 0; j < read_ntimes; j++)
		for (size_t i = 0; i < count; i++)
			strstr(haystack, needle) || putchar(0);
	return true;
}

bool cxx_string_search(const char *haystack, const char *needle,
		       size_t count, size_t read_ntimes, bool delete_all)
{
	RETURN_IF(delete_all, false);
	std::string h = haystack, n = needle;
	for (size_t i = 0; i < count; i++)
		h.find(needle);
	for (size_t j = 0; j < read_ntimes; j++)
		for (size_t i = 0; i < count; i++)
			h.find(needle);
	return true;
}

bool libsrt_string_search_easymatch_long_1a(size_t count, size_t read_ntimes,
					    bool delete_all)
{
	return libsrt_string_search(haystack_easymatch1_long,
				    needle_easymatch1a, count,
				    read_ntimes, delete_all);
}

bool libsrt_string_search_easymatch_long_1b(size_t count, size_t read_ntimes,
					    bool delete_all)
{
	return libsrt_string_search(haystack_easymatch1_long,
				    needle_easymatch1b, count,
				    read_ntimes, delete_all);
}

bool libsrt_string_search_easymatch_long_2a(size_t count, size_t read_ntimes,
					    bool delete_all)
{
	return libsrt_string_search(haystack_easymatch2_long,
				    needle_easymatch2a, count,
				    read_ntimes, delete_all);
}

bool libsrt_string_search_easymatch_long_2b(size_t count, size_t read_ntimes,
					    bool delete_all)
{
	return libsrt_string_search(haystack_easymatch2_long,
				    needle_easymatch2b, count,
				    read_ntimes, delete_all);
}

bool libsrt_string_search_hardmatch_long_1a(size_t count, size_t read_ntimes,
					    bool delete_all)
{
	return libsrt_string_search(haystack_hardmatch1_long,
				    needle_hardmatch1a, count,
				    read_ntimes, delete_all);
}

bool libsrt_string_search_hardmatch_long_1b(size_t count, size_t read_ntimes,
					    bool delete_all)
{
	return libsrt_string_search(haystack_hardmatch1_long,
				    needle_hardmatch1b, count,
				    read_ntimes, delete_all);
}

bool libsrt_string_search_hardmatch_short_1a(size_t count, size_t read_ntimes,
					     bool delete_all)
{
	return libsrt_string_search(haystack_hardmatch1_short,
				    needle_hardmatch1a, count,
				    read_ntimes, delete_all);
}

bool libsrt_string_search_hardmatch_short_1b(size_t count, size_t read_ntimes,
					     bool delete_all)
{
	return libsrt_string_search(haystack_hardmatch1_short,
				    needle_hardmatch1b, count,
				    read_ntimes, delete_all);
}

bool libsrt_string_search_hardmatch_long_2(size_t count, size_t read_ntimes,
					   bool delete_all)
{
	return libsrt_string_search(haystack_hardmatch2_long, needle_hardmatch2,
				    count, read_ntimes, delete_all);
}

bool libsrt_string_search_hardmatch_long_3(size_t count, size_t read_ntimes,
					   bool delete_all)
{
	return libsrt_string_search(haystack_hardmatch3_long, needle_hardmatch3,
				    count, read_ntimes, delete_all);
}

bool c_string_search_easymatch_long_1a(size_t count, size_t read_ntimes,
				       bool delete_all)
{
	return c_string_search(haystack_easymatch1_long, needle_easymatch1a,
			       count, read_ntimes, delete_all);
}

bool c_string_search_easymatch_long_1b(size_t count, size_t read_ntimes,
				       bool delete_all)
{
	return c_string_search(haystack_easymatch1_long, needle_easymatch1b,
			       count, read_ntimes, delete_all);
}

bool c_string_search_easymatch_long_2a(size_t count, size_t read_ntimes,
				       bool delete_all)
{
	return c_string_search(haystack_easymatch2_long, needle_easymatch2a,
			       count, read_ntimes, delete_all);
}

bool c_string_search_easymatch_long_2b(size_t count, size_t read_ntimes,
				       bool delete_all)
{
	return c_string_search(haystack_easymatch2_long, needle_easymatch2b,
			       count, read_ntimes, delete_all);
}

bool c_string_search_hardmatch_long_1a(size_t count, size_t read_ntimes,
				       bool delete_all)
{
	return c_string_search(haystack_hardmatch1_long, needle_hardmatch1a,
			       count, read_ntimes, delete_all);
}

bool c_string_search_hardmatch_long_1b(size_t count, size_t read_ntimes,
				       bool delete_all)
{
	return c_string_search(haystack_hardmatch1_long, needle_hardmatch1b,
			       count, read_ntimes, delete_all);
}

bool c_string_search_hardmatch_short_1a(size_t count, size_t read_ntimes,
					bool delete_all)
{
	return c_string_search(haystack_hardmatch1_short, needle_hardmatch1a,
			       count, read_ntimes, delete_all);
}

bool c_string_search_hardmatch_short_1b(size_t count, size_t read_ntimes,
					bool delete_all)
{
	return c_string_search(haystack_hardmatch1_short, needle_hardmatch1b,
			       count, read_ntimes, delete_all);
}

bool c_string_search_hardmatch_long_2(size_t count, size_t read_ntimes,
				      bool delete_all)
{
	return c_string_search(haystack_hardmatch2_long, needle_hardmatch2,
			       count, read_ntimes, delete_all);
}

bool c_string_search_hardmatch_long_3(size_t count, size_t read_ntimes,
				      bool delete_all)
{
	return c_string_search(haystack_hardmatch3_long, needle_hardmatch3,
			       count, read_ntimes, delete_all);
}

bool cxx_string_search_easymatch_long_1a(size_t count, size_t read_ntimes,
					 bool delete_all)
{
	return cxx_string_search(haystack_easymatch1_long, needle_easymatch1a,
				 count, read_ntimes, delete_all);
}

bool cxx_string_search_easymatch_long_1b(size_t count, size_t read_ntimes,
					 bool delete_all)
{
	return cxx_string_search(haystack_easymatch1_long, needle_easymatch1b,
				 count, read_ntimes, delete_all);
}

bool cxx_string_search_easymatch_long_2a(size_t count, size_t read_ntimes,
					 bool delete_all)
{
	return cxx_string_search(haystack_easymatch2_long, needle_easymatch2a,
				 count, read_ntimes, delete_all);
}

bool cxx_string_search_easymatch_long_2b(size_t count, size_t read_ntimes,
					 bool delete_all)
{
	return cxx_string_search(haystack_easymatch2_long, needle_easymatch2b,
				 count, read_ntimes, delete_all);
}

bool cxx_string_search_hardmatch_long_1a(size_t count, size_t read_ntimes,
					 bool delete_all)
{
	return cxx_string_search(haystack_hardmatch1_long, needle_hardmatch1a,
				 count, read_ntimes, delete_all);
}

bool cxx_string_search_hardmatch_long_1b(size_t count, size_t read_ntimes,
					 bool delete_all)
{
	return cxx_string_search(haystack_hardmatch1_long, needle_hardmatch1b,
				 count, read_ntimes, delete_all);
}

bool cxx_string_search_hardmatch_short_1a(size_t count, size_t read_ntimes,
					  bool delete_all)
{
	return cxx_string_search(haystack_hardmatch1_short, needle_hardmatch1a,
				 count, read_ntimes, delete_all);
}

bool cxx_string_search_hardmatch_short_1b(size_t count, size_t read_ntimes,
					  bool delete_all)
{
	return cxx_string_search(haystack_hardmatch1_short, needle_hardmatch1b,
				 count, read_ntimes, delete_all);
}

bool cxx_string_search_hardmatch_long_2(size_t count, size_t read_ntimes,
					bool delete_all)
{
	return cxx_string_search(haystack_hardmatch2_long, needle_hardmatch2,
				 count, read_ntimes, delete_all);
}

bool cxx_string_search_hardmatch_long_3(size_t count, size_t read_ntimes,
					bool delete_all)
{
	return cxx_string_search(haystack_hardmatch3_long, needle_hardmatch3,
				 count, read_ntimes, delete_all);
}

const char
	case_test_ascii_str[95 + 1] =
	" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`"
	"abcdefghijklmnopqrstuvwxyz{|}~",
	case_test_utf8_str[95 + 1] =
	U8_MIX_28_bytes U8_MIX_28_bytes U8_MIX_28_bytes "12345678901";

bool libsrt_string_loweruppercase(const char *in, size_t count,
				  size_t read_ntimes, bool delete_all)
{
	RETURN_IF(delete_all, false);
	ss_t *s = ss_alloca(95);
	ss_cpy(&s, ss_crefa(in));
	for (size_t i = 0; i < count; i++) {
		ss_tolower(&s);
		ss_toupper(&s);
	}
	for (size_t j = 0; j < read_ntimes; j++)
		for (size_t i = 0; i < count; i++){
			ss_tolower(&s);
			ss_toupper(&s);
		}
	return true;
}

void cstolower(char *s)
{
	for (; *s; s++)
		*s = tolower(*s);
}

void cstoupper(char *s)
{
	for (; *s; s++)
		*s = toupper(*s);
}

bool c_string_loweruppercase(const char *in, size_t count,
			     size_t read_ntimes, bool delete_all)
{
	RETURN_IF(delete_all, false);
	char s[95 + 1];
	strncpy(s, in, 95);
	s[95] = 0;
	for (size_t i = 0; i < count; i++) {
		cstolower(s);
		cstoupper(s);
		if (s[0] == s[1])
			putchar(0);
	}
	for (size_t j = 0; j < read_ntimes; j++)
		for (size_t i = 0; i < count; i++) {
			cstolower(s);
			cstoupper(s);
			if (s[0] == s[1])
				putchar(0);
		}
	return true;
}

void cxxtolower(std::string &s)
{
	std::transform(s.begin(), s.end(), s.begin(), ::tolower);
}

void cxxtoupper(std::string &s)
{
	std::transform(s.begin(), s.end(), s.begin(), ::tolower);
}

bool cxx_string_loweruppercase(const char *in, size_t count,
			      size_t read_ntimes, bool delete_all)
{
	RETURN_IF(delete_all, false);
	std::string s = in;
	for (size_t i = 0; i < count; i++) {
		cxxtolower(s);
		cxxtoupper(s);
	}
	for (size_t j = 0; j < read_ntimes; j++)
		for (size_t i = 0; i < count; i++) {
			cxxtolower(s);
			cxxtoupper(s);
		}
	return true;
}

bool libsrt_string_loweruppercase_ascii(size_t count, size_t read_ntimes,
					bool delete_all)
{
	return libsrt_string_loweruppercase(case_test_ascii_str, count,
					    read_ntimes, delete_all);
}

bool libsrt_string_loweruppercase_utf8(size_t count, size_t read_ntimes,
				       bool delete_all)
{
	return libsrt_string_loweruppercase(case_test_utf8_str, count,
					    read_ntimes, delete_all);
}

bool c_string_loweruppercase_ascii(size_t count, size_t read_ntimes,
				   bool delete_all)
{
	return c_string_loweruppercase(case_test_ascii_str, count,
				       read_ntimes, delete_all);
}

bool c_string_loweruppercase_utf8(size_t count, size_t read_ntimes,
				  bool delete_all)
{
	return c_string_loweruppercase(case_test_utf8_str, count,
				       read_ntimes, delete_all);
}

bool cxx_string_loweruppercase_ascii(size_t count, size_t read_ntimes,
				     bool delete_all)
{
	return cxx_string_loweruppercase(case_test_ascii_str, count,
					 read_ntimes, delete_all);
}

bool cxx_string_loweruppercase_utf8(size_t count, size_t read_ntimes,
				    bool delete_all)
{
	return cxx_string_loweruppercase(case_test_utf8_str, count,
					 read_ntimes, delete_all);
}

int main(int argc, char *argv[])
{
	BENCH_INIT;
	const size_t ntests = 3,
		     count[ntests] = { 1000000, 1000000, 1000000 },
		     nread[ntests] = { 0, 10, 0 };
	bool delete_all[ntests] = { false, false, true };
	char label[ntests][512];
	snprintf(label[0], 512, "Insert or process %zu elements, cleanup",
		 count[0]);
	snprintf(label[1], 512, "Insert or process %zu elements, read or "
		 "operate over all elements %zu times, cleanup", count[1],
		 nread[1]);
	snprintf(label[2], 512, "Insert or process %zu elements, delete all "
		 "elements one by one, cleanup", count[1]);
	for (size_t i = 0; i < ntests; i++) {
		printf("\n%s\n| Test | Insert count | Read count | Memory (MB) "
		       "| Execution time (s) |\n|:---:|:---:|:---:|:---:|:---:|"
		       "\n", label[i]);
		BENCH_FN(libsrt_map_ii32, count[i], nread[i], delete_all[i]);
		BENCH_FN(cxx_map_ii32, count[i], nread[i], delete_all[i]);
#if __cplusplus >= 201103L
		BENCH_FN(cxx_umap_ii32, count[i], nread[i], delete_all[i]);
#endif
		BENCH_FN(libsrt_map_ii64, count[i], nread[i], delete_all[i]);
		BENCH_FN(cxx_map_ii64, count[i], nread[i], delete_all[i]);
#if __cplusplus >= 201103L
		BENCH_FN(cxx_umap_ii64, count[i], nread[i], delete_all[i]);
#endif
		BENCH_FN(libsrt_map_s16, count[i], nread[i], delete_all[i]);
		BENCH_FN(cxx_map_s16, count[i], nread[i], delete_all[i]);
#if __cplusplus >= 201103L
		BENCH_FN(cxx_umap_s16, count[i], nread[i], delete_all[i]);
#endif
		BENCH_FN(libsrt_map_s64, count[i], nread[i], delete_all[i]);
		BENCH_FN(cxx_map_s64, count[i], nread[i], delete_all[i]);
#if __cplusplus >= 201103L
		BENCH_FN(cxx_umap_s64, count[i], nread[i], delete_all[i]);
#endif
		BENCH_FN(libsrt_set_i32, count[i], nread[i], delete_all[i]);
		BENCH_FN(cxx_set_i32, count[i], nread[i], delete_all[i]);
		BENCH_FN(libsrt_set_i64, count[i], nread[i], delete_all[i]);
		BENCH_FN(cxx_set_i64, count[i], nread[i], delete_all[i]);
		BENCH_FN(libsrt_set_s16, count[i], nread[i], delete_all[i]);
		BENCH_FN(cxx_set_s16, count[i], nread[i], delete_all[i]);
		BENCH_FN(libsrt_set_s64, count[i], nread[i], delete_all[i]);
		BENCH_FN(cxx_set_s64, count[i], nread[i], delete_all[i]);
		BENCH_FN(libsrt_vector_i8, count[i], nread[i], delete_all[i]);
		BENCH_FN(cxx_vector_i8, count[i], nread[i], delete_all[i]);
		BENCH_FN(libsrt_vector_i16, count[i], nread[i], delete_all[i]);
		BENCH_FN(cxx_vector_i16, count[i], nread[i], delete_all[i]);
		BENCH_FN(libsrt_vector_i32, count[i], nread[i], delete_all[i]);
		BENCH_FN(cxx_vector_i32, count[i], nread[i], delete_all[i]);
		BENCH_FN(libsrt_vector_i64, count[i], nread[i], delete_all[i]);
		BENCH_FN(cxx_vector_i64, count[i], nread[i], delete_all[i]);
		BENCH_FN(libsrt_vector_gen, count[i], nread[i], delete_all[i]);
		BENCH_FN(cxx_vector_gen, count[i], nread[i], delete_all[i]);
		BENCH_FN(libsrt_string_search_easymatch_long_1a, count[i], nread[i], delete_all[i]);
		BENCH_FN(c_string_search_easymatch_long_1a, count[i], nread[i], delete_all[i]);
		BENCH_FN(cxx_string_search_easymatch_long_1a, count[i], nread[i], delete_all[i]);
		BENCH_FN(libsrt_string_search_easymatch_long_1b, count[i], nread[i], delete_all[i]);
		BENCH_FN(c_string_search_easymatch_long_1b, count[i], nread[i], delete_all[i]);
		BENCH_FN(cxx_string_search_easymatch_long_1b, count[i], nread[i], delete_all[i]);
		BENCH_FN(libsrt_string_search_easymatch_long_2a, count[i], nread[i], delete_all[i]);
		BENCH_FN(c_string_search_easymatch_long_2a, count[i], nread[i], delete_all[i]);
		BENCH_FN(cxx_string_search_easymatch_long_2a, count[i], nread[i], delete_all[i]);
		BENCH_FN(libsrt_string_search_easymatch_long_2b, count[i], nread[i], delete_all[i]);
		BENCH_FN(c_string_search_easymatch_long_2b, count[i], nread[i], delete_all[i]);
		BENCH_FN(cxx_string_search_easymatch_long_2b, count[i], nread[i], delete_all[i]);
		BENCH_FN(libsrt_string_search_hardmatch_long_1a, count[i], nread[i], delete_all[i]);
		BENCH_FN(c_string_search_hardmatch_long_1a, count[i], nread[i], delete_all[i]);
		BENCH_FN(cxx_string_search_hardmatch_long_1a, count[i], nread[i], delete_all[i]);
		BENCH_FN(libsrt_string_search_hardmatch_long_1b, count[i], nread[i], delete_all[i]);
		BENCH_FN(c_string_search_hardmatch_long_1b, count[i], nread[i], delete_all[i]);
		BENCH_FN(cxx_string_search_hardmatch_long_1b, count[i], nread[i], delete_all[i]);
		BENCH_FN(libsrt_string_search_hardmatch_short_1a, count[i], nread[i], delete_all[i]);
		BENCH_FN(c_string_search_hardmatch_short_1a, count[i], nread[i], delete_all[i]);
		BENCH_FN(cxx_string_search_hardmatch_short_1a, count[i], nread[i], delete_all[i]);
		BENCH_FN(libsrt_string_search_hardmatch_short_1b, count[i], nread[i], delete_all[i]);
		BENCH_FN(c_string_search_hardmatch_short_1b, count[i], nread[i], delete_all[i]);
		BENCH_FN(cxx_string_search_hardmatch_short_1b, count[i], nread[i], delete_all[i]);
		BENCH_FN(libsrt_string_search_hardmatch_long_2, count[i], nread[i], delete_all[i]);
		BENCH_FN(c_string_search_hardmatch_long_2, count[i], nread[i], delete_all[i]);
		BENCH_FN(cxx_string_search_hardmatch_long_2, count[i], nread[i], delete_all[i]);
		BENCH_FN(libsrt_string_search_hardmatch_long_3, count[i], nread[i], delete_all[i]);
		BENCH_FN(c_string_search_hardmatch_long_3, count[i], nread[i], delete_all[i]);
		BENCH_FN(cxx_string_search_hardmatch_long_3, count[i], nread[i], delete_all[i]);
		BENCH_FN(libsrt_string_loweruppercase_ascii, count[i], nread[i], delete_all[i]);
		BENCH_FN(c_string_loweruppercase_ascii, count[i], nread[i], delete_all[i]);
		BENCH_FN(cxx_string_loweruppercase_ascii, count[i], nread[i], delete_all[i]);
		BENCH_FN(libsrt_string_loweruppercase_utf8, count[i], nread[i], delete_all[i]);
		BENCH_FN(c_string_loweruppercase_utf8, count[i], nread[i], delete_all[i]);
		BENCH_FN(cxx_string_loweruppercase_utf8, count[i], nread[i], delete_all[i]);
	}
	return 0;
}
