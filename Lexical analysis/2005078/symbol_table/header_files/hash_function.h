unsigned long long sdbm_hash(const char* str) {
    unsigned long long hash = 0;
    int c;

    while ((c = *str++)) {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }
    
    return hash;
}