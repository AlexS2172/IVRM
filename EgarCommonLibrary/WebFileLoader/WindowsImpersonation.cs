using System;
using System.Web;
using System.Web.Security;
using System.Security.Principal;
using System.Runtime.InteropServices;

using System.Security.Permissions;



namespace WebFileLoader
{
	/// <summary>
	/// Summary description for WindowsImpersonation.
	/// </summary>
	public class WindowsImpersonation
	{
		public const int LOGON32_LOGON_INTERACTIVE = 2;
		public const int LOGON32_PROVIDER_DEFAULT = 0;

		static WindowsIdentity tempWindowsIdentity;
		static WindowsImpersonationContext impersonationContext;

		[DllImport("advapi32.dll", SetLastError=true,	CharSet=CharSet.Auto)]		
		public static extern int LogonUser(String lpszUserName, 
			String lpszDomain,
			String lpszPassword,
			int dwLogonType, 
			int dwLogonProvider,
			ref IntPtr phToken);
		[DllImport("advapi32.dll", CharSet=System.Runtime.InteropServices.CharSet.Auto,
			 SetLastError=true)]
		public extern static int DuplicateToken(IntPtr hToken, 
			int impersonationLevel,  
			ref IntPtr hNewToken);

		[DllImport("kernel32.dll", CharSet=System.Runtime.InteropServices.CharSet.Auto)]
		private unsafe static extern int FormatMessage(int dwFlags, ref IntPtr lpSource, 
			int dwMessageId, int dwLanguageId, ref String lpBuffer, int nSize, IntPtr *Arguments);

		[DllImport("kernel32.dll", CharSet=CharSet.Auto)]
		public extern static bool CloseHandle(IntPtr handle);

		/*
		[DllImport("shell32.dll", CharSet=CharSet.Auto)]
		int SHGetFolderPath(IntPtr hwndOwner,
			int nFolder,
			IntPtr hToken,
			int dwFlags,
			ref string pszPath
			);

		string GetFolderPath ()
		{
			string _pathl;

			if (SHGetFolderPath (null,CSIDL_PERSONAL))
		}*/



		WindowsImpersonation ()
		{
		}

		static public bool LogonUser (String userName, String domain, String password)
		{
			if (impersonationContext != null)
				undoImpersonation ();

			IntPtr token = IntPtr.Zero;
			IntPtr tokenDuplicate = IntPtr.Zero;

			if(LogonUser(userName, domain, password, LOGON32_LOGON_INTERACTIVE, 
				LOGON32_PROVIDER_DEFAULT, ref token) != 0)
			{
				if(DuplicateToken(token, 2, ref tokenDuplicate) != 0) 
				{
					tempWindowsIdentity = new WindowsIdentity (tokenDuplicate);
					if (Impersonate())
						return true;
					else
					{
						ThrowExeption ();
						return false; 
					}
				}
				else
				{
					ThrowExeption ();
					return false;
				} 
			}
			else
			{
				ThrowExeption ();
				return false;
			}
		}

		static void ThrowExeption ()
		{
			int ret = Marshal.GetLastWin32Error ();
			string strErr = "\nError: [" + ret.ToString () + "] " + GetErrorMessage(ret) + "\n";
			throw new System.Exception (strErr);
		}

		static public bool Impersonate()
		{
			if (tempWindowsIdentity == null)
				return true;
 
			impersonationContext = tempWindowsIdentity.Impersonate();
			if (impersonationContext != null)
				return true;
			else
				return false;			
		}

		static public void undoImpersonation()
		{
			if (impersonationContext != null)
				impersonationContext.Undo();
		}

		// GetErrorMessage formats and returns an error message
		// corresponding to the input errorCode.
		public unsafe static string GetErrorMessage(int errorCode)
		{
			int FORMAT_MESSAGE_ALLOCATE_BUFFER = 0x00000100;
			int FORMAT_MESSAGE_IGNORE_INSERTS = 0x00000200;
			int FORMAT_MESSAGE_FROM_SYSTEM  = 0x00001000;

			//int errorCode = 0x5; //ERROR_ACCESS_DENIED
			//throw new System.ComponentModel.Win32Exception(errorCode);

			int messageSize = 255;
			String lpMsgBuf = "";
			int dwFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;

			IntPtr ptrlpSource = IntPtr.Zero;
			IntPtr prtArguments = IntPtr.Zero;
	        
			int retVal = FormatMessage(dwFlags, ref ptrlpSource, errorCode, 0, ref lpMsgBuf, messageSize, &prtArguments);
			if (0 == retVal)
			{
				throw new Exception("Failed to format message for error code " + errorCode + ". ");
			}

			return lpMsgBuf;
		}

	}

}
