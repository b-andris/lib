#include <string.h>
#include <stdint.h>

static int cmph(const void* a, const void* b, int (*cmp)(const void*, const void*)) {
	return cmp(a, b);
}

static size_t pivot(void* base, size_t n, size_t size, int (*cmp)(const void*, const void*, void*), void* context) {
	int8_t* data = base;
	if (cmp(data,data + (n - 1) * size, context) < 0) {
		if (cmp(data,data + n / 2 * size, context) < 0) {
			if (cmp(data + n / 2 * size,data + (n - 1) * size, context) < 0) return n / 2;
			else return n - 1;
		}
	} else {
		if (cmp(data,data + n / 2 * size, context) > 0) {
			if (cmp(data + n / 2 * size,data + (n - 1) * size, context) < 0) return n - 1;
			else return n / 2;
		}
	}
	return 0;
}

static void swap(void* base, size_t size, size_t i, size_t j) {
	int8_t* data = base;
	if (i == j) return;
	char tmp[size];
	memcpy(tmp, data + (size * i), size);
	memcpy(data + (size * i), data + (size * j), size);
	memcpy(data + (size * j), tmp, size);
}

void qsort_s(void* base, size_t n, size_t size, int (*cmp)(const void*, const void*, void*), void* context) {
	int8_t* data = base;
	size_t count = n;
	size_t largestPivotIndex = 0;
	while (n > 2) {
		size_t pivotIndex = pivot(data, count, size, cmp, context);
		swap(data, size, 0, pivotIndex);
		size_t i = 1;
		size_t j = count - 1;
		while (i < j) {
			while (i < j && cmp(data + (i * size), data, context) <= 0) i++;
			while (i < j && cmp(data + (j * size), data, context) > 0) j--;
			swap(data, size, i, j);
		}
		if (count == 1) pivotIndex = 0;
		else if (cmp(data + (i * size), data, context) <= 0) pivotIndex = i;
		else pivotIndex = i - 1;
		swap(data, size, 0, pivotIndex);

		if (count < n) swap(data, size, pivotIndex + 1, count);
		else largestPivotIndex = pivotIndex;
		count = pivotIndex;
		while (count <= 2 && n > 2) {
			if (count == 2 && cmp(data, data + size, context) > 0) swap(data, size, 0, 1);
			data += size * (count + 1);
			n -= count + 1;
			largestPivotIndex -= count + 1;
			if (largestPivotIndex) {
				for (count = 0; (cmp(data, data + ((count + 1) * size), context) >= 0) && (count + 1 < n) && (count + 1 < largestPivotIndex); count++) ;
				swap(data, size, 0, count);
			} else {
				data += size;
				count = --n;
			}
		}
	}
	if (n == 2 && cmp(data, data + size, context) > 0) swap(data, size, 0, 1);
}

void* bsearch_s(const void* key, const void* base, size_t n, size_t size, int (*cmp)(const void*, const void*, void*), void* context) {
	const int8_t* data = base;
	if (!n) return NULL;
	size_t min = 0;
	size_t max = n - 1;
	size_t i = (min + max) / 2;
	while (1) {
		int r = cmp(data + (size * i), key, context);
		if (r && min + 1 >= max) return NULL;
		if (r < 0) {
			min = i;
			i = (min + max + 1) / 2;
		} else if (r > 0) {
			max = i;
			i = (min + max) / 2;
		} else {
			return (void*)data + (size * i);
		}
	}
}

void qsort(void* base, size_t n, size_t size, int (*cmp)(const void*, const void*)) {
	qsort_s(base, n, size, (int (*)(const void*, const void*, void*))cmph, cmp);
}

void* bsearch(const void* key, const void* base, size_t n, size_t size, int (*cmp)(const void*, const void*)) {
	return bsearch_s(key, base, n, size, (int (*)(const void*, const void*, void*))cmph, cmp);
}