using System;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Web;
using System.Web.SessionState;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.HtmlControls;

//using System.Web.Security;
//using System.Security.Principal;
//using System.Runtime.InteropServices;

namespace WebFileLoader
{
	/// <summary>
	/// Summary description for WebForm1.
	/// </summary>
	public class WebForm1 : System.Web.UI.Page
	{
		protected System.Web.UI.HtmlControls.HtmlInputFile FileInput;
		protected System.Web.UI.HtmlControls.HtmlInputButton btnUpload;
		protected System.Web.UI.WebControls.Label lblFileUploadError;
		protected System.Web.UI.WebControls.ListBox lstbxFiles;
		protected System.Web.UI.WebControls.Label lblPassword;
		protected System.Web.UI.WebControls.TextBox txtPassword;
		protected System.Web.UI.WebControls.TextBox txtUserName;
		protected System.Web.UI.WebControls.TextBox txtDomainName;
		protected System.Web.UI.WebControls.Label lblLoginMsg;
		protected System.Web.UI.WebControls.Label lblDomain;
		protected System.Web.UI.WebControls.Label lblUserName;
		protected System.Web.UI.WebControls.Button btnLogin;
		protected System.Web.UI.WebControls.Label lblLogingError;
		protected System.Web.UI.WebControls.Label lblFileUploadMsg;
		protected System.Web.UI.WebControls.Label lblCurrentUser;
		protected System.Web.UI.WebControls.Label lblMyDoc;

		WindowsImpersonation impersonationObj;
	
		private void Page_Load(object sender, System.EventArgs e)
		{
			lblCurrentUser.Text = "Current user: " + System.Security.Principal.WindowsIdentity.GetCurrent().Name;
			lblMyDoc.Text =	"Path: " + GetFolderPath ();

			try
			{
				FillFilesList (GetFolderPath ());
			}
			catch (Exception ex)
			{
				ShowFileUploadError ("Error: " + ex.Message);
			}

			//Environment.SpecialFolder.

			//DirectoryPrepare ();
			//ShowLogingError (System.Security.Principal.WindowsIdentity.GetCurrent().Name + "      \n");
		}

		#region Web Form Designer generated code
		override protected void OnInit(EventArgs e)
		{
			//
			// CODEGEN: This call is required by the ASP.NET Web Form Designer.
			//
			InitializeComponent();
			base.OnInit(e);
		}
		
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{    
			this.btnUpload.ServerClick += new System.EventHandler(this.Submit1_ServerClick);
			this.Load += new System.EventHandler(this.Page_Load);

		}
		#endregion

		private void Submit1_ServerClick(object sender, System.EventArgs e)
		{
			//System.IO.Directory.CreateDirectory ();
			//System.Security.Principal.WindowsIdentity.GetCurrent().Name
			//user = System.Security.Principal.WindowsIdentityGetCurrent ();
			//ShowLogingError (System.Security.Principal.WindowsIdentity.GetCurrent().Name + "      \n");
			//Response.Write (Context.User.Identity.Name);

			if( ( FileInput.PostedFile != null ) && ( FileInput.PostedFile.ContentLength > 0 ) )
			{
				try
				{
					string fn = System.IO.Path.GetFileName(FileInput.PostedFile.FileName);

					string strUserName = System.Security.Principal.WindowsIdentity.GetCurrent().Name;
					int index = strUserName.IndexOf ('\\');
					strUserName = strUserName.Substring (index+1);

					string DirPath = Server.MapPath("Profiles") + "\\" + strUserName + "\\";
					string SaveLocation = DirPath + fn;

					SaveLocation = GetFolderPath () + "\\" + fn;

					//if (DirFind (Server.MapPath("Profiles"),strUserName) == false)
					//	DirectoryPrepare ();

					//ShowLogingError (strUserName, true);

					FileInput.PostedFile.SaveAs(SaveLocation);
					ShowFileUploadMsg ("The file " + FileInput.PostedFile.FileName +  " has been uploaded.");
					FillFilesList (GetFolderPath ());
				}
				catch ( Exception ex )
				{
					ShowFileUploadError ("Error: " + ex.Message);
				}
			}
			else
			{
				ShowFileUploadMsg ("Please select a file to upload.");
			}
		}

		private void btnLogin_Click(object sender, System.EventArgs e)
		{
			try
			{
				ShowLogingError ("", true);

				if (WindowsImpersonation.LogonUser (txtUserName.Text, txtDomainName.Text, txtPassword.Text))
				{
					ShowLogingMsg ("Current user:  " + System.Security.Principal.WindowsIdentity.GetCurrent().Name);
					ShowLogingError ("Login has been made successfully.", true);

					DirectoryPrepare ();
					this.btnUpload.Disabled = false;
					
				}
				else
				{
					ShowLogingError ("Authentication failed, check username and password.", false);
				}
			}
			catch(Exception ex)
			{
				ShowLogingMsg ("Current user:  " + System.Security.Principal.WindowsIdentity.GetCurrent().Name);
				ShowLogingError ("Error authenticating. " + ex.Message, false);
			}
		}

		private void ShowLogingError (string strError, bool bBlue)
		{
			if (bBlue)
				lblLogingError.ForeColor = System.Drawing.Color.Blue;
			else
				lblLogingError.ForeColor = System.Drawing.Color.Red;

			lblLogingError.Text = strError;
		}

		private void ShowLogingMsg (string strError)
		{
			lblLoginMsg.Text = strError;
			lblLogingError.Text = "";
			lblFileUploadError.Text = "";
		}

		private void ShowFileUploadError (string strError)
		{
			lblFileUploadError.Text = strError;
		}

		private void ShowFileUploadMsg (string strError)
		{
			lblFileUploadMsg.Text = strError;
			lblFileUploadError.Text = "";
		}

		private void DirectoryPrepare ()
		{
			string strUserName = System.Security.Principal.WindowsIdentity.GetCurrent().Name;
			int index = strUserName.IndexOf ('\\');
			strUserName = strUserName.Substring (index+1);

			string strProfilesDir = Server.MapPath("Profiles");

			string [] strDir = System.IO.Directory.GetDirectories (strProfilesDir,strUserName);
			if (strDir.Length  < 1)
				System.IO.Directory.CreateDirectory (strProfilesDir + "\\" + strUserName);
			else
			{
				FillFilesList (strProfilesDir + "\\" + strUserName);
			}

			//string SaveLocation = Server.MapPath("Profiles") + "\\" + strUserName + "\\" + fn;
		}

		private bool DirFind (string _paht, string strDir)
		{
			string [] strFiles = System.IO.Directory.GetDirectories (_paht,strDir);
			return strFiles.Length > 0;
		}

		private void FillFilesList (string _Path)
		{
			
			string [] strFiles = System.IO.Directory.GetFiles (_Path);
			lstbxFiles.Items.Clear ();

			foreach (string strName in strFiles)
			{
				lstbxFiles.Items.Add (System.IO.Path.GetFileName (strName));
			}
			
			/*
			IDictionary    environmentVariables = Environment.GetEnvironmentVariables();
			foreach (DictionaryEntry de in environmentVariables)
			{
				lstbxFiles.Items.Add (de.Key + " = " + de.Value);
			}
*/
		}

		private string GetFolderPath ()
		{
			//return Environment.GetFolderPath (Environment.SpecialFolder.Personal);

			string strUserName = System.Security.Principal.WindowsIdentity.GetCurrent().Name;
			int index = strUserName.IndexOf ('\\');
			strUserName = strUserName.Substring (index+1);
			return "C:\\Documents and Settings\\" + strUserName + "\\My Documents";
		}
	}

		
	
}
