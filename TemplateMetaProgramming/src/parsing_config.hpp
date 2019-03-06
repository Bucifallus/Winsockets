#ifndef PARSING_CONFIG_HPP_INCLUDED
#define PARSING_CONFIG_HPP_INCLUDED

namespace sysconfig 
{
    //required to initialise the entry of the blockheader to the system specific address
    void set_blockheader_start(int* start_address);
    //parse function to fetch the section info
    bool get_id(const char* f_secName_p, const char* f_IdName_p, int &f_val);
    //parse function to fetch the section info
    bool get_long_id(const char* f_secName_p, const char* f_IdName_p, short *f_buffer,int f_buf_size);
}
#endif
