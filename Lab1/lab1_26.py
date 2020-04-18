ciphertext = 'MYAMRARUYIQTENCTORAHROYWDSOYEOUARRGDERNOGW'

def decrypt(ciphertext, m, n):
    plaintext = ""
    for i in range(m):
        for j in range(n):
            plaintext += ciphertext[j * m + i]
    return plaintext.lower()

if __name__ == '__main__':
    # print(decrypt(decrypt(ciphertext, 3, 14), 7, 6))
    print(decrypt(ciphertext, 3, 14))