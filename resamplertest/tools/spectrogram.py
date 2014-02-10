# -*- coding: utf-8 -*-
"""
Copyright (c) 2014 "Marcel Thraenhardt"

 spectrogram.py is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

 Muroa is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
"""
"""
mt 2014-01-31
short replacement for sndfile-spectrum
### TODOs ###
- done: correct scaling to 0db (depends on integral of win function)
- noise floor seems higher than in sndfile-spectrogram 
- command line switches
- calculate correct xticks, yticks
- (x,y)labels! 
- remove debugging plots
- other wave reader class? we'll need information if wave format is float oder 16bit
"""

import numpy as np
import matplotlib.pyplot as plt
import scipy.io.wavfile as wav
import scipy.fftpack as fft
from time import sleep

def main():
    (rate,data)=wav.read("sweep.wav",mmap=False)
    data = data.view('float32')
    data = data/np.max(data) # normalize
    print rate
    print len(data)
    print type(data)
    
    plt.plot(data[:1000])
    plt.show()
    # 
    fftlen = 1024
    
    dim_t = int(np.floor(len(data)/fftlen))
    spectrum = np.zeros((dim_t,fftlen),dtype='cfloat') # c16 = 128bit complex
    window = np.kaiser(fftlen,20)
    for ii in range(dim_t):
        istart = ii*fftlen 
        istop = istart+fftlen
        spectrum[ii] = fft.fft(data[istart:istop]*window)/fftlen*2/(np.sum(window)/fftlen) 
        if ii == 200:
            plt.plot(np.abs(spectrum[ii]))
            plt.show()
    
    fig = plt.figure()
    ax = fig.add_subplot(111)
    logabsspec = 20*np.log10(np.abs(spectrum))    
    plt.imshow(np.transpose(np.fliplr(logabsspec)),cmap='hot',vmin=-180,vmax=0)
    ax.set_aspect('equal')
    
    cax = fig.add_axes([0.12, 0.1, 0.78, 0.8])
    cax.get_xaxis().set_visible(False)   
    cax.get_yaxis().set_visible(False) 
    cax.patch.set_alpha(0)
    cax.set_frame_on(False)
    plt.colorbar(orientation='vertical')
    plt.show()
    
    print np.max(logabsspec)
if __name__ == "__main__":
    main()
