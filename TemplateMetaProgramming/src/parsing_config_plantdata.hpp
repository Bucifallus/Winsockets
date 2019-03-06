#ifndef PARSING_CONFIG_PLANTDATA_HPP_INCLUDED
#define PARSING_CONFIG_PLANTDATA_HPP_INCLUDED

#include "parsing_config.hpp"

namespace sysconfig 
{
   //required to initialise the start adress of plant data structure 
    bool set_plantdata_start_adr();
    
    //parse function to fetch the data from plant data
    bool get_plantdata(int const f_IdName_p, short *f_buffer,short f_buf_size);

}
#endif
