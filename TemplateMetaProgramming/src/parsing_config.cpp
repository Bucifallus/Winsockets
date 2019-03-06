#include "parsing_config.hpp"

//actual implementation
//const int mainBlockHdr_startAddr = 0x40000;
//const char l_lchd_BlockHdr_Name[] = "LCHD";


//note that this value could be obtained from a linker symbol but used hard coded as
//cross binaries consistency, this address is the single hardcoded required entry point
#define BLOCK_HEADER_START 0X1CA00300
#define BH_LONG_ID_MASK_LE 0x00000080
#define SWAP32(v)   ((int)((((int)(v)>>24)&0xFF) | (((int)(v)>> 8)&0xFF00) | (((int)(v)&0xFF)<<24) | (((int)(v)&0xFF00)<< 8)))
#define BIG_2_LITTLE(v)    (SWAP32(*((int  *)(v))))

//TODO share this function as utils
//function to convert the hex value read from memory(f_hex_name_u32) to ascii(stored in f_Name_arr_p)
int char4_to_uint32(const char *f_Name_arr_p)
{
    return  (int)f_Name_arr_p[0] << 24 |
            (int)f_Name_arr_p[1] << 16 |
            (int)f_Name_arr_p[2] <<  8 |
            (int)f_Name_arr_p[3];
}

int char4_to_uint32_swap(const char *f_Name_arr_p)
{
    return  (int)f_Name_arr_p[3] << 24 |
            (int)f_Name_arr_p[2] << 16 |
            (int)f_Name_arr_p[1] <<  8 |
            (int)f_Name_arr_p[0];
}

namespace sysconfig
{   
    
    int const g_LCHD = 0x4448434c;// "LCHD" in little endien, ready for compare
    static int* g_blockheader_start_p = 0x0;

    void set_blockheader_start(int* start_address_p)
    {
        g_blockheader_start_p = start_address_p;
    }

    bool get_id(const char* f_secName_p, const char* f_IdName_p, int &f_val)
    {
        bool result = false;
        int sec_Name = char4_to_uint32_swap(f_secName_p);
        int Id_Name = char4_to_uint32_swap(f_IdName_p);

        if(g_blockheader_start_p == 0x0)
        {
            return false;
        }
        int * p_search = g_blockheader_start_p;
        
        //Main Blockheader identifier check        
        if((*p_search++) != g_LCHD)
        {
            //trace::trace_mprintf("BH>ERROR : Header Identifier not found\n");
            return false;
        }
        int bh_size_bytes = BIG_2_LITTLE(p_search);//macro does not support ++
        p_search++;// same as +=4 for an int
        int * bh_end = g_blockheader_start_p + (bh_size_bytes/4);
        //TODO signature / CRC check
        //---------------------- Find the section name ----------------------
        while((p_search < bh_end) && !result)//not endless as of guaranteed p_search increment
        {
            int * sec_start = p_search;
            int sec_size_bytes = BIG_2_LITTLE((p_search+1));// keep the pointer on the name for the check
            if(sec_size_bytes == 0)
            {
                //trace::trace_mprintf("BH>ERROR : Corrupted Header, section size zero\n");
                return false;
            }
            if((*p_search) == sec_Name )
            {
                p_search+=2;///skip x2 uint32 : the name and size already parsed
                int * sec_end = sec_start + (sec_size_bytes/4);
                while((p_search < sec_end) && !result)
                {
                    if((*p_search) == Id_Name)//short id is matching without mask
                    {
                        p_search++;                     //now pointing on data
                        f_val = BIG_2_LITTLE(p_search); //pointed data value
                        result = true;
                    }
                    else//now jumping over the id.data depend whether it is a short or long id
                    {
                        if( (*p_search++) & BH_LONG_ID_MASK_LE )//if it is a long id , with ++ point on size
                        {
                            int long_id_size = BIG_2_LITTLE(p_search);//cannot ++ here due to macro
                            p_search++;//now pointing on long id data, but do not need it in this get (short) id function
                            int long_id_padded_size;
                            int mod = long_id_size % 4;
                            if(mod != 0)    //in this case a padding has really to be added
                            {
                                long_id_padded_size = long_id_size + 4 - mod;
                            }
                            else            //in this case no padding is added
                            {
                                long_id_padded_size = long_id_size;
                            }
                            p_search += (long_id_padded_size/4);//from bytes to uin32_t
                        }
                        else//increment the size of a short id which is 4 Bytes = x1 int
                        {
                            p_search++;
                        }
                    }
                }
            }
            else
            {
                p_search += (sec_size_bytes/4);
            }
        }

        return result;
    }

   //parse function to fetch the section info
    bool get_long_id(const char* f_secName_p, const char* f_IdName_p, short *f_buffer,int f_buf_size)
    {
        bool result = false;
        int sec_Name = char4_to_uint32_swap(f_secName_p);
        int Id_Name = char4_to_uint32_swap(f_IdName_p);

        if(g_blockheader_start_p == 0x0)
        {
            return false;
        }
        int * p_search = g_blockheader_start_p;
        
        //Main Blockheader identifier check        
        if((*p_search++) != g_LCHD)
        {
            //trace::trace_mprintf("BH>ERROR : Header Identifier not found\n");
            return false;
        }
        int bh_size_bytes = BIG_2_LITTLE(p_search);//macro does not support ++
        p_search++;// same as +=4 for an int
        int * bh_end = g_blockheader_start_p + (bh_size_bytes/4);
        //TODO signature / CRC check
        //---------------------- Find the section name ----------------------
        while((p_search < bh_end) && !result)//not endless as of guaranteed p_search increment
        {
            int * sec_start = p_search;
            int sec_size_bytes = BIG_2_LITTLE((p_search+1));// keep the pointer on the name for the check
            if(sec_size_bytes == 0)
            {
                //trace::trace_mprintf("BH>ERROR : Corrupted Header, section size zero\n");
                return false;//direct return with false
            }
            if((*p_search) == sec_Name )
            {
                p_search+=2;///skip x2 uint32 : the name and size already parsed
                int * sec_end = sec_start + (sec_size_bytes/4);
                while((p_search < sec_end) && !result)
                {
                    if((*p_search) == (Id_Name | BH_LONG_ID_MASK_LE))//The long id bit is set
                    {
                        p_search++;                     //now pointing on long id size
                        if(f_buf_size == BIG_2_LITTLE(p_search))
                        {
                            p_search++;                     //now pointing on long id buffer
                            char* p_cpy = (char*)p_search;
                            char* p_cpy_end = p_cpy + f_buf_size;
                            while(p_cpy<p_cpy_end)
                            {
                                *f_buffer++ = *p_cpy++;
                            }
                            result=true;
                        }
                        else
                        {
                            return false;//return an error as the size is mismatching - direct return with false
                        }
                    }
                    else//now jumping over the id.data depend whether it is a short or long id
                    {
                        if( (*p_search++) & BH_LONG_ID_MASK_LE )//if it is a long id , with ++ point on size
                        {
                            int long_id_size = BIG_2_LITTLE(p_search);//cannot ++ here due to macro
                            p_search++;//now pointing on long id data, but do not need it in this get (short) id function
                            int long_id_padded_size;
                            int mod = long_id_size % 4;
                            if(mod != 0)    //in this case a padding has really to be added
                            {
                                long_id_padded_size = long_id_size + 4 - mod;
                            }
                            else            //in this case no padding is added
                            {
                                long_id_padded_size = long_id_size;
                            }
                            p_search += (long_id_padded_size/4);//from bytes to uin32_t
                        }
                        else//increment the size of a short id which is 4 Bytes = x1 int
                        {
                            p_search++;
                        }
                    }
                }
            }
            else
            {
                p_search += (sec_size_bytes/4);
            }
        }

        return result;
    }
}
