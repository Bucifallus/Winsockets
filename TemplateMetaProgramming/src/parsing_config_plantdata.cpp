#include "parsing_config.hpp"

#define PLANT_DATA_SIZE 0x1000 // 4k bytes - size of valid plant data
short swap_u16 ( short data_u16 )
{
  return short( ((data_u16>> 8)&0xFF) | ((data_u16 & 0x00FF)<< 8));
}
namespace sysconfig
{   
    int const g_HEAD = 0x44414548;        // "HEAD" in little endien, starting ID of plant data
    int const g_CHKS = 0x53484B43;        // "CHKS" in little endien, last ID of plant data - checksum
    int g_val = 0;
    static int* g_plantdata_start_p = 0x0;
    bool set_plantdata_start_adr()
    {
        // use the block header API to fetch the start adress of Plant data 
        // PLDT - section name , STAF - ID name in block header 
        #ifdef NEW_R6_LAYOUT_USING_BLOCKHEADER
        if((sysconfig::get_id("PLDT", "STAF", g_val)))
        {
            g_plantdata_start_p = ((int*)g_val);
            return true;
        }
        else 
        {
            return false;
        }
        #else
            /*Woraround explained in ticket NRCSTWO-47395 : This must not go to develop as it is an old layout compatibility
                R5 stays with layout of R4
                Develop has already a new layout
                in the new layout, the blockheader takes the address where the entry point of the application was before
            */
            g_plantdata_start_p = ((int*)0x4000);//0x4000 is the R4/R5 layout for Plant data
            return true;
        #endif
}
    //parser function to read the data from plant data strcuture by ID and length 
    bool get_plantdata( int const f_IdName_p, short *f_buffer,short f_buf_size)
    {
        bool result = false;
        int Id_Name = (f_IdName_p);
        if(g_plantdata_start_p == 0x0)
        {
            return false;
        }
        int * p_search = g_plantdata_start_p;
        //check if HEAD is the Structure Header of plant data  
        if((*p_search++) != g_HEAD)
        {
            //trace::trace_mprintf("BH>ERROR : Header Identifier not found\n");
            return false;
        }
        int pd_size_bytes = PLANT_DATA_SIZE; //size of total vaild plant data - 4096 bytes
        int * pd_end      = g_plantdata_start_p + (pd_size_bytes);
        p_search+=3; //pointing to the address 0x10 where the actual data starts 
        while((p_search < pd_end) && !result)  //loop through the whole structure till the end address 
        {  
            if((*p_search) == Id_Name) //check for the required ID
            {
                p_search++; //pointing on id size
                short* p_search_u16  = reinterpret_cast<short*>(p_search); // pointer to retreive u16 length
                if(f_buf_size == swap_u16(*p_search_u16)) // swap for big endian to little endian 
                {
                    p_search_u16++;                     //now pointing on id's data
                    char* p_cpy = (char*)p_search_u16;
                    char* p_cpy_end = p_cpy + f_buf_size;
                    while(p_cpy<p_cpy_end)
                    {
                        *f_buffer++ = *p_cpy++; // copy the data 
                    }
                    result=true;
                }
                else
                {
                    return false;//return an error as the size is mismatching - direct return with false
                }
            }
            else if ((*p_search) == g_CHKS)// check if the last ID - checksum is reached 
            {
                return false;
            }
            else 
            {
                *p_search++;                        // with ++ point on size of the ID
                short*  p_search_u16 = reinterpret_cast<short*>(p_search);
                int id_size = swap_u16(* p_search_u16);
                p_search_u16++; //now pointing on  id data
                char *p_search_ch = reinterpret_cast<char*>( p_search_u16); // pointer to pass through the not required data and padding 
                int id_padded_size;
                int mod = ((6+id_size)%16); // 6 bytes= 4 byte ID + 2 byte length
                if(mod==0)
                {
                    id_padded_size = id_size;
                }
                else 
                {   
                    id_padded_size = id_size + (16 - mod);
                }   
                p_search_ch += id_padded_size;//pointing to the next ID
                p_search = reinterpret_cast<int*>( p_search_ch);
            }
        }
        return result;
    }
}
