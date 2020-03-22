ciphertext = 'MYAMRARUYIQTENCTORAHROYWDSOYEOUARRGDERNOGW'

def decrypt(ciphertext, m, n):
    plaintext = ""
    for i in range(m):
        for j in range(n):
            plaintext += ciphertext[j * m + i]
    return plaintext.lower()

if __name__ == '__main__':
    print(decrypt(ciphertext, 7, 6))