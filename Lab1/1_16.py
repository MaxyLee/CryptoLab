reper = [1, 3, 5, 0, 7, 2, 4, 6]
ciphertext = "TGEEMNELNNTDROEOAAHDOETCSHAEIRLM"
plaintext = ""
l = int(len(ciphertext) / 8)
# print(ciphertext[0:8])
for i in range(l):
    # s = ciphertext[i * 8:(i + 1) * 8]
    for j in range(8):
        plaintext += ciphertext[i * 8 + reper[j]]
print(plaintext)