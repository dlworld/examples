def split_msg(msg, limit):
    n = len(msg)
    for b in range(1, n + 1):
        suf_len = len(f"<{b}/{b}>")
        if suf_len >= limit:
            return []

        valid = True
        parts = []
        a = 1
        i = 0
        while a <= b and i < n:
            suf = f"<{a}/{b}>"
            suf_len = len(suf)
            part_len = limit - suf_len
            part = msg[i:i + part_len]
            parts.append(part + suf)
            a += 1
            i += part_len

        if n == i:
            return parts


m="this is really a very awesome message"
limit = 9
print(split_msg(m, limit))
        
