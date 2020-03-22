import numpy as np
import consts as c

ciphertext = 'BEEAKFYDJXUQYHYJIQRYHTYJIQFBQDUYJIIKFUHCQD'
asciiCiphertext = np.fromstring(ciphertext, dtype = np.uint8)

for i in range(len(asciiCiphertext)):
    asciiCiphertext[i] -= ord('A')
# print(asciiCiphertext)
for i in range(26):
    print("trying key: ", i)
    plaintext = ""
    for c in asciiCiphertext:
        plaintext += chr((c + i) % 26 + ord('a'))
    print("plaintext: " + plaintext)
     