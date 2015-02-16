#include <string.h>

int cmph(const void *a, const void *b, int (*cmp)(const void *, const void *)) {
	return cmp(a, b);
}

void qsort_s(void *base, size_t n, size_t size, int (*cmp)(const void *, const void *, void *), void *context) {
	if (n < 2) return;
	size_t i = n / 2;
	size_t up = n - 1;
	size_t low = 0;
	while (1) {
		while (low < i && cmp(&base[size * low], &base[size * i], context) <= 0) low++;
		while (up > i && cmp(&base[size * up], &base[size * i], context) >= 0) up--;
		if (up == low) break;
		if (low == i) i = up;
		else if (up == i) i = low;
		char tmp[size];
		memcpy(tmp, &base[size * low], size);
		memcpy(&base[size * low], &base[size * up], size);
		memcpy(&base[size * up], tmp, size);
	}
	qsort_s(base, i, size, cmp, context);
	qsort_s(&base[size * (i + 1)], n - i - 1, size, cmp, context);
}

void *bsearch_s(const void *key, const void *base, size_t n, size_t size, int (*cmp)(const void *, const void *, void *), void *context) {
	if (!n) return NULL;
	size_t min = 0;
	size_t max = n - 1;
	size_t i = (min + max) / 2;
	while (1) {
		int r = cmp(&base[size * i], key, context);
		if (r && min + 1 >= max) return NULL;
		if (r < 0) {
			min = i;
			i = (min + max + 1) / 2;
		} else if (r > 0) {
			max = i;
			i = (min + max) / 2;
		} else {
			return (void*)&base[size * i];
		}
	}
}

void qsort(void *base, size_t n, size_t size, int (*cmp)(const void *, const void *)) {
	qsort_s(base, n, size, (int (*)(const void *, const void *, void *))cmph, cmp);
}

void *bsearch(const void *key, const void *base, size_t n, size_t size, int (*cmp)(const void *, const void *)) {
	return bsearch_s(key, base, n, size, (int (*)(const void *, const void *, void *))cmph, cmp);
}