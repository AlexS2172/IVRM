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
		private DevExpress.XtraGrid.Columns.GridColumn colDivDate;
		private DevExpress.XtraGrid.Columns.GridColumn colDivAmount;
		private DevExpress.XtraGrid.GridControl gridDivs;
        private DevExpress.LookAndFeel.DefaultLookAndFeel defaultLookAndFeel1;
        private System.ComponentModel.IContainer components;
		private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
		private bool bTheFirstTime;
		private ArrayList listDataSource;
        private ArrayList listDataSourceVisible;
        private double dExpDate;
        private System.Windows.Forms.Label label3;
        private DevExpress.XtraEditors.SimpleButton OK;
        private DevExpress.XtraEditors.SimpleButton btnCancel;
        private DevExpress.XtraEditors.CalcEdit tbAmount;
        private DevExpress.XtraEditors.GroupControl groupControl1;
        private DevExpress.XtraEditors.DateEdit dtDate;
        private DevExpress.XtraEditors.SimpleButton btnAdd;
        private DevExpress.XtraEditors.SimpleButton btnRemove;
        private DevExpress.XtraEditors.SimpleButton btnRemoveAll;
        private DevExpress.XtraEditors.TextEdit tbDateStep;
		private DevExpress.XtraGrid.Views.Grid.GridView gridDividends;
		//private EtsGeneralLib.IEtsDivColl mDivColl; // custom dividends are temporary disabled

		public DivStreamForm()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();
			
			listDataSource          = new ArrayList();
            listDataSourceVisible   = new ArrayList();
            bTheFirstTime = true;
			dExpDate = 0;
			//mDivColl = new EtsGeneralLib.EtsDivCollClass();
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(DivStreamForm));
            this.gridDivs = new DevExpress.XtraGrid.GridControl();
            this.gridDividends = new DevExpress.XtraGrid.Views.Grid.GridView();
            this.colDivDate = new DevExpress.XtraGrid.Columns.GridColumn();
            this.colDivAmount = new DevExpress.XtraGrid.Columns.GridColumn();
            this.defaultLookAndFeel1 = new DevExpress.LookAndFeel.DefaultLookAndFeel(this.components);
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.OK = new DevExpress.XtraEditors.SimpleButton();
            this.btnCancel = new DevExpress.XtraEditors.SimpleButton();
            this.tbAmount = new DevExpress.XtraEditors.CalcEdit();
            this.groupControl1 = new DevExpress.XtraEditors.GroupControl();
            this.tbDateStep = new DevExpress.XtraEditors.TextEdit();
            this.dtDate = new DevExpress.XtraEditors.DateEdit();
            this.btnAdd = new DevExpress.XtraEditors.SimpleButton();
            this.btnRemove = new DevExpress.XtraEditors.SimpleButton();
            this.btnRemoveAll = new DevExpress.XtraEditors.SimpleButton();
            ((System.ComponentModel.ISupportInitialize)(this.gridDivs)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.gridDividends)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbAmount.Properties)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.groupControl1)).BeginInit();
            this.groupControl1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.tbDateStep.Properties)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dtDate.Properties)).BeginInit();
            this.SuspendLayout();
            // 
            // gridDivs
            // 
            this.gridDivs.EmbeddedNavigator.Name = "";
            this.gridDivs.Location = new System.Drawing.Point(8, 121);
            this.gridDivs.MainView = this.gridDividends;
            this.gridDivs.Name = "gridDivs";
            this.gridDivs.Size = new System.Drawing.Size(256, 343);
            this.gridDivs.TabIndex = 5;
            this.gridDivs.ViewCollection.AddRange(new DevExpress.XtraGrid.Views.Base.BaseView[] {
            this.gridDividends});
            // 
            // gridDividends
            // 
            this.gridDividends.Appearance.ColumnFilterButton.BackColor = System.Drawing.Color.DarkGray;
            this.gridDividends.Appearance.ColumnFilterButton.BorderColor = System.Drawing.Color.DarkGray;
            this.gridDividends.Appearance.ColumnFilterButton.ForeColor = System.Drawing.Color.DimGray;
            this.gridDividends.Appearance.ColumnFilterButton.Options.UseBackColor = true;
            this.gridDividends.Appearance.ColumnFilterButton.Options.UseBorderColor = true;
            this.gridDividends.Appearance.ColumnFilterButton.Options.UseForeColor = true;
            this.gridDividends.Appearance.ColumnFilterButtonActive.BackColor = System.Drawing.Color.DarkGray;
            this.gridDividends.Appearance.ColumnFilterButtonActive.BorderColor = System.Drawing.Color.DarkGray;
            this.gridDividends.Appearance.ColumnFilterButtonActive.ForeColor = System.Drawing.Color.Gainsboro;
            this.gridDividends.Appearance.ColumnFilterButtonActive.Options.UseBackColor = true;
            this.gridDividends.Appearance.ColumnFilterButtonActive.Options.UseBorderColor = true;
            this.gridDividends.Appearance.ColumnFilterButtonActive.Options.UseForeColor = true;
            this.gridDividends.Appearance.Empty.BackColor = System.Drawing.Color.White;
            this.gridDividends.Appearance.Empty.BackColor2 = System.Drawing.Color.WhiteSmoke;
            this.gridDividends.Appearance.Empty.GradientMode = System.Drawing.Drawing2D.LinearGradientMode.BackwardDiagonal;
            this.gridDividends.Appearance.Empty.Options.UseBackColor = true;
            this.gridDividends.Appearance.EvenRow.BackColor = System.Drawing.Color.White;
            this.gridDividends.Appearance.EvenRow.Options.UseBackColor = true;
            this.gridDividends.Appearance.FilterCloseButton.BackColor = System.Drawing.Color.Gray;
            this.gridDividends.Appearance.FilterCloseButton.BorderColor = System.Drawing.Color.Gray;
            this.gridDividends.Appearance.FilterCloseButton.Options.UseBackColor = true;
            this.gridDividends.Appearance.FilterCloseButton.Options.UseBorderColor = true;
            this.gridDividends.Appearance.FilterPanel.BackColor = System.Drawing.Color.Gray;
            this.gridDividends.Appearance.FilterPanel.ForeColor = System.Drawing.Color.Black;
            this.gridDividends.Appearance.FilterPanel.Options.UseBackColor = true;
            this.gridDividends.Appearance.FilterPanel.Options.UseForeColor = true;
            this.gridDividends.Appearance.FocusedRow.BackColor = System.Drawing.Color.Black;
            this.gridDividends.Appearance.FocusedRow.ForeColor = System.Drawing.Color.White;
            this.gridDividends.Appearance.FocusedRow.Options.UseBackColor = true;
            this.gridDividends.Appearance.FocusedRow.Options.UseForeColor = true;
            this.gridDividends.Appearance.FooterPanel.BackColor = System.Drawing.Color.DarkGray;
            this.gridDividends.Appearance.FooterPanel.BorderColor = System.Drawing.Color.DarkGray;
            this.gridDividends.Appearance.FooterPanel.Options.UseBackColor = true;
            this.gridDividends.Appearance.FooterPanel.Options.UseBorderColor = true;
            this.gridDividends.Appearance.GroupButton.BackColor = System.Drawing.Color.Silver;
            this.gridDividends.Appearance.GroupButton.BorderColor = System.Drawing.Color.Silver;
            this.gridDividends.Appearance.GroupButton.Options.UseBackColor = true;
            this.gridDividends.Appearance.GroupButton.Options.UseBorderColor = true;
            this.gridDividends.Appearance.GroupFooter.BackColor = System.Drawing.Color.Silver;
            this.gridDividends.Appearance.GroupFooter.BorderColor = System.Drawing.Color.Silver;
            this.gridDividends.Appearance.GroupFooter.Options.UseBackColor = true;
            this.gridDividends.Appearance.GroupFooter.Options.UseBorderColor = true;
            this.gridDividends.Appearance.GroupPanel.BackColor = System.Drawing.Color.WhiteSmoke;
            this.gridDividends.Appearance.GroupPanel.ForeColor = System.Drawing.Color.White;
            this.gridDividends.Appearance.GroupPanel.Options.UseBackColor = true;
            this.gridDividends.Appearance.GroupPanel.Options.UseForeColor = true;
            this.gridDividends.Appearance.GroupRow.BackColor = System.Drawing.Color.Silver;
            this.gridDividends.Appearance.GroupRow.Font = new System.Drawing.Font("Tahoma", 8F, System.Drawing.FontStyle.Bold);
            this.gridDividends.Appearance.GroupRow.Options.UseBackColor = true;
            this.gridDividends.Appearance.GroupRow.Options.UseFont = true;
            this.gridDividends.Appearance.HeaderPanel.BackColor = System.Drawing.Color.DarkGray;
            this.gridDividends.Appearance.HeaderPanel.BorderColor = System.Drawing.Color.DarkGray;
            this.gridDividends.Appearance.HeaderPanel.Options.UseBackColor = true;
            this.gridDividends.Appearance.HeaderPanel.Options.UseBorderColor = true;
            this.gridDividends.Appearance.HideSelectionRow.BackColor = System.Drawing.Color.LightSlateGray;
            this.gridDividends.Appearance.HideSelectionRow.Options.UseBackColor = true;
            this.gridDividends.Appearance.HorzLine.BackColor = System.Drawing.Color.LightGray;
            this.gridDividends.Appearance.HorzLine.Options.UseBackColor = true;
            this.gridDividends.Appearance.OddRow.BackColor = System.Drawing.Color.WhiteSmoke;
            this.gridDividends.Appearance.OddRow.Options.UseBackColor = true;
            this.gridDividends.Appearance.Preview.BackColor = System.Drawing.Color.Gainsboro;
            this.gridDividends.Appearance.Preview.ForeColor = System.Drawing.Color.DimGray;
            this.gridDividends.Appearance.Preview.Options.UseBackColor = true;
            this.gridDividends.Appearance.Preview.Options.UseForeColor = true;
            this.gridDividends.Appearance.Row.BackColor = System.Drawing.Color.White;
            this.gridDividends.Appearance.Row.Options.UseBackColor = true;
            this.gridDividends.Appearance.RowSeparator.BackColor = System.Drawing.Color.DimGray;
            this.gridDividends.Appearance.RowSeparator.Options.UseBackColor = true;
            this.gridDividends.Appearance.SelectedRow.BackColor = System.Drawing.Color.DimGray;
            this.gridDividends.Appearance.SelectedRow.Options.UseBackColor = true;
            this.gridDividends.Appearance.VertLine.BackColor = System.Drawing.Color.LightGray;
            this.gridDividends.Appearance.VertLine.Options.UseBackColor = true;
            this.gridDividends.BestFitMaxRowCount = 10;
            this.gridDividends.BorderStyle = DevExpress.XtraEditors.Controls.BorderStyles.Office2003;
            this.gridDividends.Columns.AddRange(new DevExpress.XtraGrid.Columns.GridColumn[] {
            this.colDivDate,
            this.colDivAmount});
            this.gridDividends.GridControl = this.gridDivs;
            this.gridDividends.Name = "gridDividends";
            this.gridDividends.OptionsCustomization.AllowColumnMoving = false;
            this.gridDividends.OptionsMenu.EnableColumnMenu = false;
            this.gridDividends.OptionsView.EnableAppearanceEvenRow = true;
            this.gridDividends.OptionsView.EnableAppearanceOddRow = true;
            this.gridDividends.OptionsView.ShowFilterPanel = false;
            this.gridDividends.OptionsView.ShowGroupPanel = false;
            this.gridDividends.OptionsView.ShowIndicator = false;
            this.gridDividends.PaintStyleName = "MixedXP";
            this.gridDividends.ShowButtonMode = DevExpress.XtraGrid.Views.Base.ShowButtonModeEnum.ShowForFocusedRow;
            // 
            // colDivDate
            // 
            this.colDivDate.Caption = "Dates";
            this.colDivDate.Name = "colDivDate";
            this.colDivDate.OptionsColumn.ShowInCustomizationForm = false;
            this.colDivDate.Visible = true;
            this.colDivDate.VisibleIndex = 0;
            // 
            // colDivAmount
            // 
            this.colDivAmount.Caption = "Amounts";
            this.colDivAmount.Name = "colDivAmount";
            this.colDivAmount.OptionsColumn.ShowInCustomizationForm = false;
            this.colDivAmount.Visible = true;
            this.colDivAmount.VisibleIndex = 1;
            // 
            // defaultLookAndFeel1
            // 
            this.defaultLookAndFeel1.LookAndFeel.SkinName = "Money Twins";
            this.defaultLookAndFeel1.LookAndFeel.Style = DevExpress.LookAndFeel.LookAndFeelStyle.Office2003;
            this.defaultLookAndFeel1.LookAndFeel.UseWindowsXPTheme = false;
            // 
            // label1
            // 
            this.label1.Location = new System.Drawing.Point(79, 28);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(35, 16);
            this.label1.TabIndex = 9;
            this.label1.Text = "Date:";
            // 
            // label2
            // 
            this.label2.Location = new System.Drawing.Point(50, 51);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(64, 16);
            this.label2.TabIndex = 10;
            this.label2.Text = "Amount ($):";
            // 
            // label3
            // 
            this.label3.Location = new System.Drawing.Point(10, 74);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(110, 16);
            this.label3.TabIndex = 14;
            this.label3.Text = "Add with step (days):";
            // 
            // OK
            // 
            this.OK.ButtonStyle = DevExpress.XtraEditors.Controls.BorderStyles.Office2003;
            this.OK.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.OK.Location = new System.Drawing.Point(270, 410);
            this.OK.Name = "OK";
            this.OK.Size = new System.Drawing.Size(88, 24);
            this.OK.TabIndex = 9;
            this.OK.Text = "OK";
            this.OK.Click += new System.EventHandler(this.OK_Click);
            // 
            // btnCancel
            // 
            this.btnCancel.ButtonStyle = DevExpress.XtraEditors.Controls.BorderStyles.Office2003;
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(270, 440);
            this.btnCancel.LookAndFeel.SkinName = "Coffee";
            this.btnCancel.LookAndFeel.Style = DevExpress.LookAndFeel.LookAndFeelStyle.Office2003;
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(88, 24);
            this.btnCancel.TabIndex = 10;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // tbAmount
            // 
            this.tbAmount.EditValue = new decimal(new int[] {
            0,
            0,
            0,
            0});
            this.tbAmount.Location = new System.Drawing.Point(122, 51);
            this.tbAmount.Name = "tbAmount";
            this.tbAmount.Properties.BorderStyle = DevExpress.XtraEditors.Controls.BorderStyles.Simple;
            this.tbAmount.Properties.Buttons.AddRange(new DevExpress.XtraEditors.Controls.EditorButton[] {
            new DevExpress.XtraEditors.Controls.EditorButton(DevExpress.XtraEditors.Controls.ButtonPredefines.Combo)});
            this.tbAmount.Properties.Mask.EditMask = "c5";
            this.tbAmount.Size = new System.Drawing.Size(128, 20);
            this.tbAmount.TabIndex = 3;
            // 
            // groupControl1
            // 
            this.groupControl1.Controls.Add(this.tbDateStep);
            this.groupControl1.Controls.Add(this.dtDate);
            this.groupControl1.Controls.Add(this.tbAmount);
            this.groupControl1.Controls.Add(this.label1);
            this.groupControl1.Controls.Add(this.label3);
            this.groupControl1.Controls.Add(this.label2);
            this.groupControl1.Location = new System.Drawing.Point(8, 8);
            this.groupControl1.Name = "groupControl1";
            this.groupControl1.Size = new System.Drawing.Size(256, 107);
            this.groupControl1.TabIndex = 1;
            this.groupControl1.Text = "Add";
            // 
            // tbDateStep
            // 
            this.tbDateStep.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.tbDateStep.EditValue = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.tbDateStep.Location = new System.Drawing.Point(121, 77);
            this.tbDateStep.Name = "tbDateStep";
            this.tbDateStep.Properties.AllowNullInput = DevExpress.Utils.DefaultBoolean.False;
            this.tbDateStep.Properties.BorderStyle = DevExpress.XtraEditors.Controls.BorderStyles.Simple;
            this.tbDateStep.Properties.Mask.EditMask = "d";
            this.tbDateStep.Properties.Mask.MaskType = DevExpress.XtraEditors.Mask.MaskType.Numeric;
            this.tbDateStep.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.tbDateStep.Size = new System.Drawing.Size(130, 20);
            this.tbDateStep.TabIndex = 4;
            // 
            // dtDate
            // 
            this.dtDate.EditValue = new System.DateTime(2005, 12, 6, 0, 0, 0, 0);
            this.dtDate.Location = new System.Drawing.Point(122, 24);
            this.dtDate.Name = "dtDate";
            this.dtDate.Properties.BorderStyle = DevExpress.XtraEditors.Controls.BorderStyles.Simple;
            this.dtDate.Properties.Buttons.AddRange(new DevExpress.XtraEditors.Controls.EditorButton[] {
            new DevExpress.XtraEditors.Controls.EditorButton(DevExpress.XtraEditors.Controls.ButtonPredefines.Combo)});
            this.dtDate.Size = new System.Drawing.Size(128, 20);
            this.dtDate.TabIndex = 2;
            // 
            // btnAdd
            // 
            this.btnAdd.ButtonStyle = DevExpress.XtraEditors.Controls.BorderStyles.Office2003;
            this.btnAdd.Location = new System.Drawing.Point(270, 91);
            this.btnAdd.Name = "btnAdd";
            this.btnAdd.Size = new System.Drawing.Size(88, 24);
            this.btnAdd.TabIndex = 6;
            this.btnAdd.Text = "Add";
            this.btnAdd.Click += new System.EventHandler(this.btnAdd_Click);
            // 
            // btnRemove
            // 
            this.btnRemove.ButtonStyle = DevExpress.XtraEditors.Controls.BorderStyles.Office2003;
            this.btnRemove.Location = new System.Drawing.Point(270, 121);
            this.btnRemove.Name = "btnRemove";
            this.btnRemove.Size = new System.Drawing.Size(88, 24);
            this.btnRemove.TabIndex = 7;
            this.btnRemove.Text = "Delete";
            this.btnRemove.Click += new System.EventHandler(this.btnRemove_Click);
            // 
            // btnRemoveAll
            // 
            this.btnRemoveAll.ButtonStyle = DevExpress.XtraEditors.Controls.BorderStyles.Office2003;
            this.btnRemoveAll.Location = new System.Drawing.Point(270, 151);
            this.btnRemoveAll.Name = "btnRemoveAll";
            this.btnRemoveAll.Size = new System.Drawing.Size(88, 24);
            this.btnRemoveAll.TabIndex = 8;
            this.btnRemoveAll.Text = "Delete All";
            this.btnRemoveAll.Click += new System.EventHandler(this.btnRemoveAll_Click);
            // 
            // DivStreamForm
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.BackColor = System.Drawing.Color.Gainsboro;
            this.ClientSize = new System.Drawing.Size(365, 472);
            this.Controls.Add(this.btnRemoveAll);
            this.Controls.Add(this.btnRemove);
            this.Controls.Add(this.btnAdd);
            this.Controls.Add(this.groupControl1);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.OK);
            this.Controls.Add(this.gridDivs);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "DivStreamForm";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Dividend Stream";
            this.Load += new System.EventHandler(this.DivStreamForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.gridDivs)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.gridDividends)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbAmount.Properties)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.groupControl1)).EndInit();
            this.groupControl1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.tbDateStep.Properties)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dtDate.Properties)).EndInit();
            this.ResumeLayout(false);

		}
		#endregion

		public void SetFirstTime()
		{
			bTheFirstTime = true;
		}

        private void CloneData(ref ArrayList rListTo, ref ArrayList rListFrom)
        {
            rListTo.Clear();
            for(int i = 0 ;i < rListFrom.Count; ++i)
            {
                Object rO = rListFrom[i];
                rListTo.Add(new Record(ref rO));
            }
        }
        private void DivStreamForm_Load(object sender, System.EventArgs e)
		{

			if (bTheFirstTime)
			{
				bTheFirstTime = false;
				if (listDataSourceVisible.Count == 0)
				{

                    CloneData(ref listDataSourceVisible, ref listDataSource);
//                    listDataSourceVisible = (ArrayList)listDataSource.Clone();
                    gridDivs.DataSource = listDataSourceVisible;
					gridDivs.MainView.PopulateColumns();
					//listDataSourceVisible.Clear();
                    listDataSource.Clear();
				}
			}
            else
            {
                CloneData(ref listDataSourceVisible, ref listDataSource);
//                listDataSourceVisible = (ArrayList)listDataSource.Clone();
                gridDivs.DataSource = listDataSourceVisible;
            }
			
			gridDivs.MainView.PopulateColumns();
            if (listDataSourceVisible.Count > 0)
				btnRemove.Enabled		= true;
			else
				btnRemove.Enabled		= false;
			btnRemoveAll.Enabled	= btnRemove.Enabled;

		}


		private bool CheckNoDataDupletsEx(DateTime dtData, int iEx)
		{
            if (listDataSourceVisible.Count > 0)
			{
				Record tmpRecord;
                for (int iCount = 0; iCount < listDataSourceVisible.Count; iCount++)
				{
					if (iCount == iEx)
						continue;
                    tmpRecord = (Record)listDataSourceVisible[iCount];
					if (dtData == tmpRecord.DivDate)
						return false;
				}
			}
			return true;
		}

		private bool CheckNoDataDuplets(DateTime dtData)
		{
            if (listDataSourceVisible.Count > 0)
			{
				Record tmpRecord;
                for (int iCount = 0; iCount < listDataSourceVisible.Count; iCount++)
				{
                    tmpRecord = (Record)listDataSourceVisible[iCount];
					if (dtData == tmpRecord.DivDate)
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

//		private bool CheckDoubles()
//		{
//			
//			if (listDataSource.Count > 0 )
//			{
//				Record tmpRecord;
//				for (int iCount = 0; iCount<listDataSource.Count; iCount++)
//				{
//					tmpRecord = (Record)listDataSource[iCount];
//					if (!CheckDouble(tmpRecord.DivAmount))
//						return false;
//				}
//			}		
//			return true;		
//		}


		private bool CheckDate(DateTime dtData)
		{
			return true;
//			double dDate;
//
//			try
//			{
//				dDate = DateTime.Parse(sInDate).ToOADate();
//			}
//			catch
//			{
//				MessageBox.Show("Wrong date: " + sInDate);
//				return false;
//			}
//			
//			/*
//			if ((dExpDate > 0)&&(dDate < dExpDate))
//			{
//				MessageBox.Show("Dividend date must be earlier than an expiration date");
//				return false;
//			}
//			*/
//
//			return true;
		}

		private bool CheckDatesOnOk()
		{
            if (listDataSourceVisible.Count > 0)
			{
				Record tmpRecord;
                for (int iCount = 0; iCount < listDataSourceVisible.Count; iCount++)
				{
                    tmpRecord = (Record)listDataSourceVisible[iCount];
					if (!CheckDate(tmpRecord.DivDate))
						return false;
				}
			}		
			return true;		
		}
		
		private void btnAdd_Click(object sender, System.EventArgs e)
		{

			long lStep;


			if (!CheckDate(dtDate.DateTime))
				return;
			
            if (Convert.ToDouble(tbAmount.Value) == 0)
			{
				MessageBox.Show("Can't add zero dividend");			
				return;
			}

            if (CheckNoDataDuplets(dtDate.DateTime))
			{
				btnRemove.Enabled = true;
				btnRemoveAll.Enabled = true;
                listDataSourceVisible.Add(new Record(dtDate.DateTime, Convert.ToDouble(tbAmount.Value)));
				gridDivs.MainView.PopulateColumns();
				try
				{
					lStep = Convert.ToInt32(tbDateStep.Text);
                    dtDate.DateTime = dtDate.DateTime.AddDays(lStep);
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
            if (listDataSourceVisible.Count == 0)
			{
				btnRemove.Enabled = false;
				btnRemoveAll.Enabled = false;
				return;
			}
			int[] iSelRows = gridDividends.GetSelectedRows();
			if (iSelRows.GetLength(0) > 0)
				foreach (int iItem in iSelRows)
				{
                    if ((iItem < listDataSourceVisible.Count) && (iItem >= 0))
                        listDataSourceVisible.RemoveAt(iItem);	
				}
			
			gridDivs.MainView.PopulateColumns();
			//gridDivs.MainView.
			//gridDivs.EmbeddedNavigator.

		
		}

		private void btnRemoveAll_Click(object sender, System.EventArgs e)
		{
			btnRemove.Enabled = false;
			btnRemoveAll.Enabled = false;
            listDataSourceVisible.Clear();
			gridDivs.MainView.PopulateColumns();
		}

		private bool CheckNoDataDupletsOnOk()
		{
            if (listDataSourceVisible.Count > 0)
			{
				Record tmpRecord;
                for (int iCount = 0; iCount < listDataSourceVisible.Count; iCount++)
				{
                    tmpRecord = (Record)listDataSourceVisible[iCount];
					if (!CheckNoDataDupletsEx(tmpRecord.DivDate, iCount))
						return false;
				}
			}
			return true;
		}
		
		private void btnOK_Click(object sender, System.EventArgs e)
		{
			if (!CheckDatesOnOk())
			{
				return;
			}
			if (CheckNoDataDupletsOnOk())
			{
                CloneData(ref listDataSource, ref listDataSourceVisible);
                listDataSource = (ArrayList)listDataSourceVisible.Clone();
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
            return listDataSourceVisible.Count;
		}

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
            if (listDataSource.Count <= 0)
            {
                listDataSourceVisible.Clear();
                bTheFirstTime = true;
            }

			this.DialogResult = DialogResult.Cancel;
			this.Close();
		}


		public int GetCustomDividends(ref double[] dDivAmounts, ref double[] dDivYears)
		{
			
			if (listDataSource.Count > 0 )
			{
				Record tmpRecord;
				DateTime dtTmp;
								
				for (int iCount = 0; iCount<listDataSource.Count; iCount++)
				{
					
					tmpRecord = (Record)listDataSource[iCount];
					dtTmp = tmpRecord.DivDate;
					dDivAmounts[iCount]	=	Convert.ToDouble(tmpRecord.DivAmount); 
					dDivYears[iCount]	=	 Convert.ToDouble(dtTmp.ToOADate());
				}
			}
			return listDataSource.Count;
		}

		public int SetCustomDividends(ref double[] dDivAmounts, ref double[] dDivYears)
		{
			listDataSource.Clear();
			for (int iCount = 0; iCount < dDivAmounts.Length; iCount++)
				listDataSource.Add(new Record(DateTime.FromOADate(dDivYears[iCount]), dDivAmounts[iCount]));
			
			return 0;
		}

      
        private void OK_Click(object sender, EventArgs e)
        {
            if (!CheckDatesOnOk())
            {
                return;
            }
            if (CheckNoDataDupletsOnOk())
            {
                listDataSource = (ArrayList)listDataSourceVisible.Clone();
                this.DialogResult = DialogResult.OK;
                this.Close();
            }
            else
            {
                MessageBox.Show("Duplicated dividend dates found!");
            }
        }
	}

	public class Record 
	{
		DateTime dtDivDate;
		double  dDivAmount;
        public Record(ref Object rO)
        {
            this.dtDivDate = ((Record)rO).dtDivDate;
            this.dDivAmount = ((Record)rO).dDivAmount;
        }

		public Record(DateTime dtInDate, double dInAmount) 
		{
			this.dtDivDate  = dtInDate;
			this.dDivAmount = dInAmount;
		}
			
		public double DivAmount 
		{
			get { return dDivAmount; }
			set { dDivAmount = value; }
		}

		public long DaysToDiv 
		{
			get 
			{ 
				int iDaysToDiv  = Convert.ToInt32(dtDivDate.ToOADate() - DateTime.Today.ToOADate());
				if(iDaysToDiv <=0)	iDaysToDiv =0;

				return iDaysToDiv;
			}
			set { dtDivDate = DateTime.Today.AddDays(value); }
		}
		public DateTime DivDate 
		{
			get { return dtDivDate ; }
			set { dtDivDate  = value; }
		}

	}




}
