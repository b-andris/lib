#include <string.h>

static int cmph(const void *a, const void *b, int (*cmp)(const void *, const void *)) {
	return cmp(a, b);
}

static size_t getPivotIndex(void *base, size_t n, size_t size, int (*cmp)(const void *, const void *, void *), void *context) {
	if (cmp(base,base + (n - 1) * size, context) < 0) {
		if (cmp(base,base + n / 2 * size, context) < 0) {
			if (cmp(base + n / 2 * size,base + (n - 1) * size, context) < 0) return n / 2;
			else return n - 1;
		}
	} else {
		if (cmp(base,base + n / 2 * size, context) > 0) {
			if (cmp(base + n / 2 * size,base + (n - 1) * size, context) < 0) return n - 1;
			else return n / 2;
		}
	}
	return 0;
}

static void swap(void *base, size_t size, size_t i, size_t j) {
	if (i == j) return;
	char tmp[size];
	memcpy(tmp, base + (size * i), size);
	memcpy(base + (size * i), base + (size * j), size);
	memcpy(base + (size * j), tmp, size);
}

void qsort_s(void *base, size_t n, size_t size, int (*cmp)(const void *, const void *, void *), void *context) {
	size_t count = n;
	size_t largestPivotIndex = 0;
	while (n > 2) {
		size_t pivotIndex = getPivotIndex(base, count, size, cmp, context);
		swap(base, size, 0, pivotIndex);
		size_t i = 1;
		size_t j = count - 1;
		while (i < j) {
			while (i < j && cmp(base + (i * size), base, context) <= 0) i++;
			while (i < j && cmp(base + (j * size), base, context) > 0) j--;
			swap(base, size, i, j);
		}
		if (count == 1) pivotIndex = 0;
		else if (cmp(base + (i * size), base, context) <= 0) pivotIndex = i;
		else pivotIndex = i - 1;
		swap(base, size, 0, pivotIndex);

		if (count < n) swap(base, size, pivotIndex + 1, count);
		else largestPivotIndex = pivotIndex;
		count = pivotIndex;
		while (count <= 2 && n > 2) {
			if (count == 2 && cmp(base, base + size, context) > 0) swap(base, size, 0, 1);
			base += size * (count + 1);
			n -= count + 1;
			largestPivotIndex -= count + 1;
			if (largestPivotIndex) {
				for (count = 0; (cmp(base, base + ((count + 1) * size), context) >= 0) && (count + 1 < n) && (count + 1 < largestPivotIndex); count++) ;
				swap(base, size, 0, count);
			} else {
				base += size;
				count = --n;
			}
		}
	}
	if (n == 2 && cmp(base, base + size, context) > 0) swap(base, size, 0, 1);
}

void *bsearch_s(const void *key, const void *base, size_t n, size_t size, int (*cmp)(const void *, const void *, void *), void *context) {
	if (!n) return NULL;
	size_t min = 0;
	size_t max = n - 1;
	size_t i = (min + max) / 2;
	while (1) {
		int r = cmp(base + (size * i), key, context);
		if (r && min + 1 >= max) return NULL;
		if (r < 0) {
			min = i;
			i = (min + max + 1) / 2;
		} else if (r > 0) {
			max = i;
			i = (min + max) / 2;
		} else {
			return (void*)base + (size * i);
		}
	}
}

void qsort(void *base, size_t n, size_t size, int (*cmp)(const void *, const void *)) {
	qsort_s(base, n, size, (int (*)(const void *, const void *, void *))cmph, cmp);
}

void *bsearch(const void *key, const void *base, size_t n, size_t size, int (*cmp)(const void *, const void *)) {
	return bsearch_s(key, base, n, size, (int (*)(const void *, const void *, void *))cmph, cmp);
}