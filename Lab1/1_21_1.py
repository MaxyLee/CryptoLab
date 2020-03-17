ciphertext = 'EMGLOSUDCGDNCUSWYSFHNSFCYKDPUMLWGYICOXYSIPJCKQPKUGKMGOLICGINCGACKSNISACYKZSCKXECJCKSHYSXCGOIDPKZCNKSHICGIWYGKKGKGOLDSILKGOIUSIGLEDSPWZUGFZCCNDGYYSFUSZCNXEOJNCGYEOWEUPXEZGACGNFGLKNSACIGOIYCKXCJUCIUZCFZCCNDGYYSFEUEKUZCSOCFZCCNCIACZEJNCSHFZEJZEGMXCYHCJUMGKUCY'
plaintext = ""
frequency = {}
sub = {
    'A':'v', 'B':'x', 'C':'e', 'D':'b', 'E':'i',
    'F':'w', 'G':'a', 'H':'f', 'I':'d', 'J':'c',
    'K':'s', 'L':'y', 'M':'m', 'N':'l', 'O':'n',
    'P':'u', 'Q':'j', 'R':'k', 'S':'o', 'T':'z',
    'U':'t', 'V':'q', 'W':'g', 'X':'p', 'Y':'r',
    'Z':'h'
    }

for i in range(26):
    c = chr(i + ord('A'))
    frequency[c] = 0
#     sub[c] = "-"

for c in ciphertext:
    frequency[c] += 1

fre = ""
for i in range(26):
    c = chr(i + ord('A'))
    fre += (c + ": " + str(frequency[c]) + " ")
print(fre)

for c in ciphertext:
    plaintext += sub[c]

print(plaintext)