/*
Copyright (c) 2002-2014 "Martin Runge"

main.cpp is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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

*/

#include <iostream>
#include <string>

#include <getopt.h>

#include "CSpeexResampler.h"

#include <math.h>

using namespace std;

void usage(string appname);

int createSweep(string filename);

int main(int argc, char** argv)
{

	int c;
    int digit_optind = 0;

    string m_infile, m_outfile, m_resampler;

    static struct option long_options[] = {
        {"infile", 1, 0, 'i'},
        {"outfile", 1, 0, 'o'},
        {"resampler", 1, 0, 'r'},
        {"help", 0, 0, '?'},
        {0, 0, 0, 0}
    };

    while (1) {
        int this_option_optind = optind ? optind : 1;
        int option_index = 0;

        c = getopt_long(argc, argv, "i:o:r:?", long_options, &option_index);
        if (c == -1)
            break;

        switch (c) {
        case 'i':
            if (optarg) {
                m_infile = optarg;
            }
            else {
            	cerr << "--infile option requires an argument" << endl;
            	usage(argv[0]);
            }
            break;

        case 'o':
            if (optarg) {
                m_outfile = optarg;
            }
            else {
            	cerr << "--outfile option requires an argument" << endl;
            	usage(argv[0]);
            }
            break;

        case 'r':
            if (optarg) {
                m_resampler = optarg;
            }
            else {
            	cerr << "--resampler option requires an argument" << endl;
            	usage(argv[0]);
            }
            break;

        case '?':
        	usage(argv[0]);
        	return 1;
            break;

        default:
            printf("?? getopt returned character code 0%o ??\n", c);
            break;
        }
    }

    if (optind < argc) {
        printf("non-option ARGV-elements: ");
        while (optind < argc)
            printf("%s ", argv[optind++]);
        printf("\n");
    }

    int sampleRate = 48000;

    CSpeexResampler* resampler = new CSpeexResampler();
    resampler->openInfile(m_infile);
    resampler->openOutfile(m_outfile, sampleRate);

    resampler->resample();

    resampler->closeInfile();
    resampler->closeOutfile();

    // createSweep("sweep.wav");
}


void usage(string appname) {
	cout << "usage:" << endl;
	cout << appname << " <options>" << endl;
	cout << "  --infile      -i  \tfile with input samples" << endl;
	cout << "  --outfile     -o  \tfile to store resampled samples" << endl;
	cout << "  --resampler   -r  \tresampler to use" << endl;
	cout << "  --help        -?  \tthis message" << endl;
}

int createSweep(string filename) {
    int sampleRate = 44100;

    CSpeexResampler* resampler = new CSpeexResampler();
    resampler->openOutfile(filename, sampleRate);

    const int size = sampleRate * 20;
    int16_t samples[size];

    double startfreq = 100;
    double endfreq = 20000;
    double maxfactor = endfreq / startfreq;
    double stepfactor = pow(maxfactor, (double)1.0/size);
    double freq = startfreq;

    for (int i=0; i<size; i++) {
    	freq *= stepfactor; //1.00001;
    	samples[i] = INT16_MAX * sin(float(i) * 2 * M_PI * freq / sampleRate);
    }
    // resampler->writeOutfile(samples, size);

    delete resampler;
}
