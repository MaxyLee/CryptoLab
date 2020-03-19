ciphertext = 'KQEREJEBCPPCJCRKIEACUZBKRVPKRBCIBQCARBJCVECUPKRIOFKPACUZQEPBKRXPEIIEABDKPBCPFCDCCAFIEABDKPBCPFEQPKAZBKRHAIBKAPCCIBURCCDKDCCJCIDFUIXPAFFERBICZDFKABICBBENEFCUPJCVKABPCYDCCDPKBCOCPERKIVKSCPICBRKIJPKABI'
rev = {1:1, 3:9, 5:21, 7:15, 9:3, 11:19, 15:7, 17:23, 19:11, 21:5, 23:17, 25:25}
probability = [0.082, 0.015, 0.028, 0.043, 0.127,      
                        0.022, 0.02, 0.061, 0.07, 0.002, 
                        0.008, 0.04, 0.024, 0.067,0.075,
                        0.019, 0.001, 0.06, 0.063, 0.091,
                        0.028, 0.01, 0.023, 0.001, 0.02, 
                        0.001]

def get_frequency(text):
    frequency = [0] * 26

    for c in text:
        frequency[ord(c.upper()) - ord('A')] += 1

    fre = ""
    for i in range(26):
        fre += (c + ": " + str(frequency[i]) + " ")
    # print(fre)
    return frequency

def decrypt(ciphertext, key):
    plaintext = ""

    for c in ciphertext:
        plaintext += chr(int((ord(c) - ord('A') - key[1]) * rev[key[0]]) % 26 + ord('a'))
    
    return plaintext

def enum(ciphertext):
    for i,_ in rev.items():
        for j in range(26):
            mykey = [i, j]
            mytext = decrypt(ciphertext, mykey)
            myfrequency = get_frequency(mytext)
            length = len(mytext)
            M = 0

            for k in range(26):
                M += probability[k] * myfrequency[k]
            M /= length
            if M > 0.06:
                print(i, j, M)
                print(mytext)


if __name__ == "__main__":
    # get_frequency(ciphertext)
    enum(ciphertext)
    # print(decrypt(ciphertext,key))