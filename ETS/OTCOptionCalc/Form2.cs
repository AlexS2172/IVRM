using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace OTCOptionCalc
{
	/// <summary>
	/// Summary description for Form2.
	/// </summary>
	/// 
	public class DivStreamForm : System.Windows.Forms.Form
	{
		private DevExpress.XtraGrid.Views.Grid.GridView gridView1;
		private DevExpress.XtraGrid.Columns.GridColumn colDivDate;
		private DevExpress.XtraGrid.Columns.GridColumn colDivAmount;
		private DevExpress.XtraGrid.GridControl gridDivs;
		private DevExpress.LookAndFeel.DefaultLookAndFeel defaultLookAndFeel1;
		private System.Windows.Forms.Button btnAdd;
		private System.ComponentModel.IContainer components;
		private System.Windows.Forms.Button btnRemove;
		private System.Windows.Forms.Button btnRemoveAll;
		private System.Windows.Forms.TextBox tbAmount;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.DateTimePicker dtDate;
		private System.Windows.Forms.Button btnOK;
		private bool bTheFirstTime;
		public ArrayList listDataSource;
		private double dExpDate;
		private System.Windows.Forms.TextBox tbDateStep;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.GroupBox gbAdd;
		private System.Windows.Forms.Button btnCancel;
		private EtsGeneralLib.IEtsDivColl mDivColl;


		public DivStreamForm()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();
			
			listDataSource = new ArrayList();
			bTheFirstTime = true;
			dExpDate = 0;
			mDivColl = new EtsGeneralLib.EtsDivCollClass();
			//
			// TODO: Add any constructor code after InitializeComponent call
			//
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.components = new System.ComponentModel.Container();
			System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(DivStreamForm));
			this.btnAdd = new System.Windows.Forms.Button();
			this.gridDivs = new DevExpress.XtraGrid.GridControl();
			this.gridView1 = new DevExpress.XtraGrid.Views.Grid.GridView();
			this.colDivDate = new DevExpress.XtraGrid.Columns.GridColumn();
			this.colDivAmount = new DevExpress.XtraGrid.Columns.GridColumn();
			this.defaultLookAndFeel1 = new DevExpress.LookAndFeel.DefaultLookAndFeel(this.components);
			this.btnRemove = new System.Windows.Forms.Button();
			this.btnRemoveAll = new System.Windows.Forms.Button();
			this.tbAmount = new System.Windows.Forms.TextBox();
			this.label1 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.dtDate = new System.Windows.Forms.DateTimePicker();
			this.btnOK = new System.Windows.Forms.Button();
			this.tbDateStep = new System.Windows.Forms.TextBox();
			this.label3 = new System.Windows.Forms.Label();
			this.gbAdd = new System.Windows.Forms.GroupBox();
			this.btnCancel = new System.Windows.Forms.Button();
			((System.ComponentModel.ISupportInitialize)(this.gridDivs)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.gridView1)).BeginInit();
			this.gbAdd.SuspendLayout();
			this.SuspendLayout();
			// 
			// btnAdd
			// 
			this.btnAdd.BackColor = System.Drawing.Color.Silver;
			this.btnAdd.Location = new System.Drawing.Point(128, 104);
			this.btnAdd.Name = "btnAdd";
			this.btnAdd.Size = new System.Drawing.Size(72, 23);
			this.btnAdd.TabIndex = 2;
			this.btnAdd.Text = "Add";
			this.btnAdd.Click += new System.EventHandler(this.btnAdd_Click);
			// 
			// gridDivs
			// 
			this.gridDivs.BackColor = System.Drawing.Color.Silver;
			// 
			// gridDivs.EmbeddedNavigator
			// 
			this.gridDivs.EmbeddedNavigator.Name = "";
			this.gridDivs.Location = new System.Drawing.Point(248, 16);
			this.gridDivs.MainView = this.gridView1;
			this.gridDivs.Name = "gridDivs";
			this.gridDivs.Size = new System.Drawing.Size(280, 208);
			this.gridDivs.Styles.AddReplace("ColumnFilterButtonActive", new DevExpress.Utils.ViewStyleEx("ColumnFilterButtonActive", "Grid", System.Drawing.SystemColors.Control, System.Drawing.Color.Blue, System.Drawing.SystemColors.ControlLightLight, System.Drawing.Drawing2D.LinearGradientMode.Horizontal));
			this.gridDivs.TabIndex = 4;
			this.gridDivs.Text = "gridDivs";
			// 
			// gridView1
			// 
			this.gridView1.Columns.AddRange(new DevExpress.XtraGrid.Columns.GridColumn[] {
																							 this.colDivDate,
																							 this.colDivAmount});
			this.gridView1.Name = "gridView1";
			this.gridView1.ShowButtonMode = DevExpress.XtraGrid.Views.Base.ShowButtonModeEnum.ShowAlways;
			// 
			// colDivDate
			// 
			this.colDivDate.Caption = "Dates";
			this.colDivDate.Name = "colDivDate";
			this.colDivDate.VisibleIndex = 0;
			// 
			// colDivAmount
			// 
			this.colDivAmount.Caption = "Amounts";
			this.colDivAmount.Name = "colDivAmount";
			this.colDivAmount.VisibleIndex = 1;
			// 
			// defaultLookAndFeel1
			// 
			this.defaultLookAndFeel1.LookAndFeel.Style = DevExpress.LookAndFeel.LookAndFeelStyle.Style3D;
			// 
			// btnRemove
			// 
			this.btnRemove.BackColor = System.Drawing.Color.Silver;
			this.btnRemove.Location = new System.Drawing.Point(80, 160);
			this.btnRemove.Name = "btnRemove";
			this.btnRemove.Size = new System.Drawing.Size(72, 23);
			this.btnRemove.TabIndex = 5;
			this.btnRemove.Text = "Delete";
			this.btnRemove.Click += new System.EventHandler(this.btnRemove_Click);
			// 
			// btnRemoveAll
			// 
			this.btnRemoveAll.BackColor = System.Drawing.Color.Silver;
			this.btnRemoveAll.Location = new System.Drawing.Point(160, 160);
			this.btnRemoveAll.Name = "btnRemoveAll";
			this.btnRemoveAll.Size = new System.Drawing.Size(72, 23);
			this.btnRemoveAll.TabIndex = 6;
			this.btnRemoveAll.Text = "Clear";
			this.btnRemoveAll.Click += new System.EventHandler(this.btnRemoveAll_Click);
			// 
			// tbAmount
			// 
			this.tbAmount.Location = new System.Drawing.Point(128, 40);
			this.tbAmount.Name = "tbAmount";
			this.tbAmount.Size = new System.Drawing.Size(88, 20);
			this.tbAmount.TabIndex = 7;
			this.tbAmount.Text = "0.0";
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(77, 16);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(35, 16);
			this.label1.TabIndex = 9;
			this.label1.Text = "Date:";
			// 
			// label2
			// 
			this.label2.Location = new System.Drawing.Point(48, 40);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(64, 16);
			this.label2.TabIndex = 10;
			this.label2.Text = "Amount ($):";
			// 
			// dtDate
			// 
			this.dtDate.Format = System.Windows.Forms.DateTimePickerFormat.Short;
			this.dtDate.Location = new System.Drawing.Point(128, 16);
			this.dtDate.Name = "dtDate";
			this.dtDate.Size = new System.Drawing.Size(88, 20);
			this.dtDate.TabIndex = 11;
			// 
			// btnOK
			// 
			this.btnOK.BackColor = System.Drawing.Color.Silver;
			this.btnOK.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.btnOK.Location = new System.Drawing.Point(360, 232);
			this.btnOK.Name = "btnOK";
			this.btnOK.TabIndex = 12;
			this.btnOK.Text = "OK";
			this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
			// 
			// tbDateStep
			// 
			this.tbDateStep.Location = new System.Drawing.Point(128, 72);
			this.tbDateStep.Name = "tbDateStep";
			this.tbDateStep.Size = new System.Drawing.Size(88, 20);
			this.tbDateStep.TabIndex = 13;
			this.tbDateStep.Text = "1";
			// 
			// label3
			// 
			this.label3.Location = new System.Drawing.Point(8, 72);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(112, 16);
			this.label3.TabIndex = 14;
			this.label3.Text = "Add with step (days):";
			// 
			// gbAdd
			// 
			this.gbAdd.Controls.Add(this.dtDate);
			this.gbAdd.Controls.Add(this.label1);
			this.gbAdd.Controls.Add(this.tbAmount);
			this.gbAdd.Controls.Add(this.label2);
			this.gbAdd.Controls.Add(this.label3);
			this.gbAdd.Controls.Add(this.tbDateStep);
			this.gbAdd.Controls.Add(this.btnAdd);
			this.gbAdd.Location = new System.Drawing.Point(8, 8);
			this.gbAdd.Name = "gbAdd";
			this.gbAdd.Size = new System.Drawing.Size(224, 144);
			this.gbAdd.TabIndex = 15;
			this.gbAdd.TabStop = false;
			this.gbAdd.Text = "Add";
			// 
			// btnCancel
			// 
			this.btnCancel.BackColor = System.Drawing.Color.Silver;
			this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.btnCancel.Location = new System.Drawing.Point(448, 232);
			this.btnCancel.Name = "btnCancel";
			this.btnCancel.TabIndex = 16;
			this.btnCancel.Text = "Cancel";
			this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
			// 
			// DivStreamForm
			// 
			this.AcceptButton = this.btnOK;
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.BackColor = System.Drawing.Color.WhiteSmoke;
			this.ClientSize = new System.Drawing.Size(534, 259);
			this.Controls.Add(this.btnCancel);
			this.Controls.Add(this.gbAdd);
			this.Controls.Add(this.btnOK);
			this.Controls.Add(this.btnRemoveAll);
			this.Controls.Add(this.btnRemove);
			this.Controls.Add(this.gridDivs);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MaximizeBox = false;
			this.Name = "DivStreamForm";
			this.Text = "Dividend Stream";
			this.Load += new System.EventHandler(this.DivStreamForm_Load);
			((System.ComponentModel.ISupportInitialize)(this.gridDivs)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.gridView1)).EndInit();
			this.gbAdd.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion



		private void DivStreamForm_Load(object sender, System.EventArgs e)
		{
			
			if (bTheFirstTime)
			{
				bTheFirstTime = false;
				string sToday;
				sToday = DateTime.Today.ToShortDateString();
				dtDate.Text = sToday;
				
				listDataSource.Add(new Record(sToday, "0.00"));				
				gridDivs.DataSource = listDataSource;
				
				
				
				gridDivs.MainView.PopulateColumns();

				
				listDataSource.Clear();
			}
			

			gridDivs.DataSource = listDataSource;
			gridDivs.MainView.PopulateColumns();
			if (listDataSource.Count > 0)
				btnRemove.Enabled		= true;
			else
				btnRemove.Enabled		= false;
			btnRemoveAll.Enabled	= btnRemove.Enabled;

		}


		private bool CheckNoDataDupletsEx(string sData, int iEx)
		{
			if (listDataSource.Count > 0 )
			{
				Record tmpRecord;
				for (int iCount = 0; iCount<listDataSource.Count; iCount++)
				{
					if (iCount == iEx)
						continue;
					tmpRecord = (Record)listDataSource[iCount];
					if (sData == tmpRecord.DivDate)
						return false;
				}
			}
			return true;
		}

		private bool CheckNoDataDuplets(string sData)
		{
			if (listDataSource.Count > 0 )
			{
				Record tmpRecord;
				for (int iCount = 0; iCount<listDataSource.Count; iCount++)
				{
					tmpRecord = (Record)listDataSource[iCount];
					if (sData == tmpRecord.DivDate)
						return false;
				}
			}
			return true;
		}


		private bool CheckDouble(string sInParam)
		{
			
			double dTmp;

			try
			{
				dTmp = Convert.ToDouble(sInParam);
			}
			catch
			{
				MessageBox.Show("Wrong amount: " + sInParam);
				return false;
			}
			return true;
		}

		private bool CheckDoubles()
		{
			
			if (listDataSource.Count > 0 )
			{
				Record tmpRecord;
				for (int iCount = 0; iCount<listDataSource.Count; iCount++)
				{
					tmpRecord = (Record)listDataSource[iCount];
					if (!CheckDouble(tmpRecord.DivAmount))
						return false;
				}
			}		
			return true;		
		}

		private bool CheckDate(string sInDate)
		{
			
			double dDate;

			try
			{
				dDate = DateTime.Parse(sInDate).ToOADate();
			}
			catch
			{
				MessageBox.Show("Wrong date: " + sInDate);
				return false;
			}
			
			/*
			if ((dExpDate > 0)&&(dDate < dExpDate))
			{
				MessageBox.Show("Dividend date must be earlier than an expiration date");
				return false;
			}
			*/

			return true;
		}

		private bool CheckDatesOnOk()
		{
			if (listDataSource.Count > 0 )
			{
				Record tmpRecord;
				for (int iCount = 0; iCount<listDataSource.Count; iCount++)
				{
					tmpRecord = (Record)listDataSource[iCount];
					if (!CheckDate(tmpRecord.DivDate))
						return false;
				}
			}		
			return true;		
		}
		
		private void btnAdd_Click(object sender, System.EventArgs e)
		{

			long lStep;

			if (!CheckDate(dtDate.Text))
				return;
			
			
			if (!CheckDouble(tbAmount.Text))
				return;

			if (Convert.ToDouble(tbAmount.Text) == 0)
			{
				MessageBox.Show("Can't add zero dividend");			
				return;
			}

			if (CheckNoDataDuplets(dtDate.Text))
			{
				btnRemove.Enabled = true;
				btnRemoveAll.Enabled = true;
				listDataSource.Add(new Record(dtDate.Text,tbAmount.Text));
				gridDivs.MainView.PopulateColumns();
				try
				{
					lStep = Convert.ToInt32(tbDateStep.Text);
					dtDate.Value = dtDate.Value.AddDays(lStep);
				}
				catch
				{
					MessageBox.Show("Wrong date step");

				}
				
				
			}
			else
			{
				MessageBox.Show("Dividend date " + dtDate.Text + " already exists");
			}
			
			
		}

		private void btnRemove_Click(object sender, System.EventArgs e)
		{
			//cellValue = gridDivs.MainView.GetRowCellValue(gridDivs.MainView.FocusedRowHandle, gridDivs.MainView.Columns["Date"]).ToString();
			if (listDataSource.Count == 0)
			{
				btnRemove.Enabled = false;
				btnRemoveAll.Enabled = false;
				return;
			}
			int[] iSelRows = gridView1.GetSelectedRows();
			if (iSelRows.GetLength(0) > 0)
				foreach (int iItem in iSelRows)
				{
					if ((iItem < listDataSource.Count )&&(iItem>=0))
						listDataSource.RemoveAt(iItem);	
				}
			
			gridDivs.MainView.PopulateColumns();
			//gridDivs.MainView.
			//gridDivs.EmbeddedNavigator.

		
		}

		private void btnRemoveAll_Click(object sender, System.EventArgs e)
		{
			btnRemove.Enabled = false;
			btnRemoveAll.Enabled = false;
			listDataSource.Clear();
			gridDivs.MainView.PopulateColumns();
		}

		private bool CheckNoDataDupletsOnOk()
		{
			if (listDataSource.Count > 0 )
			{
				Record tmpRecord;
				for (int iCount = 0; iCount<listDataSource.Count; iCount++)
				{
					tmpRecord = (Record)listDataSource[iCount];
					if (!CheckNoDataDupletsEx(tmpRecord.DivDate, iCount))
						return false;
				}
			}
			return true;
		}
		
		private void btnOK_Click(object sender, System.EventArgs e)
		{
			if (!CheckDatesOnOk()||!CheckDoubles())
			{
				return;
			}
			if (CheckNoDataDupletsOnOk())
			{
				this.DialogResult = DialogResult.OK;
				this.Close();
			}
			else
			{
				MessageBox.Show("Duplicated dividend dates found!");
			}
		}

		public int GetCustomDividendsCount()
		{
			return listDataSource.Count;
		}
/*
		private int FillCustDivColl()
		{
			mDivColl.Clear();
			if (listDataSource.Count > 0 )
			{
				Record tmpRecord;
				DateTime dtTmp;
				for (int iCount = 0; iCount<listDataSource.Count; iCount++)
				{
					
					tmpRecord = (Record)listDataSource[iCount];
					dtTmp = DateTime.Parse(tmpRecord.DivDate);
					mDivColl.Add(dtTmp, Convert.ToDouble(tmpRecord.DivAmount));
				}
			}
			return 0;
		}


		public int GetCustDivColl(ref EtsGeneralLib.IEtsDivColl rColl)
		{
			rColl = mDivColl;
			return 0;
		}

*/
		public int GetCustomDividendList(ref ArrayList rList)
		{
			rList =  (ArrayList)listDataSource.Clone();
			return 0;
		}



		public int SetCustomDividendList(ref ArrayList arInList)
		{
			listDataSource = arInList;
			return 0;
		}


		public int SetExpirationDate(double dDate)
		{
			dExpDate = dDate;
			return 0;
		}

		private void btnCancel_Click(object sender, System.EventArgs e)
		{
			this.DialogResult = DialogResult.Cancel;
			this.Close();
		}

	}
	public class Record 
	{
		string sDivDate;
		string sDivAmount;
		public Record(string sInDate, string sInAmount) 
		{
			this.sDivDate = sInDate;
			this.sDivAmount = sInAmount;
		}
			
		public string DivAmount 
		{
			get { return sDivAmount; }
			set { sDivAmount = value; }
		}

		public string DivDate 
		{
			get { return sDivDate ; }
			set { sDivDate  = value; }
		}

	}


}
