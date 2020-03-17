ciphertext = 'KCCPKBGUFDPHQTYAVINRRTMVGRKDNBVFDETDGILTXRGUDDKOTFMBPVGEGLTGCKQRACQCWDNAWCRXIZAKFTLEWRPTYCQKYVXCHKFTPONCQQRHJVAJUWETMCMSPKQDYHJVDAHCTRLSVSKCGCZQQDZXGSFRLSWCWSJTBHAFSIASPRJAHKJRJUMVGKMITZHFPDISPZLVLGWTFPLKKEBDPGCEBSHCTJRWXBAFSPEZQNRWXCVYCGAONWDDKACKAWBBIKFTIOVKCGGHJVLNHIFFSQESVYCLACNVRWBBIREPBBVFEXOSCDYGZWPFDTKFQIYCWHJVLNHIQIBTKHJVNPIST'
key = [2,17,24,15,19,14]

def kasiski(ciphertext):
    kasiski = {}
    n = len(ciphertext)
    for i in range(n - 2):
        s = ciphertext[i:i + 3]
        if s == 'HJV':
            print(i - 107)
        if s in kasiski:
            kasiski[s] += 1
        else:
            kasiski[s] = 1

    for key,value in kasiski.items():
        if value > 1:
            print(key + ": " + str(value))

def index_of_coincidence(ciphertext, m):
    split = [""] * m
    length = len(ciphertext)
    num = int(length / m)
    
    for i in range(num * m):
        split[i % m] += ciphertext[i]

    for s in split:
        frequency = {}
        n = len(s)

        for i in range(26):
            frequency[chr(i + ord('A'))] = 0
        
        for c in s:
            frequency[c] += 1
        
        index = 0
        for i in range(26):
            c = chr(i + ord('A'))
            index += frequency[c] * (frequency[c] - 1)
        index /= (n * (n - 1))
        print(index)

def vigenere(ciphertext, m):
    split = [""] * m
    length = len(ciphertext)
    num = int(length / m)
    
    for i in range(num * m):
        split[i % m] += ciphertext[i]
    # print(split)
    
    for i in range(m):
        frequency = [0] * 26
        probability = [0.082, 0.015, 0.028, 0.043, 0.127,      
                        0.022, 0.02, 0.061, 0.07, 0.002, 
                        0.008, 0.04, 0.024, 0.067,0.075,
                        0.019, 0.001, 0.06, 0.063, 0.091,
                        0.028, 0.01, 0.023, 0.001, 0.02, 
                        0.001]
        s = split[i]
        n = len(s)

        print("i:", i)

        for c in s:
            frequency[ord(c) - ord('A')] += 1

        M = [0] * 26
        for g in range(26):
            for j in range(26):
                M[g] += probability[j] * frequency[(j + g) % 26]
            M[g] /= n
            print(" M[" + str(g) + "]: " + str(M[g])[0:5])

def decrypt(ciphertext, key):
    plaintext = ""
    m = len(key)
    for i,c in enumerate(ciphertext):
        plaintext += chr((ord(c) - ord('A') - key[i % m]) % 26 + ord('a'))
    return plaintext
    
if __name__ == "__main__":
    # kasiski(ciphertext)
    # index_of_coincidence(ciphertext, 6)
    # vigenere(ciphertext, 6)
    print(decrypt(ciphertext,key))