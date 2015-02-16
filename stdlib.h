extern void qsort(void *base, size_t n, size_t size, int (*cmp)(const void *, const void *));
extern void qsort_s(void *base, size_t n, size_t size, int (*cmp)(const void *, const void *, void *), void *context);

extern void *bsearch(const void *key, const void *base, size_t n, size_t size, int (*cmp)(const void *, const void *));
extern void *bsearch_s(const void *key, const void *base, size_t n, size_t size, int (*cmp)(const void *, const void *, void *), void *context);