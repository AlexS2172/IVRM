 
//----------------------------------------------------------------------------//
//                                                                            //
//                       Real Time data server interface                      //
//                                                                            //
//            This is a PRELIMINARY DRAFT and is subject to change.           //
//  main docs are in rt_api.txt and rt_api.ppt                                //
//                                                                            //
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
//                                                                            //
//                                   OVERVIEW                                 //
//                                                                            //
//  The following specification provides application-level access to real-    //
//  time data from the Track Data ticker feeds. Anything from a request for   //
//  a single data field (get last price for "DEC") to a continuous updating   //
//  of all activity on an exchange is supported by this interface. The caller //
//  is completely isolated from any communication protocols or any internal   //
//  data representations.                                                     //
//                                                                            //
//----------------------------------------------------------------------------//
//                                                                            //
//                             SUMMARY OF COMMANDS                            //
//                                                                            //
//  The commands can be divided into three groups:                            //
//                                                                            //
//  1) Initialization/termination                                             //
//     This group of commands establishes and terminates connection to the    //
//     Real Time data server.                                                 //
//       rt_startup  - establishes a connection the the RT server             //
//       rt_shutdown - terminates the connection                              //
//                                                                            //
//  2) Symbol/field specification                                             //
//     These commands tell the RT server for which symbols and which fields   //
//     within the symbols information is desired.  Just the latest data, or   //
//     continuous updating of the data as transactions take place (data       //
//     "minding"), can be requested.  All data is supplied via callback       //
//     routines.                                                              //
//       rt_request_fields     - specifies fields within symbols for which    //
//                               data is desired                              //
//       rt_request_fields_via_handle - same as above, except symbols is      //
//                               specified via an internal "handle"           //
//       RT_GET_FIELDS_CLBK    - callback routine for above two calls         //
//       rt_request_symbols    - get components of a group symbol             //
//       RT_GET_SYMBOLS_CLBK   - callback routine for above call              //
//       rt_remove_fields      - removes fields from original request         //
//       rt_add_fields         - adds fields to original request              //
//       rt_remove_all_symbols - eliminates all symbol minding                //
//       rt_syserr             - platform-specific low-level error code       //
//                                                                            //
//  3) Data retrieval                                                         //
//     Within the callback routines, data fields are accessed via the         //
//     following:                                                             //
//       rt_get_field_int  - returns field as an integer                      //
//       rt_get_field_real - returns field as a floating point number         //
//       rt_get_field_char - returns field as a character string              //
//       rt_get_field_definition - dummy, dont use!!                          //
//                                                                            //
//----------------------------------------------------------------------------//
//                                                                            //
//                          IMPORTANT C++ NOTES                               //
//                                                                            //
// 1) This include file and the whole interface is C and C++ compatable.      //
//                                                                            //
// 2) For technical C++ reasons, a callback routine cannot be a class object  //
//    member function. If it needs to be, code the callback routine as a      //
//    non-class routine, and invoke the member function from within this      //
//    non-class routine.                                                      //
//                                                                            //
//----------------------------------------------------------------------------//
//                                                                            //
//                               EXAMPLE OF USE                               //
//                                                                            //
//     -----------------  THIS WAS WRONG DELETED .  ---------------           //
//----------------------------------------------------------------------------//
//                                                                            //
//                           SUMMARY OF CHANGES                               //
//                                                                            //
// Date     By  Change                                                        //
// -------- --- ------------------------------------------------------------- //
// 12/28/94  MER Miscellaneous changes to specification:                      //
//                Merge RT_INSTRUMENT into RT_SYMBOL_TYPE.                    //
//                Extract option-specific fields in RT_SYMBOL into a union.   //
//                Eliminate RT_LAST_FIELD and replace with a field_count.     //
//                Add max_length parameter to rt_get_field_char.              //
//                Add definition for new rt_get_field_definition call.        //
//                Improve documentation.                                      //
//                                                                            //
//  1/11/95  MER Changed rt_startup from a synchronous to an asynchronous     //
//              call, with a callback.                                        //
//                                                                            //
//  1/23/95  MER Changed rt_get_field_int to rt_get_field_int32.              //
//              Added rt_get_field_type function.                             //
//              Changed RT_FIELD_TYPE TO RT_FIELD and created a new           //
//              RT_FIELD_TYPE to indicate the "type" of a field.              // 
//                                                                            //
//  2/15/95  MER Added preprocessing code to make this accessable from both   //
//              C and C++.                                                    //
//                                                                            //
//  2/28/95  MER Changed "RT_REQ_ID * req_id" to "RT_REQ_ID req_id" in        //
//              rt_remove_fields and rt_add_fields.                           //
//                                                                            //
//  5 95     KYR FUBAR                                                        //
//                                                                            //
//  10/05/95 KYR all new dynamic guts no security amount limit                //
//                                                                            //
//----------------------------------------------------------------------------//


#ifndef _RT_API_H
#define _RT_API_H
#include "rt_share.h"
// If C++, define the function declarations as C. //
#ifdef __cplusplus
extern "C" {
#endif

  //-------------------------------------------------------------------//
  //                                                                   //
  //                   Definition of RT_FIELD_TYPE                     //
  //                                                                   //
  //  The following are used in the rt_get_field_type call to specify  //
  //                   the "type" of a data field.                     //
  //                                                                   //
  //-------------------------------------------------------------------//

typedef enum  RT_FIELD_TYPE
{  
   RT_CHAR = 1                          , // Null-terminated character string          
   RT_INT32                             , // 32-bit signed integer                     
   RT_UINT32                            , // 32-bit unsigned integer                   
   RT_REAL                              , // Double-precision floating point           
   RT_TIME                              , // Time-of-day (hh:mm), in 24-hour time.     
                                          //  Format will be adjusted to local         
                                          //  conventions.                             
   RT_DATE                                // Date (mm/dd/yy). Format will be adjusted  
                                          //  to local conventions.                    
}                         RT_FIELD_TYPE ;  



typedef  void *           RT_CONNECT_ID ; // Identifies virtual client-RT connection          
typedef  void *           RT_IDENTIFIER ; // DEPRECATED DON'T USE!!!!    
 

typedef  void *               RT_REQ_ID ; // Identifies a particular RT request      

#define  RT_RAW_FIELD_BUF  char           // replaces the origonal data_buffer
                                          // which is no longer exposed



//----------------------------------------------------------------------------
//                                                                            
//                      IDENTIFYING THE FINANCIAL INSTRUMENT                  
//                                                                            
//----------------------------------------------------------------------------

typedef struct 
{    
  char                       series[2] ; // option series month                        
  char                         year[4] ; // series year  0000-9999                     
  char                 strike_price[6] ; // strike price                               
  char                            type ; // 'C' = call, 'P' = put, etc.                
}                        option_fields ;

typedef struct 
{   
  int                            dummy ;
}                         dummy_others ; // see union below   


typedef struct
{  
  char                        name[10] ; // Root of instrument, NULL terminated.  If  
                                         //  the field is null (i.e. name[0] = NULL)  
                                         //  that indicates a request for all symbols 
                                         //  which satisfy the qualifications repre-  
                                         //  sented by the following fields.          
  char                     exchange[2] ; // exchange code                             
   
     
  union                                  // union defines the type-specific fields 
  {
    option_fields               options ;
    dummy_others                  dummy ;
  }                                   u ;

}                             RT_SYMBOL ;


typedef unsigned int          RT_HANDLE ; // This field type is returned when an    
                                          //  RT_SYMBOL_HANDLE or RT_PARENT_HANDLE  
                                          //  field is requested and is used in an  
                                          //  rt_request_fields_via_handle call.    



typedef enum  RT_SYMBOL_TYPE              //      Define symbol types             
{                                         // These types cannot presently be ORed 
                                          // together.                            
   RT_SYMBOL_TYPE_STOCK          = 0x01 ,
   RT_SYMBOL_TYPE_OPTION         = 0x02 ,
   RT_SYMBOL_TYPE_FUTURES        = 0x04 ,     
   RT_SYMBOL_TYPE_MKTSTAT        = 0x08 ,
   RT_SYMBOL_TYPE_BOND           = 0x10 ,
   RT_SYMBOL_TYPE_FUTURE         = 0x20 ,
   RT_SYMBOL_TYPE_RATES          = 0x40 ,
}                        RT_SYMBOL_TYPE ;


  //-------------------------------------------------------------------
  //                                                                   
  //                     Definition of RT_FIELD                        
  //                                                                   
  //  The following are all the possible fields which can be requested 
  //                          on a symbol.                             
  //                                                                   
  //-------------------------------------------------------------------
// TKR_TYPE_BITS defined in rt_share.h


typedef enum  RT_FIELD     
  {
   RT_SYMBOL_NAME = 1                   , // instrument's symbolic name                
   RT_DESCRIPTION                       , // Up to 128-char. description of symbol     
                                          //  e.g. International Business Machines     
   RT_PRIMARY_MKT_FLAG                  , // set if this is the primary market         
   RT_INSTRUMENT_TYPE                   , // type of instrument                        
   RT_MARKET_CODE                       , // exchange code                             
   RT_SYMBOL_HANDLE                     , // Internal "handle" for symbol. It can be   
                                          //  used in an rt_request_fields_via_handle  
                                          //  call to get additional data.             
   RT_PARENT_HANDLE                     , // Internal "handle" for "parent" of symbol. 
                                          //  Usable in an rt_request_fields_via_handle
                                          //  call to get parent field data.           
   RT_LAST_VOLUME                       , // volume of last trade                      
   RT_LAST_PRICE                        , // price at last trade                       
   RT_LAST_TIME                         , // Time of last trade midnight sec           
   RT_PREV_TRADE                        , // previous trade price (may be same)        
   RT_PREV_DIFF_PRICE                   , // previous trade with different price       
   RT_HIGH                              , // high for day                              
   RT_LOW                               , // low for day                               
   RT_OPEN                              , // day's open                                
   RT_Y_CLOSE                           , // yesterday's closing price                 
   RT_Y_COMPOSITE_CLOSE                 , // yesterday's composite close               
   RT_BID                               , // real-time bid                             
   RT_Y_BID                             , // yesterday's closing bid                   
   RT_ASK                               , // real-time ask                             
   RT_Y_ASK                             , // yesterday's closing ask                   
   RT_PRICE_RENDER_FMT                  , // display format for price fields           
   RT_ASK_SIZE                          , // real-time ask in 100's                    
   RT_BID_SIZE                          , // real-time bid in 100's                    
   RT_TOT_VOLUME                        , // real-time volume                          
   RT_Y_VOLUME                          , // yesterday's closing  volume               
   RT_OPTION_CODE                       , // option series designation                 
   RT_RESTRICTED                        , // restricted list indicator                 
   RT_LAST_NEWS_TIME                    , // hours/minutes                             
   RT_1ST_DIVID_DATE                    , // 1st dividend date, yymmdd                 
   RT_1ST_DIVID                         , // 1st dividend amount                       
   RT_2ND_DIVID_DATE                    , // 2st dividend date, yymmdd                 
   RT_2ND_DIVID                         , // 2st dividend amount                       
   RT_PROJECTED_DIV                     , // 0=actual, 1=projected                     
   RT_DIV_PER_YEAR                      , // # divs per year                           
   RT_EPS                               , // earnings per share                        
   RT_ANN_HIGH                          , // annual high price                         
   RT_ANN_LOW                           , // annual low                                
   RT_VARIANCE                          , // price variance                            
   RT_TRADING_UNITS                     , // option trading units                      
   RT_OPTION_DIV                        , // # of dividend payments                    
   RT_OPTION_DAY                        , // # of days to expiration                   
   RT_STRIKE_PRICE                      , // strike price                              
   RT_OPEN_INTEREST                     , // open interest                             
   RT_OPTION_SERIES                     , // series #                                  
   RT_TRADE_FLAG                        , // trade : 0=last 1=bid 2=ask 3=sett         
   RT_FUTURE_OPEN_RANGE_1               , // opening range 1                           
   RT_FUTURE_OPEN_RANGE_2               , // opening range 2                           
   RT_FUTURE_TYPE                       , // future type                               
   RT_FIRST_DELIVERY_DATE               , // first delivery date, yymmdd               
                                          // to extract use sprintf(buf,"%02d\/%02d\/%02",          
                                          //    d/10000 , (d%10000)/100 , d%100 );       
   RT_LAST_DELIVERY_DATE                , // last  delivery date, yymmdd               
   RT_LAST_TRADE_DATE                   , // last trade date, yymmdd                   
   RT_VENDOR1_NAME                      , // vendor 1 sec name                         
   RT_VENDOR2_NAME                      , // vendor 2 sec name                         
   RT_STRIKE_DEV_CODE                   , // strike derivation code                    
   RT_FUT_MON_LTR_CODE                  , // future month letter code                  
   RT_VENDOR_STRIKE_PRICE               , // vendor_strike_price;                      
   RT_BOND_RATE                         , // interest rate in 1000's                   
   RT_BOND_INT_DATE                     , // next int. payment date                    
   RT_BOND_FREQ                         , // int. pay frequency:                       
   RT_ISSUE_DATE                        , // issue date, mmddyy                        
   RT_COUPON1_DATE                      , // 1st coupon date, mmddyy                   
   RT_CALL_DATE                         , // call date, mmddyy                         
   RT_AUCTION_SETT_DATE                 , // auction settlement date, mmddyy           
   RT_MATURITY_DATE                     , // maturity_date, mmddyy                     
   RT_COUPON_RATE                       , // coupon_rate in percentage                 
   RT_CUSIP                             , // cusip number                              
   RT_SYS_ALERT                         , // E.g. CBOE feed down                       
   RT_TKR_TYPE_BITS                     , // see ticker line trade types in rt_share.h 
   RT_DAYS_TO_EXPIRATION                , //                                           
   RT_TRADE_SEQUENCE_NUM                , //                                           
   RT_SERIES_YEAR                       , // opt/bnd series year                       
   RT_CALL_PUT_FLAG                     , // C/P                                       
   RT_MODEL                             , // Model to use                              
   RT_QUOTE_CONDITION                   , // quote condition                           
   RT_MARKET_CODE_NUMBER                , // exchange code number                      
   RT_BIT_FLAGS                         , // bit flags in pricefile record             
   RT_PRIMARY_OPTION_MKT_CODE           , // primary option market code                
   RT_BLOCK_SIZE                        , // trading block size ofr instrument
   RT_RESTRICTED_LIST                   , // restricted list level bits 
                                          // !!PRIVATE TO BROKERAGE HOUSE API'S
                                          // and only aviliable where entitled
   RT_NEXT_DB                           , // next valid db number in pricefile
   RT_TICK_INDICATOR                    , // 1 if tick is up -1 if tick is down ?? can tick be unchanged??
   RT_SIGNAL_TYPE                       , // defined in pstypes.h from track's internal use
   RT_H2                                , // defined in pstypes.h from track's internal use
   RT_TRADE_BIT_FLAGS                   , // defined in pstypes.h from track's internal use

   RT_TRADE_CONDITION_CHAR              , // defined in pstypes.h from track's internal use

   RT_TICK_TIME                         , // time of tick (resynch, quote, trade, correction etc...

   RT_NUMBER_OF_FIELDS                    // This is NOT a field specifier. It is only a 
                                          //   count of the number of defined fields.      
                                          //   Thus will always be the last item defined in  
                                          //   this "enum" and its value will change as 
                                          //   items are added.   
}                              RT_FIELD ;

#define RT_LAST_FIELD RT_NUMBER_OF_FIELDS




#define RT_FIELD_ID_STRS_LEN_MAX 30
#define RT_FIELD_ID_STRS_INITIALIZER \
{                                \
  { ""                        } ,  \
  { "SYMBOL_NAME"             } ,  \
  { "DESCRIPTION"             } ,  \
  { "PRIMARY_MKT_FLAG"        } ,  \
  { "INSTRUMENT_TYPE"         } ,  \
  { "MARKET_CODE"             } ,  \
  { "SYMBOL_HANDLE"           } ,  \
  { "PARENT_HANDLE"           } ,  \
  { "LAST_VOLUME"             } ,  \
  { "LAST_PRICE"              } ,  \
  { "LAST_TIME"               } ,  \
  { "PREV_TRADE"              } ,  \
  { "PREV_DIFF_PRICE"         } ,  \
  { "HIGH"                    } ,  \
  { "LOW"                     } ,  \
  { "OPEN"                    } ,  \
  { "Y_CLOSE"                 } ,  \
  { "Y_COMPOSITE_CLOSE"       } ,  \
  { "BID"                     } ,  \
  { "Y_BID"                   } ,  \
  { "ASK"                     } ,  \
  { "Y_ASK"                   } ,  \
  { "PRICE_RENDER_FMT"        } ,  \
  { "ASK_SIZE"                } ,  \
  { "BID_SIZE"                } ,  \
  { "TOT_VOLUME"              } ,  \
  { "Y_VOLUME"                } ,  \
  { "OPTION_CODE"             } ,  \
  { "RESTRICTED"              } ,  \
  { "LAST_NEWS_TIME"          } ,  \
  { "1ST_DIVID_DATE"          } ,  \
  { "1ST_DIVID"               } ,  \
  { "2ND_DIVID_DATE"          } ,  \
  { "2ND_DIVID"               } ,  \
  { "PROJECTED_DIV"           } ,  \
  { "DIV_PER_YEAR"            } ,  \
  { "EPS"                     } ,  \
  { "ANN_HIGH"                } ,  \
  { "ANN_LOW"                 } ,  \
  { "VARIANCE"                } ,  \
  { "TRADING_UNITS"           } ,  \
  { "OPTION_DIV"              } ,  \
  { "OPTION_DAY"              } ,  \
  { "STRIKE_PRICE"            } ,  \
  { "OPEN_INTEREST"           } ,  \
  { "OPTION_SERIES"           } ,  \
  { "TRADE_FLAG"              } ,  \
  { "FUTURE_OPEN_RANGE_1"     } ,  \
  { "FUTURE_OPEN_RANGE_2"     } ,  \
  { "FUTURE_TYPE"             } ,  \
  { "FIRST_DELIVERY_DATE"     } ,  \
  { "LAST_DELIVERY_DATE"      } ,  \
  { "LAST_TRADE_DATE"         } ,  \
  { "VENDOR1_NAME"            } ,  \
  { "VENDOR2_NAME"            } ,  \
  { "STRIKE_DEV_CODE"         } ,  \
  { "FUT_MON_LTR_CODE"        } ,  \
  { "VENDOR_STRIKE_PRICE"     } ,  \
  { "BOND_RATE"               } ,  \
  { "BOND_INT_DATE"           } ,  \
  { "BOND_FREQ"               } ,  \
  { "ISSUE_DATE"              } ,  \
  { "COUPON1_DATE"            } ,  \
  { "CALL_DATE"               } ,  \
  { "AUCTION_SETT_DATE"       } ,  \
  { "MATURITY_DATE"           } ,  \
  { "COUPON_RATE"             } ,  \
  { "CUSIP"                   } ,  \
  { "SYS_ALERT"               } ,  \
  { "TKR_TYPE_BITS"           } ,  \
  { "DAYS_TO_EXPIRATION"      } ,  \
  { "TRADE_SEQUENCE_NUM"      } ,  \
  { "SERIES_YEAR"             } ,  \
  { "CALL_PUT_FLAG"           } ,  \
  { "MODEL"                   } ,  \
  { "QUOTE_CONDITION"         } ,  \
  { "MARKET_CODE_NUMBER"      } ,  \
  { "BIT_FLAGS"               } ,  \
  { "PRIMARY_OPTION_MKT_CODE" } ,  \
  { "BLOCK_SIZE"              } ,  \
  { "RESTIRCTED_LIST"         } ,  \
  { "NEXT_DB"                 } ,  \
  { "NUMBER_OF_FIELDS"        }    \
} ;                                       
// USAGE FOR ABOVE INITIALIZER  
// static char rt_field_names[][RT_FIELD_ID_STRS_LEN_MAX] = RT_FIELD_ID_STRS_INITIALIZER ;
// qprintf("id = %d<%s> \n",id,rt_get_field_id_string(id));
                                       
#ifdef _DEBUG
static _inline char * rt_get_field_id_string(RT_FIELD id)
{
static char rt_field_names[][RT_FIELD_ID_STRS_LEN_MAX] = RT_FIELD_ID_STRS_INITIALIZER ;
return rt_field_names[id] ;
}
#endif                                




//----------------------------------------------------------------------------
//                                                                            
//                          INITIALIZATION/TERMINATION                        
//                                                                            
//----------------------------------------------------------------------------
       


//---------------------------------------------------------------------------- 
typedef void (*RT_STARTUP_CLBK)           // gets called when the startup process 
                                          // returns, good or not
(
  RT_CONNECT_ID                   rt_id , // Identifies RT connection               
  RT_ERR                       err_code   // Error code:                         
                                          //        RT_ERR_OK                    
                                          //        RT_ERR_CONNECTION_NOT_OPENED 
                                          //        RT_ERR_NOT_ENTITLED          
)                                       ;


//---------------------------------------------------------------------------- 
typedef void (*RT_SHUTDOWN_CLBK)          // get called if user sets it up  
                                          // otherwise api does and exit (0)
(
  RT_CONNECT_ID                   rt_id , // Identifies RT connection      
  RT_ERR                       err_code   // Error code:                         
                                          //        RT_ERR_OK                    
                                          //        RT_ERR_CONNECTION_INVALID    
)                                       ;

//----------------------------------------------------------------------------
RT_ERR rt_startup                        // Open connection with Real Time server  
                                          // This call is synchronous !!
(
  char *                       username , // User name                                    
  char *                        user_id , // User id                                      
  RT_CONNECT_ID *                 rt_id , // Returned connection pointer to 
                                          //   be used with all calls                              
  RT_STARTUP_CLBK          connect_clbk   // Address of routine call by api when
                                          //   connection completes with err code
                                          //   if it completes in a failure               
)                                       ;


//----------------------------------------------------------------------------
RT_ERR  rt_set_shutdown_callback          // set the address of the user routine
                                          // called when a connection shutsdown
                                          // if not set then the api will do
                                          // and exit(0)
                                          // returns:                        
                                          //        RT_ERR_OK                 
                                          //        RT_ERR_CONNECTION_INVALID 
(
  RT_CONNECT_ID                   rt_id ,   
  RT_SHUTDOWN_CLBK        shutdown_clbk     
)                                       ;


//---------------------------------------------------------------------------- 
RT_ERR  rt_shutdown              // Shut down connection to the RT server     
                                 //  returns:                                 
                                 //        RT_ERR_OK                          
                                 //        RT_ERR_CONNECTION_INVALID          
                                 //        RT_ERR_CONNECTION_NOT_OPEN         
(
   RT_CONNECT_ID   rt_id         // Identifies connection to shutdown            
)                              ;


//---------------------------------------------------------------------------- 
//                                                                            
//                     SYMBOL/FIELD SPECIFICATION CALLS                       
//                                                                            
//----------------------------------------------------------------------------


//typedef enum  RT_REQ_TYPE   in rt_share.h



//----------------------------------------------------------------------------
//                                                                            
//                             RT_GET_FIELDS_CLBK                             
//                                                                            
//              The following defines the rt_request_fields and               
//        rt_request_fields_via_handle (see below for their definition)       
//                         callback routine interface.                        
//                                                                            
//  This callback routine gets called (possibly repeatedly, depending on      
//  RT_REQ_TYPE) with the data requested by the associated rt_request_fields. 
//  The first three fields are duplicates of the fields in the associated     
//  rt_request_fields call and are there to identify the originating call and 
//  the type of data being returned.                                          
//                                                                            
//  The fifth field, fields, identifies which of the requested fields were    
//  actually returned. The returned data is in data_buf.  This data may never 
//  be accessed directly, only through the rt_get_field_... calls. These      
//  calls may only be made within the callback routine (i.e., up until it     
//  returns), since the data_buf area will be reused once the callback        
//  routine gives up control.  The data_length field is provided in case the  
//  user wishes to move the data_buf area (in which case the rt_get_field_... 
//  calls may be issued even after the callback routine ends, by refering to  
//  the moved data_buf area.                                                  
//                                                                            
//----------------------------------------------------------------------------

typedef void (*RT_GET_FIELDS_CLBK)    // Callback to get data from RT server  
  (
   RT_CONNECT_ID              rt_id , // Identifies RT connection              
   RT_REQ_ID                 req_id , // Identifies original request          
                                      //  generating this callback            
   void *                 user_data , // User data pointer from original user 
                                      //  request                             
   RT_REQ_TYPE             req_type , // Type of response                     
   int                  field_count , // Number of fields in "fields" below.  
   RT_FIELD *                fields , // Pointer to list of values indicating 
                                      //  the fields returned.                
   int                  data_length , // Length, in bytes, of data_buf.       
   RT_RAW_FIELD_BUF *      data_buf , // Pointer to buffer holding returned   
                                      //  data.  This data may never be       
                                      //  accessed directly, only through the 
                                      //  rt_get_field_... calls.  Using the  
                                      //  data_length field above, this buffer
                                      //  may be moved by the user wherever   
                                      //  desired, since it is self-          
                                      //  describing and does not depend on   
                                      //  any other external data.            
   RT_ERR                    err_code // Error code:                          
                                      //        RT_ERR_OK                     
                                      //        RT_ERR_SYMBOL_NOT_FOUND       
                                      //        RT_ERR_NOT_ENTITLED           
  );



//----------------------------------------------------------------------------
//                                                                            
//                              rt_request_fields                             
//                                                                            
//                    Request data fields from RT server                      
//                                                                            
//  This call is the central focus of all the RT calls.  It passes to the RT  
//  server the specific field(s) within a symbol for which it wants real-time 
//  information. It also indicates whether the request is a 1-time request or 
//  whether continuous updates are desired (symbol "minding"). The requested  
//  data is returned in the callback routine.                                 
//                                                                            
//----------------------------------------------------------------------------

RT_ERR  rt_request_fields                 // Requests data for symbol                
                                          //  returns:                               
                                          //        RT_ERR_OK                        
                                          //        RT_ERR_CONNECTION_INVALID        
                                          //        RT_ERR_CONNECTION_NOT_OPEN       
(
  RT_CONNECT_ID                   rt_id , // RT connect id
  RT_REQ_ID *        req_id_ptr_to_fill , // Returned address of a req_entry to        
                                          //   uniquely identify this request.        
  void  *                     user_data , // Pointer to any user data. This pointer, 
                                          //   or the data that it points to  can be  
                                          //   used in the callback routine to help   
                                          //   in processing the data or for any      
                                          //   other purpose. This pointer value      
                                          //   is passed unchanged to the callback    
                                          //   routine.                               
  RT_REQ_TYPE                  req_type , // Type of request (just current data,     
                                          //   every tick on symbol, etc.)            
  RT_SYMBOL *                    symbol , // Symbol of instrument for which data is  
                                          //   requested.                             
  int                       field_count , // Number of fields in "fields" below.     
  RT_FIELD *                     fields , // Pointer to list of values which         
                                          //   represent the fields which should be   
                                          //   returned for the specified symbol.     
  RT_GET_FIELDS_CLBK           callback   // Address of callback routine which gets  
                                          //   called (possibly more than once,       
                                          //   depending on RT_REQ_TYPE) with the     
                                          //   requested data. See below.             
                                          // NOTE: can be null if set generic has been called
)                                       ;



//----------------------------------------------------------------------------
//                                                                            
//                        rt_request_fields_via_handle                        
//                                                                            
//                  sends request for fields to RT server                     
//                                                                            
//  This call is identical to rt_request_fields call above except that a      
//  symbol handle is used instead of a symbol. Data is returned via the same  
//  callback mechanism.                                                       
//                                                                            
//----------------------------------------------------------------------------

RT_ERR  rt_request_fields_via_handle      // Requests data for symbol    
                                          //  returns:                               
                                          //        RT_ERR_OK                        
                                          //        RT_ERR_CONNECTION_INVALID        
                                          //        RT_ERR_CONNECTION_NOT_OPEN       
(
  RT_CONNECT_ID                   rt_id , // RT connect id
  RT_REQ_ID *        req_id_ptr_to_fill , // Returned address of a req_entry to        
                                          //   uniquely identify this request.        
  void  *                     user_data , // Pointer to any user data. This pointer, 
                                          //   or the data that it points to  can be  
                                          //   used in the callback routine to help   
                                          //   in processing the data or for any      
                                          //   other purpose. This pointer value      
                                          //   is passed unchanged to the callback    
                                          //   routine.                               
  RT_REQ_TYPE                  req_type , // Type of request (just current data,     
                                          //   every tick on symbol, etc.)            
  RT_HANDLE                      handle , // Handle for symbol of instrument for     
                                          //   which data is requested.               
  int                       field_count , // Number of fields in "fields" below.     
  RT_FIELD *                     fields , // Pointer to list of values which         
                                          //   represent the fields which should be   
                                          //   returned for the specified symbol.     
  RT_GET_FIELDS_CLBK           callback   // Address of callback routine which gets  
                                          //   called (possibly more than once,       
                                          //   depending on RT_REQ_TYPE) with the     
                                          //   requested data. See below.             
                                          // NOTE: can be null if set generic has been called
)                                       ;






//----------------------------------------------------------------------------
//                                                                            
//                            RT_GET_GROUP_CLBK                               
//                                                                            
//  The following defines the rt_request_group callback routine interface.    
//  This callback routine gets called after a symbol or field get of a root   
//  security.                                                                 
//  the list list are only available within the callback routine (i.e.until it
//  returns), since the list area will be reused once the callback            
//  routine gives up control.                                                 
//                                                                            
//----------------------------------------------------------------------------

typedef void (*RT_GET_GROUP_CLBK) // Callback to get symbols             
  (
   RT_CONNECT_ID    rt_id         , // Identifies RT connection                  
   RT_REQ_ID        req_id        , // Identifies original request generating 
                                    //  this callback                         
   void           * user_data     , // User data from original user request   
   RT_GRP_REQ_TY     group_type   , // Type of group for which information is 
                                    //  is being returned from rt_share.h     
   RT_HANDLE        underlier     , // Root symbol (i.e. ticker, future       
                                    //  family)                               
   int              tot_num       , // Number of handles there are            
   int              cur_offset    , // offset of current chunk in list        
   int              num_received  , // Number of handles in this message      
                                    // if cur + num == tot we are at the end  
   RT_HANDLE    *   handle_list   , // List of symbol handles.                
   RT_ERR           err_code        // Error code:                            
                                    //        RT_ERR_OK                       
                                    //        RT_ERR_SYMBOL_NOT_FOUND         
                                    //        RT_ERR_NOT_ENTITLED             
  );



//---------------------------------------------------------------------------- 
RT_ERR  rt_request_group                  // Request for component group (pt,bnds,etc
                                          //  returns:                               
                                          //        RT_ERR_OK                        
                                          //        RT_ERR_CONNECTION_INVALID        
                                          //        RT_ERR_CONNECTION_NOT_OPEN       
(
  RT_CONNECT_ID                   rt_id , // RT connect id
  RT_REQ_ID *        req_id_ptr_to_fill , // Returned address of a req_entry to        
                                          //   uniquely identify this request.        
  void  *                     user_data , // Pointer to any user data. This pointer, 
                                          //   or the data that it points to  can be  
                                          //   used in the callback routine to help   
                                          //   in processing the data or for any      
                                          //   other purpose. This pointer value      
                                          //   is passed unchanged to the callback    
                                          //   routine.                               
  RT_GRP_REQ_TY              group_type , // Type of GROUP for which information is 
                                          //  wanted                                 
  RT_HANDLE                      handle , // Root symbol (i.e. ticker, future family)    
                                          //   which data is requested.               
  RT_GET_GROUP_CLBK            callback   // Address of callback routine which gets  
                                          //   called (possibly more than once,       
                                          //   depending on RT_REQ_TYPE) with the     
                                          //   requested data. See below.             
)                                       ;




//----------------------------------------------------------------------------
//                                                                            
//                              rt_request_header callback                            
//                                                                            
//                    Request header from RT server                      
//                                                                            
//                                                                            
//----------------------------------------------------------------------------

typedef void (*RT_GET_HEADER_CLBK)    // Callback to get header data from RT server  
  (
   RT_CONNECT_ID              rt_id , // Identifies RT connection              
   RT_REQ_ID                 req_id , // Identifies original request          
                                      //  generating this callback            
   void *                 user_data , // User data pointer from original user 
                                      //  request                             
   int                  data_length , // Length, in bytes, of data_buf.       
   void *                  data_buf , // Pointer to buffer holding returned   
                                      //  data. 
   RT_ERR                  err_code   // Error code:                          
                                      //        RT_ERR_OK                     
  );



//----------------------------------------------------------------------------
//                                                                            
//                              rt_request_header                            
//                                                                            
//                    Request header from RT server                      
//                                                                            
//                                                                            
//----------------------------------------------------------------------------

RT_ERR  rt_request_header                 // Requests header                
                                          //  returns:                               
                                          //        RT_ERR_OK                        
                                          //        RT_ERR_CONNECTION_INVALID        
                                          //        RT_ERR_CONNECTION_NOT_OPEN       
(
  RT_CONNECT_ID                   rt_id , // RT connect id
  RT_REQ_ID *        req_id_ptr_to_fill , // Returned address of a req_entry to        
                                          //   uniquely identify this request.        
  void  *                     user_data , // Pointer to any user data. This pointer, 
                                          //   or the data that it points to  can be  
                                          //   used in the callback routine to help   
                                          //   in processing the data or for any      
                                          //   other purpose. This pointer value      
                                          //   is passed unchanged to the callback    
                                          //   routine.                               
  RT_GET_HEADER_CLBK           callback   // Address of callback routine which gets  
                                          //   called
)                                       ;



//---------------------------------------------------------------------------- 
RT_ERR  rt_remove_fields                  // Turn off field minding                    
                                          //  returns:                                 
                                          //        RT_ERR_OK                          
                                          //        RT_ERR_CONNECTION_INVALID          
                                          //        RT_ERR_CONNECTION_NOT_OPEN         
                                          //        RT_ERR_INVALID_REQ_ID              
                                          //        RT_ERR_INVALID_FIELD               
(                        
  RT_CONNECT_ID                   rt_id , // Identifies RT connection                     
  RT_REQ_ID                      req_id , // Identifies original request whose fields  
                                          //  are being minded.                        
  int                       field_count , // Number of fields in "fields" below. If    
                                          //  "fields" is 0 (see below for what this   
                                          //  means), this field is ignored.            
  RT_FIELD *                     fields   // Pointer to list of values which represent 
                                          //  the fields to be ignored.  If 0, all     
                                          //  minded fields in symbol specified by     
                                          //  req_id are removed, which results in the 
                                          //  entire symbol being forgotten (no longer 
                                          //  "minded") and invalidating the req_id.   
)                                     
                                        ;



//---------------------------------------------------------------------------- 
RT_ERR  rt_add_fields                     // Add field(s) to be minding                
                                          //  returns:                                 
                                          //        RT_ERR_OK                          
                                          //        RT_ERR_CONNECTION_INVALID          
                                          //        RT_ERR_CONNECTION_NOT_OPEN         
                                          //        RT_ERR_INVALID_REQ_ID              
                                          //        RT_ERR_INVALID_FIELD               
(                                       
  RT_CONNECT_ID                   rt_id , // Identifies RT connection                     
  RT_REQ_ID                      req_id , // Identifies original request whose fields  
                                          //  are being minded.                        
  int                       field_count , // Number of fields in "fields" below. If    
                                          // 0 then we remove the request completely
  RT_FIELD *                     fields   // Pointer to list of values which represent 
                                          //  the fields to be ignored.  
)                                       ;



//---------------------------------------------------------------------------- 
RT_ERR  rt_remove_all_symbols             // Turn off all symbol minders               
                                          //  returns:                                 
                                          //        RT_ERR_OK                          
                                          //        RT_ERR_CONNECTION_INVALID          
                                          //        RT_ERR_CONNECTION_NOT_OPEN         
(
  RT_CONNECT_ID                   rt_id           
)                                       ;



//---------------------------------------------------------------------------
int   rt_syserr                           // Get system error code possibly 
                                          // paltform-specific, This routine                   
                                          // must be called immediately after 
                                          // the call which failed
(    
  RT_CONNECT_ID                   rt_id   // Identifies RT connection                 
)                                       ;   



//----------------------------------------------------------------------------
//                                                                            
//                              rt_request_mind_by_exchange
//                                                                            
//                    Request mind by exchange                    
//                                                                            
//                                                                            
//----------------------------------------------------------------------------
RT_ERR  rt_mind_by_exchange               // Requests data by exchanges
                                          //  returns:                               
                                          //        RT_ERR_OK                        
                                          //        RT_ERR_CONNECTION_INVALID        
                                          //        RT_ERR_CONNECTION_NOT_OPEN       
(
  RT_CONNECT_ID                   rt_id , // RT connect id
  RT_REQ_ID *        req_id_ptr_to_fill , // Returned address of a req_entry to        
                                          //   uniquely identify this request.        
  void  *                     user_data , // Pointer to any user data. This pointer, 
                                          //   or the data that it points to  can be  
                                          //   used in the callback routine to help   
                                          //   in processing the data or for any      
                                          //   other purpose. This pointer value      
                                          //   is passed unchanged to the callback    
                                          //   routine.                               
  int                       field_count , // Number of fields in "fields" below.     
  RT_FIELD *                     fields , // Pointer to list of values which         
                                          //   represent the fields which should be   
                                          //   returned for the specified symbol.     
  RT_GET_FIELDS_CLBK           callback , // Address of callback routine which gets  
                                          //   called (possibly more than once,       
                                          //   depending on RT_REQ_TYPE) with the     
                                          //   requested data. See below. 
                                           
  unsigned char exchg_map[RT_MAX_EXCHANGES] 

)                                       ;



//----------------------------------------------------------------------------
//                                                                            
//                              rt_request_modify_mind_by_exchange
//                                                                            
// NOTE: to modify the fields on a minde by exchange just use the 
//       add and remove fields call on the req id
//     : also a remove fields call with a count of 0 will result in a stop mind                                                                            
//----------------------------------------------------------------------------

RT_ERR  rt_modify_mind_by_exchange             
                                          //  returns:                               
                                          //        RT_ERR_OK                        
                                          //        RT_ERR_CONNECTION_INVALID        
                                          //        RT_ERR_CONNECTION_NOT_OPEN       
(
  RT_CONNECT_ID                   rt_id , // RT connect id
  RT_REQ_ID                      req_id , //req id of live mind
  void  *                     user_data , // Pointer to any user data. This pointer, 
                                          //   or the data that it points to  can be  
                                          //   used in the callback routine to help   
                                          //   in processing the data or for any      
                                          //   other purpose. This pointer value      
                                          //   is passed unchanged to the callback    
                                          //   routine.                        
                                          
  int                       modify_flag , // TRUE= modify map, FALSE= to stop the mind 

  unsigned char exchg_map[RT_MAX_EXCHANGES] // replacement map  

)                                       ;




//--------------------------------------------------------------------------
//                                                                          
//                               DATA RETRIEVAL                             
//                                                                          
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
RT_ERR  rt_get_field_int32                // Returns field value as an integer                   
                                          // returns:                              
                                          //        RT_ERR_OK                      
                                          //        RT_ERR_INVALID_FIELD           
(
  RT_RAW_FIELD_BUF *           data_buf , // Pointer to raw_data_buffer from    
                                          //   RT_GET_FIELDS_CLBK callback routine.   
  RT_FIELD                        field , // Field id whose "int32" data is needed              
  /*unsigned*/ int *         int_out_ptr   // address of output "int32" value
)                                       ;
  
//--------------------------------------------------------------------------
RT_ERR  rt_get_field_real                 // Returns field value as a double
                                          // precision floating point number     
                                          // returns:                              
                                          //        RT_ERR_OK                      
                                          //        RT_ERR_INVALID_FIELD           
(
  RT_RAW_FIELD_BUF *           data_buf , // Pointer to raw_data_buffer from    
                                          //   RT_GET_FIELDS_CLBK callback routine.   
  RT_FIELD                        field , // Field id whose "real" data is needed              
  double *                 real_out_ptr   // address of caller's output "real" value       
)                                       ;

//--------------------------------------------------------------------------
RT_ERR  rt_get_field_char                 // Returns field value as a NULL 
                                          // terminated character string       
                                          // returns:                              
                                          //        RT_ERR_OK                      
                                          //        RT_ERR_INVALID_FIELD           
(                                         
  RT_RAW_FIELD_BUF *           data_buf , // Pointer to raw_data_buffer from    
                                          //   RT_GET_FIELDS_CLBK callback routine.   
  RT_FIELD                        field , // Field id whose "char" data is needed              
  int                        max_length , // Maximum string length which can fit in  
                                          //   out_string var        
  char *                 out_string_ptr , // address of caller's output string
  int *                  out_length_ptr   // address of caller's output strlen    
)                                       ;

//--------------------------------------------------------------------------
RT_ERR  rt_get_field_type                 // Returns the type of the field value.  
                                          // Based on this type, call the correct
                                          // rt_get_field_(int/real/char...)
                                          // returns:                              
                                          //        RT_ERR_OK                      
                                          //        RT_ERR_INVALID_FIELD           
(
  RT_RAW_FIELD_BUF *           data_buf , // Pointer to data buffer returned in      
                                          //  RT_GET_FIELDS_CLBK callback routine.   
  RT_FIELD                        field , // Field whose type is needed              
  RT_FIELD_TYPE *                p_type   // Pointer to a field in which the field   
                                          //  type will be returned.                 
)                                       ;




//------ NOT IMPLEMENTED !!!!!!---------------------------------------------------- 
typedef struct RT_FIELD_DEFINITION { int dummy ; } RT_FIELD_DEFINITION ;
RT_ERR rt_get_field_definition             
(                                       
  RT_RAW_FIELD_BUF *           data_buf ,  
                                           
  RT_FIELD                        field ,  
  RT_FIELD_DEFINITION  *           p_fd   // Pointer to a self-defining field        
)                                       ;
  

// Terminate C++ specific code 
#ifdef __cplusplus
}
#endif

#endif    // ifndef _RT_API_H 
