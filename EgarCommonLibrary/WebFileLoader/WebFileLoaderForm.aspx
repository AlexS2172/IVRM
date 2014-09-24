<%@ Page language="c#" Codebehind="WebFileLoaderForm.aspx.cs" AutoEventWireup="false" Inherits="WebFileLoader.WebForm1" %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN" >
<HTML>
	<HEAD>
		<title>WebForm1</title>
		<meta content="Microsoft Visual Studio .NET 7.1" name="GENERATOR">
		<meta content="C#" name="CODE_LANGUAGE">
		<meta content="JavaScript" name="vs_defaultClientScript">
		<meta content="http://schemas.microsoft.com/intellisense/ie5" name="vs_targetSchema">
	</HEAD>
	<body text="#3300cc" bgColor="#ccffff" MS_POSITIONING="GridLayout">
		<form id="Form1" method="post" runat="server">
			<INPUT id="btnUpload" style="Z-INDEX: 102; LEFT: 8px; WIDTH: 120px; POSITION: absolute; TOP: 40px; HEIGHT: 24px"
				type="submit" value="Upload" name="Submit1" runat="server">
			<asp:label id="lblFileUploadMsg" style="Z-INDEX: 114; LEFT: 16px; POSITION: absolute; TOP: 472px"
				runat="server" Height="64px" Font-Bold="True" Width="840px"></asp:label><asp:label id="lblLogingError" style="Z-INDEX: 113; LEFT: 16px; POSITION: absolute; TOP: 560px"
				Font-Bold="True" ForeColor="Red" Runat="server" Width="790px" Visible="False"></asp:label><asp:button id="btnLogin" style="Z-INDEX: 112; LEFT: 216px; POSITION: absolute; TOP: 592px"
				Runat="server" Width="232px" Text="Login" Visible="False"></asp:button><asp:label id="lblLoginMsg" style="Z-INDEX: 111; LEFT: 8px; POSITION: absolute; TOP: 616px"
				runat="server" Height="24px" Font-Bold="True" Width="789px" Visible="False">To upload file into server make login.</asp:label><asp:textbox id="txtDomainName" style="Z-INDEX: 106; LEFT: 96px; POSITION: absolute; TOP: 488px"
				Height="24px" Runat="server" Width="160px" Visible="False"></asp:textbox><asp:label id="lblDomain" style="Z-INDEX: 103; LEFT: 24px; POSITION: absolute; TOP: 496px"
				Height="27px" Font-Bold="True" Runat="server" Visible="False">Domain:</asp:label><asp:label id="lblUserName" style="Z-INDEX: 108; LEFT: 24px; POSITION: absolute; TOP: 528px"
				Height="24px" Font-Bold="True" Runat="server" Visible="False">Username:</asp:label><asp:textbox id="txtUserName" style="Z-INDEX: 104; LEFT: 24px; POSITION: absolute; TOP: 584px"
				Height="24px" Runat="server" Width="160px" Visible="False"></asp:textbox><asp:textbox id="txtPassword" style="Z-INDEX: 105; LEFT: 488px; POSITION: absolute; TOP: 608px"
				Height="24px" Runat="server" Width="160px" TextMode="Password" Visible="False"></asp:textbox><asp:label id="lblPassword" style="Z-INDEX: 107; LEFT: 712px; POSITION: absolute; TOP: 600px"
				Font-Bold="True" Runat="server" Visible="False">Password:</asp:label><asp:label id="lblFileUploadError" style="Z-INDEX: 109; LEFT: 16px; POSITION: absolute; TOP: 536px"
				Height="58px" Font-Bold="True" ForeColor="Red" Runat="server" Width="840px"></asp:label>&nbsp;
			<br>
			<INPUT id="FileInput" style="Z-INDEX: 101; LEFT: 8px; WIDTH: 792px; POSITION: absolute; TOP: 8px; HEIGHT: 22px"
				type="file" size="112" name="File1" runat="server">&nbsp;
			<asp:listbox id="lstbxFiles" style="Z-INDEX: 110; LEFT: 8px; POSITION: absolute; TOP: 128px"
				runat="server" Height="336px" Width="864px"></asp:listbox><asp:label id="lblCurrentUser" style="Z-INDEX: 115; LEFT: 200px; POSITION: absolute; TOP: 40px"
				runat="server" Height="16px" Width="672px">Label</asp:label>
			<asp:Label id="lblMyDoc" style="Z-INDEX: 116; LEFT: 16px; POSITION: absolute; TOP: 88px" runat="server"
				Height="32px" Width="848px">Label</asp:Label></form>
	</body>
</HTML>
