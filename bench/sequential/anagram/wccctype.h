static inline int wccislower(int c)
{
    return 'a' <= c && c <= 'z';
}

static inline int wccisupper(int c)
{
    return 'A' <= c && c <= 'Z';
}

static inline int wccisalpha(int c)
{
    return wccisupper(c) || wccislower(c);
}

static inline int wcctolower(int c)
{
    return wccisupper(c) ? c + ('a' - 'A') : c;
}
