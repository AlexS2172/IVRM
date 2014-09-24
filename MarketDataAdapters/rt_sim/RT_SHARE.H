#ifndef _RT_SHARE_H
#define _RT_SHARE_H

// this file contains stuf shared bettween vaious pf and rt apps
// and is availiable to the end user for inclusion in rt_api.h
// and is currently included in rt_api.h ps_types.h pf_rt_q.h 
// rt_pf_q.h rt_talk.h

// !!! this file will be renamed to rt.h and be deprecated

#ifdef __cplusplus
extern "C" {
#endif
                        
#define RT_MAX_EXCHANGES 128
#define RT_SZ_EXCHANGE_BITMAP   (RT_MAX_EXCHANGES/8)

#define MAX_GROUP_REQ_TYPES 7
typedef enum RT_GRP_REQ_TY
{
   RT_GRP_REQ_TY_THIS      , // 0 = just resynch this security
   RT_GRP_REQ_TY_STKOPTS   , // 1 = resynch security and its options
   RT_GRP_REQ_TY_REGIONALS , // 2 = resynch security and its regionals
   RT_GRP_REQ_TY_FUTURS    , // 3 = resynch futures
   RT_GRP_REQ_TY_FUTOPT    , // 4 = resynch futures and all options
   RT_GRP_REQ_TY_INDEX_DVD , // 5 = resynch index dividend data 
   RT_GRP_REQ_TY_ALL         // 6 = resynch security and entire family
}  
                     RT_GRP_REQ_TY;


// use the following list of string for for debug dumping
// eg char group_strings[][30] =  RT_GRP_REQ_TY_STR_INITALIZER ;
// printf("group type = %s\n",group_string[group_type]);
#define RT_GRP_TY_STR_LEN  10    
#define RT_GRP_TY_STR_INITALIZER  \
        {                  \
          { "UNDERLIER" }, \
          { "STKOPTS"   }, \
          { "REGIONALS" }, \
          { "FUTURS"    }, \
          { "FUTOPT"    }, \
          { "INDEX_DVD" }, \
          { "ALL"       }  \
        }



#define MAX_HANDLES_PER_GROUP_MSG 32     // per MTU (Message Transfer Unit ieee 802... sepc)
#define MAX_HANDLES_PER_GROUP     1500   // enough room for valueline

      /*                   Transaction request types.                      */
      /* These are binary values which can be ORed together.  This         */
      /* is sometimes necessary, since if just RT_TALK_REQ_TYPE_MIND is    */
      /* specified, no data will be returned until the current value       */
      /* changes, while if both it and RT_TALK_REQ_TYPE_GET are specified, */
      /* the current value will also be returned.                          */
typedef enum  RT_REQ_TYPE       
{
   RT_REQ_TYPE_GET      = 0x01,  /* Return current values for specified */
                                 /*  data fields; don't "mind" them.    */
   RT_REQ_TYPE_MIND     = 0x02,  /* "Mind" specified data fields; i.e., */
                                 /*  return field data every time any   */
                                 /*  activity occurs on the fields.     */
   RT_REQ_TYPE_GET_MIND = 0x03,  /* Combination of above two types      */

   RT_REQ_VALID_TYPES = RT_REQ_TYPE_GET | RT_REQ_TYPE_MIND     
                                 /* Bits which indicate a valid         */
                                 /*  request type                       */ 
} RT_REQ_TYPE;                 
       
#define RT_REQ_TY_STR_LEN  12    
#define RT_REQ_TY_STR_INITALIZER \
        {                        \
          { "GET"        },      \
          { "MIND"       },      \
          { "GET_&_MIND" }       \
        }




#define MAX_SYMBOL_REQ_LEN    32   


typedef enum                             // valid rt record types                      
{                                        
  e_TD_PF_TYPE_STOCK         ,
  e_TD_PF_TYPE_INDEX         ,
  e_TD_PF_TYPE_OPTION        ,
  e_TD_PF_TYPE_REGIONAL      ,
  e_TD_PF_TYPE_MKTSTAT       ,
  e_TD_PF_TYPE_FUTURE        ,
  e_TD_PF_TYPE_FUT_OPT       ,
  e_TD_PF_TYPE_MRATES        ,             
  e_TD_PF_TYPE_MISC          ,
  e_TD_PF_TYPE_BOND     
}
              te_TD_PF_TYPES ;

#define MAX_TD_PF_TYPES  10               

#define PF_TY_STR_LEN  20    
#define PF_TY_STR_INITALIZER \
        {                   \
          { "STOCK"    }, \
          { "INDEX"    }, \
          { "OPTION"   }, \
          { "REGIONAL" }, \
          { "MKTSTAT"  }, \
          { "FUTURE"   }, \
          { "FUT_OPT"  }, \
          { "MRATES"   }, \
          { "MISC"     }, \
          { "BOND"     }  \
        }


// ticker line trade types
#define TKR_TYPE_RESYNC 0x00
#define TKR_TYPE_BID    0x01    /* valid if quote                             */
#define TKR_TYPE_ASK    0x02
#define TKR_TYPE_QUOTE  0x03             
#define TKR_TYPE_TRADE  0x04
#define TKR_TYPE_NEWS   0x08





           //-------------------------------------------------//
           //                Error return codes               //
           // and which routines can get which return codes   //
           // ("rt_" has been removed from each routine name) //
           //-------------------------------------------------//
//                                               r            
//                                               e            
//                                               q            
//                                               u            
//                                               e            
//                                               s            
//                                               t            
//                                               _            
//                                               f                           i        
//                                               i                           g        
//                                               e                           n       
//                                               l   G                       o        
//                                           r   d   E       g   g           r   r   G  
//                                           e   s   T   g   e   e   r       e   e   E  
//                                           q   _   _   e   t   t   e       _   q   T         
//                                           u   v   F   t   _   _   m       a   u   _         
//                                           e   i   I   _   f   f   o   a   l   e   G          
//                                           s   a   E   f   i   i   v   d   l   s   R          
//                                       s   t   _   L   i   e   e   e   d   _   t   O          
//                                   s   h   _       D   e   l   l   _   _   s   _   U          
//                                   t   u   f   h   S   l   d   d   f   f   y       P         
//                                   a   t   i   a   _   d   _   _   i   i   m   G   _        
//                                   r   d   e   n   C   _   r   c   e   e   b   R   C      
//                                   t   o   l   d   L   i   e   h   l   l   o   O   L      
//                                   u   w   d   l   B   n   a   a   d   d   l   U   B     
//                                   p   n   s   e   K   t   l   r   s   s   s   P   K    
typedef enum  RT_ERR              // |   |   |   |   |   |   |   |   |   |   |   |   | 
  {                               // |   |   |   |   |   |   |   |   |   |   |   |   | 
   RT_ERR_OK      = 0           , // x   x   x   x   x   x   x   x   x   x   x   x   x  
   RT_ERR_BAD_USERNAME          , // x                                                  
   RT_ERR_BAD_USER_ID           , // x                                                  
   RT_ERR_CONNECTION_NOT_OPENED , // x                                                  
   RT_ERR_CONNECTION_NOT_OPEN   , //     x   x   x                   x   x   x   x      
   RT_ERR_CONNECTION_INVALID    , //     x   x   x                   x   x   x   x      
   RT_ERR_SYMBOL_NOT_FOUND      , //                 x                               x  
   RT_ERR_INVALID_REQ_ID        , //                                 x   x              
   RT_ERR_INVALID_FIELD         , //                     x   x   x   x   x              
   RT_ERR_INVALID_CONVERSION    , //                     x   x   x                      
   RT_ERR_NOT_ENTITLED          , //                 x                               x  
   RT_ERR_OUT_OF_STORAGE        , //                                                                   
   RT_ERR_BAD_CALLBACK          , //     x   x   x                               X                     
   RT_ERR_NOT_OK                , // x   x   x   x   x   x   x   x   x   x   x   x   x  
   RT_ERR_INVALID_HANDLE        , // x   x   x   x   x   x   x   x   x   x   x   x   x  
   RT_ERR_UNDEFINED_ERR         , // x   x   x   x   x   x   x   x   x   x   x   x   x  
   RT_ERR_INVALID_RET_CODE        // x   x   x   x   x   x   x   x   x   x   x   x   x  
  }                      RT_ERR ;

#define IS_RT_ERR_VALID(e) ((e) >= RT_ERR_OK && (e) < RT_ERR_INVALID_RET_CODE)


#define RT_ERROR_STRING_LEN 32
#define RT_ERROR_STRING_INITIALIZER \
{                                   \
 {"OK"                         } ,  \
 {"BAD_USERNAME"               } ,  \
 {"BAD_USER_ID"                } ,  \
 {"CONNECTION_NOT_OPENED"      } ,  \
 {"CONNECTION_NOT_OPEN"        } ,  \
 {"CONNECTION_INVALID"         } ,  \
 {"SYMBOL_NOT_FOUND"           } ,  \
 {"INVALID_REQ_ID"             } ,  \
 {"INVALID_FIELD"              } ,  \
 {"INVALID_CONVERSION"         } ,  \
 {"NOT_ENTITLED"               } ,  \
 {"OUT_OF_STORAGE"             } ,  \
 {"INVALID_HANDLE"             } ,  \
 {"UNDEFINED_ERR"              } ,  \
 {"INVALID_RET_CODE"           }    \
}

static _inline char * rt_get_error_string(RT_ERR err)
{
  static char rt_err_strings [][RT_ERROR_STRING_LEN] = RT_ERROR_STRING_INITIALIZER ;
  if(IS_RT_ERR_VALID(err))
    return rt_err_strings[err] ;
  else
    return rt_err_strings[RT_ERR_INVALID_RET_CODE] ;
}



#ifdef __cplusplus
}
#endif


#endif //_RT_SHARE_H
