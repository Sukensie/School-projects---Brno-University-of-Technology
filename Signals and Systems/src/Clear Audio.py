import math
from scipy.io import wavfile
import numpy as np
import matplotlib.pyplot as plt
import IPython
from scipy.signal import spectrogram, freqz, tf2zpk, buttord, butter, bilinear, freqz, impulse, freqs, iirnotch, filtfilt

#-----------------------------------
#           4.1 Základy
#-----------------------------------
fs, data = wavfile.read('../xsouce15.wav')
data.min(), data.max()
dataRaw = data

#normalizace
data = data / 2**15
data.min(), data.max()

seconds = data.size /fs
secondsArr = (np.arange(data.size)/fs)

print("delka v sekundach = " + str(seconds))
print("delka ve vzorcích = " + str(data.size))

print("minimální hodnota = " + str(data.min()))
print("maximální hodnota = " + str(data.max()))


plt.plot(secondsArr, data)
plt.gca().set_xlabel('$t[s]$')
plt.gca().set_title('Úloha 4.1')
plt.margins(0,0)
plt.show()


#-----------------------------------
#    4.2 Předzpracování a rámce
#-----------------------------------
print('')
print("stredni hodnota = " + str(data.mean()))
data = data - data.mean() #odečtení střední hodnoty

data = data / np.abs(data.max()) #normalizace mezi -1 a 1



n = 1024
data = [data[i:i + n] for i in range(0, len(data), n)] #rozsekání pole, ale převede poté na list
data = np.asarray(data, dtype=object) #konvertuje list na pole... zkusit dtype nahradit float64


pocetRamcu = data.size
print("pocet rámců = " + str(pocetRamcu))

#překrývání vzorků
indexX = 0
for x in data:
    indexY = 0
    for y in x:
        if (indexY >= 512):
            break
        else:
            if(indexX > 0):
                data[indexX][indexY] = data[indexX-1][indexY+511]
                indexY += 1
    indexX += 1




seconds2 = data[0].size / fs
secondsArr2 = (np.arange(data[0].size)/fs)

#plt.plot(secondsArr2, data[8]) #tahle je good, ale i 12

plt.plot(secondsArr2, data[8])

plt.gca().set_xlabel('$t[s]$')
plt.gca().set_title('Úloha 4.2')
plt.margins(0,0)
plt.show()


#-----------------------------------
#            4.3 DFT
#-----------------------------------
N = 1024

k = 0
xn = 0

xk = [0 for i in range(N)]


for k in range (N-1):
    n = 0
    for xn in data[8]:
        xk[k] = xk[k] +(xn * (math.e ** (-(2j*math.pi/N)*k*n)))
        n += 1
    xn = 0




s_seg_spec = np.fft.fft(data[8])
G = 10 * np.log10(1/N * np.abs(s_seg_spec)**2)

G = np.abs(xk)
f = np.arange(G.size) / N * fs

# zobrazujeme prvni pulku spektra
plt.plot(f[:f.size//2+1], G[:G.size//2+1])
plt.gca().set_xlabel('$f[Hz]$')
plt.gca().set_title('Úloha 4.3 [dB]')
plt.grid(alpha=0.3, linestyle='--')
plt.margins(0,0)
plt.show()



#-----------------------------------
#         4.4 Spektrogram
#-----------------------------------

f, t, sgr = spectrogram(dataRaw, fs, nperseg=1024, noverlap=512)
# prevod na PSD
# (ve spektrogramu se obcas objevuji nuly, ktere se nelibi logaritmu, proto +1e-20)
sgr_log = 10 * np.log10(sgr+1e-20)



plt.pcolormesh(t,f,sgr_log)
plt.gca().set_title('Úloha 4.4')
plt.gca().set_xlabel('Čas [s]')
plt.gca().set_ylabel('Frekvence [Hz]')
cbar = plt.colorbar()
cbar.set_label('Spektralní hustota výkonu [dB]', rotation=270, labelpad=15)

plt.tight_layout()
plt.show()

#-----------------------------------
#  4.5 Určení rušivých frekvencí
#-----------------------------------
distrortedFrequencies = [1000, 2000, 3000, 4000]
for i in range(len(distrortedFrequencies)):
    if i == 0:
        continue

    if (distrortedFrequencies[i] % distrortedFrequencies[0] == 0):
        print("f"+str(i+1)+" je harmonicky vztažená")




#-----------------------------------
#      4.6 Generování signálu
#-----------------------------------

t = np.linspace(0, seconds, dataRaw.size)

y = []
for i in range(len(distrortedFrequencies)):
    y.append(np.cos(2 * math.pi * distrortedFrequencies[i] * t))

cosineMerged = sum(y)

wavfile.write('../audio/4cos.wav', fs, cosineMerged)


f, t, sgr = spectrogram(cosineMerged, fs, nperseg=1024, noverlap=512)
sgr_log = 10 * np.log10(sgr+1e-20)

plt.pcolormesh(t,f,sgr_log)
plt.gca().set_title('Úloha 4.6')
plt.gca().set_xlabel('Čas [s]')
plt.gca().set_ylabel('Frekvence [Hz]')
cbar = plt.colorbar()
cbar.set_label('Spektralní hustota výkonu [dB]', rotation=270, labelpad=15)

plt.tight_layout()
plt.show()


#-----------------------------------
#       4.7 Čistící filtr
#-----------------------------------


n = []
omega = []
b = []
a = []
for i in range(len(distrortedFrequencies)):
    ni, omegai = buttord([(distrortedFrequencies[i]-50)/(fs/2), (distrortedFrequencies[i]+50)/(fs/2)], [(distrortedFrequencies[i]-30/2)/(fs/2), (distrortedFrequencies[i]+30/2)/(fs/2)], 3, 40, False)
    n.append(ni)
    omega.append(omegai)
    bi, ai = butter(n[i],omega[i], 'bandstop', False)
    a.append(ai)
    b.append(bi)

# impulsni odezva
N_imp = 40
imp = [1, *np.zeros(N_imp-1)] # jednotkovy impuls
h = filtfilt(b[3], a[3], imp)



plt.figure(figsize=(5,3))
plt.stem(np.arange(N_imp), h, basefmt=' ')
plt.gca().set_xlabel('$n$')
plt.gca().set_title('4.7 Impulsní odezva $h[n]$')
plt.grid(alpha=0.5, linestyle='--')
plt.tight_layout()
plt.show()

#-----------------------------------
#       4.8 Nuly a póly
#-----------------------------------
zeros, poles, gain = tf2zpk(b[0], a[0])

is_stable = (poles.size == 0) or np.all(np.abs(poles) < 1)
if (is_stable):
    print("filtr je stabilni")

plt.figure(figsize=(4,4))

# jednotkova kruznice
ang = np.linspace(0, 2*np.pi,100)
plt.plot(np.cos(ang), np.sin(ang))

# nuly, poly
plt.scatter(np.real(zeros), np.imag(zeros), marker='o', facecolors='none', edgecolors='r', label='nuly')
plt.scatter(np.real(poles), np.imag(poles), marker='x', color='g', label='póly')
plt.gca().set_xlabel('Realná složka $\mathbb{R}\{$z$\}$')
plt.gca().set_ylabel('Imaginarní složka $\mathbb{I}\{$z$\}$')

plt.grid(alpha=0.5, linestyle='--')
plt.legend(loc='upper right')

plt.tight_layout()
plt.show()



#-----------------------------------
#  4.9 Frekvenční charakteristika
#-----------------------------------
w, H = freqz(b[3], a[3])

_, ax = plt.subplots(1, 2, figsize=(8,3))

ax[0].plot(w / 2 / np.pi * fs, np.abs(H))
ax[0].set_xlabel('Frekvence [Hz]')
ax[0].set_title('Modul frekvenční charakteristiky $|H(e^{j\omega})|$')

ax[1].plot(w / 2 / np.pi * fs, np.angle(H))
ax[1].set_xlabel('Frekvence [Hz]')
ax[1].set_title('Argument frekvenční charakteristiky $\mathrm{arg}\ H(e^{j\omega})$')



plt.tight_layout()
plt.show()



#-----------------------------------
#         4.10 Filtrace
#-----------------------------------

filter0 = filtfilt(b[0], a[0], dataRaw)
filter1 = filtfilt(b[1], a[1], filter0)
filter2 = filtfilt(b[2], a[2], filter1)
clean = filtfilt(b[3], a[3], filter2)


clean = clean / np.abs(clean.max()) #normalizace mezi -1 a 1

wavfile.write('../audio/clean_bandstop.wav', fs, clean)

plt.plot(secondsArr, clean)
plt.gca().set_xlabel('$t[s]$')
plt.gca().set_title('Úloha 4.10')
plt.margins(0,0)
plt.show()




#tohle nemusí být, ale bude to fajn do protokolu
f, t, sgr = spectrogram(clean, fs, nperseg=1024, noverlap=512)
# prevod na PSD
# (ve spektrogramu se obcas objevuji nuly, ktere se nelibi logaritmu, proto +1e-20)
sgr_log = 10 * np.log10(sgr+1e-20)



plt.pcolormesh(t,f,sgr_log)
plt.gca().set_title('Úloha 4.x')
plt.gca().set_xlabel('Čas [s]')
plt.gca().set_ylabel('Frekvence [Hz]')
cbar = plt.colorbar()
cbar.set_label('Spektralní hustota výkonu [dB]', rotation=270, labelpad=15)

plt.tight_layout()
plt.show()