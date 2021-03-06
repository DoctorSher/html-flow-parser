/* 
 *
 * Notice: this file is a modified version of main.cpp from the true tcpflow.
 * I wanted to simplify it for my moderately narrow purposes.
 * The original license may be found below. -Adam Drescher
 * ---------------------------------------------------------------------
 * This file is a part of tcpflow by Simson Garfinkel <simsong@acm.org>.
 * Originally by Jeremy Elson <jelson@circlemud.org>.
 *
 * This source code is under the GNU Public License (GPL).  See
 * LICENSE for details.
 *
 */

#define __MAIN_C__

#include "tcpflow.h"
#include <vector>
#include <string>

#define ENABLE_GZIP 0

u_int min_skip  = 1000000;
const char *progname = 0;

#ifdef HAVE_PTHREAD
sem_t *semlock = 0;
#endif


int main(int argc, char *argv[])
{
    if (argc < 2 && argc > 3) {
        std::cerr << "Usage: tcpflow PCAP_FILE [OUTPUT_DIRECTORY]" << std::endl;
        exit(EXIT_FAILURE);
    }


    tcpdemux demux;			// the demux object we will be using.
    char *device = NULL;

    std::string filename(argv[1]);
    std::string directory;

    if (argc == 2) {
        size_t offset = filename.rfind("/");
        if (offset != std::string::npos) 
            directory = filename.substr(0,offset);
    } else {
        directory = argv[2];
    }

    progname = argv[0];
    demux.outdir = directory.c_str();

    struct stat sbuf;
    if(stat(demux.outdir.c_str(),&sbuf)==0) {
	if(!S_ISDIR(sbuf.st_mode)) {
	    std::cerr << "outdir is not a directory: " << demux.outdir << "\n";
	    exit(1);
	}
    } else {
	if(mkdir(demux.outdir.c_str(),0777)) {
	    std::cerr << "cannot create " << demux.outdir << ": " << strerror(errno) << "\n";
	    exit(1);
	}
    }

    demux.process_infile(device, filename, true);
    demux.close_all();
    
    return 0;
}

