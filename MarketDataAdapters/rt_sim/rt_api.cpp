#include "stdafx.h"
#include "rt_API.h" 

inline BOOL internalIsEqualGUID(REFGUID rguid1, REFGUID rguid2)
{
   return (
	  ((PLONG) &rguid1)[0] == ((PLONG) &rguid2)[0] &&
	  ((PLONG) &rguid1)[1] == ((PLONG) &rguid2)[1] &&
	  ((PLONG) &rguid1)[2] == ((PLONG) &rguid2)[2] &&
	  ((PLONG) &rguid1)[3] == ((PLONG) &rguid2)[3]);
}

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
)                                       
{
   RT_ERR ret =  RT_ERR_CONNECTION_NOT_OPENED;

   RT_CONNECT_ID id = 0;
   if(!username) return RT_ERR_BAD_USERNAME;
   if(!user_id) return RT_ERR_BAD_USER_ID;
   
   ret  = g_RT.Connect(rt_id, connect_clbk);
   return ret;   
}; 

RT_ERR rt_set_shutdown_callback(RT_CONNECT_ID rt_id, RT_SHUTDOWN_CLBK shutdown_clbk)
{
	RT_ERR ret = RT_ERR_CONNECTION_INVALID;
	if(rt_id)
	{
		ret = g_RT.SetShutdownNotifier(rt_id, shutdown_clbk);
	}
	return ret;
}
 
RT_ERR rt_shutdown(RT_CONNECT_ID rt_id)
{
	RT_ERR ret = RT_ERR_CONNECTION_INVALID;
	if(rt_id)
		ret = g_RT.Disconnect(rt_id);
	return ret;
}

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
)
{
 RT_ERR ret = RT_ERR_CONNECTION_INVALID;
 if(req_id_ptr_to_fill && symbol && field_count>0 && fields)
	 ret = g_RT.RequestSymbol(rt_id, req_id_ptr_to_fill, user_data, req_type, symbol,field_count,fields,callback);

 return ret;
}




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
)
{
 RT_ERR ret = RT_ERR_CONNECTION_INVALID;
 if(req_id_ptr_to_fill && handle && field_count>0 && fields)
	 ret = g_RT.RequestHandle(rt_id, req_id_ptr_to_fill, user_data, req_type, handle, field_count,fields,callback);

 return ret;
}


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
)
{
 RT_ERR ret = RT_ERR_CONNECTION_INVALID;
 if(req_id_ptr_to_fill && handle )
	 ret = g_RT.RequestGroup(rt_id, req_id_ptr_to_fill, user_data, group_type, handle, callback);
 return ret;

}


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
{
	return g_RT.RemoveFields(rt_id, req_id, field_count, fields);
}



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
)
{
	return g_RT.AddFields(rt_id, req_id, field_count, fields);
}



//---------------------------------------------------------------------------- 
RT_ERR  rt_remove_all_symbols             // Turn off all symbol minders               
                                          //  returns:                                 
                                          //        RT_ERR_OK                          
                                          //        RT_ERR_CONNECTION_INVALID          
                                          //        RT_ERR_CONNECTION_NOT_OPEN         
(
  RT_CONNECT_ID                   rt_id           
)                                       
{
	return g_RT.RemoveSymbols(rt_id);

}


RT_ERR  rt_get_field_int32                // Returns field value as an integer                   
                                          // returns:                              
                                          //        RT_ERR_OK                      
                                          //        RT_ERR_INVALID_FIELD           
(
  RT_RAW_FIELD_BUF *           data_buf , // Pointer to raw_data_buffer from    
                                          //   RT_GET_FIELDS_CLBK callback routine.   
  RT_FIELD                        field , // Field id whose "int32" data is needed              
  /*unsigned*/ int *         int_out_ptr   // address of output "int32" value
)                                       
{
	if(data_buf)
	{
		long lNum = *(long*)data_buf;

		if(internalIsEqualGUID(*((GUID*)(data_buf+sizeof(long))), guid_Buffer))
		{
			CRespBuffer* pBuffer = reinterpret_cast<CRespBuffer*>(data_buf + sizeof(long)+ sizeof(GUID));
			for(long i = 0;i<lNum;i++)
			{
				if(pBuffer[i].m_rtField != field) continue;
				switch(pBuffer[i].m_fldType)
				{
				case RT_INT32:
					*int_out_ptr = *((int*)pBuffer[i].m_buffer) ;
					break;
				case RT_UINT32:
					*int_out_ptr = *((UINT*)pBuffer[i].m_buffer);
					break;
				case RT_CHAR:
				case RT_REAL:
				case RT_TIME:
				case RT_DATE:
					return RT_ERR_INVALID_FIELD;
				}				
			}
			return RT_ERR_OK;
		}
	}
	return RT_ERR_INVALID_FIELD;
}

  
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
)
{
	if(data_buf)
	{
		long lNum = *(long*)data_buf;

		if(internalIsEqualGUID(*((GUID*)(data_buf+sizeof(long))), guid_Buffer))
		{
			CRespBuffer* pBuffer = reinterpret_cast<CRespBuffer*>(data_buf + sizeof(long)+ sizeof(GUID));
			for(long i = 0;i<lNum;i++)
			{
				if(pBuffer[i].m_rtField != field) continue;
				switch(pBuffer[i].m_fldType)
				{
				case RT_INT32:
					*real_out_ptr = *((int*)pBuffer[i].m_buffer) ;
					break;
				case RT_UINT32:
					*real_out_ptr = *((UINT*)pBuffer[i].m_buffer);
					break;
				case RT_REAL:
					*real_out_ptr = *((double*)pBuffer[i].m_buffer);
					break;
				case RT_CHAR:
				case RT_TIME:
				case RT_DATE:
					return RT_ERR_INVALID_FIELD;
				}				
			}
			return RT_ERR_OK;
		}
	}
	return RT_ERR_INVALID_FIELD;
}

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
)                                       
{
	if(data_buf)
	{
		long lNum = *(long*)data_buf;

		if(internalIsEqualGUID(*((GUID*)(data_buf+sizeof(long))), guid_Buffer))
		{
			CRespBuffer* pBuffer = reinterpret_cast<CRespBuffer*>(data_buf + sizeof(long)+ sizeof(GUID));
			for(long i = 0;i<lNum;i++)
			{
				if(pBuffer[i].m_rtField != field) continue;
				switch(pBuffer[i].m_fldType)
				{
				case RT_CHAR:
					ZeroMemory(out_string_ptr, max_length);
					strncpy(out_string_ptr, ((char*)pBuffer[i].m_buffer) ,max_length);
					break;
				case RT_INT32:
					{
						int  iData = *((int*)pBuffer[i].m_buffer);
						_snprintf(out_string_ptr, max_length, "%d", iData);
					}
					break;
				case RT_UINT32:
					{
						UINT iData = *((UINT*)pBuffer[i].m_buffer);
						_snprintf(out_string_ptr, max_length, "%u", iData);
					}					
					break;
				case RT_REAL:
					{
						double fData = *((double*)pBuffer[i].m_buffer);
						_snprintf(out_string_ptr, max_length, "%f", fData);
					}					
					break;
				case RT_TIME:
				case RT_DATE:
					return RT_ERR_INVALID_FIELD;
				}				
			}
			*out_length_ptr = strlen(out_string_ptr);
			return RT_ERR_OK;
		}
	}
	return RT_ERR_INVALID_FIELD;
}

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
)                                       
{
	return RT_ERR_OK;
}
